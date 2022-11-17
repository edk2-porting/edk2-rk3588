/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include "Mvpp2Lib.h"
#include "Mvpp2LibHw.h"
#include "Pp2Dxe.h"

/* Parser configuration routines */

/* Update parser Tcam and Sram hw entries */
STATIC
INT32
Mvpp2PrsHwWrite (
  IN MVPP2_SHARED *Priv,
  IN OUT MVPP2_PRS_ENTRY *Pe
  )
{
  INT32 i;

  if (Pe->Index > MVPP2_PRS_TCAM_SRAM_SIZE - 1) {
    return MVPP2_EINVAL;
  }

  /* Clear entry invalidation bit */
  Pe->Tcam.Word[MVPP2_PRS_TCAM_INV_WORD] &= ~MVPP2_PRS_TCAM_INV_MASK;

  /* Write Tcam Index - indirect access */
  Mvpp2Write (Priv, MVPP2_PRS_TCAM_IDX_REG, Pe->Index);
  for (i = 0; i < MVPP2_PRS_TCAM_WORDS; i++) {
    Mvpp2Write (Priv, MVPP2_PRS_TCAM_DATA_REG(i), Pe->Tcam.Word[i]);
  }

  /* Write Sram Index - indirect access */
  Mvpp2Write (Priv, MVPP2_PRS_SRAM_IDX_REG, Pe->Index);
  for (i = 0; i < MVPP2_PRS_SRAM_WORDS; i++) {
    Mvpp2Write (Priv, MVPP2_PRS_SRAM_DATA_REG(i), Pe->Sram.Word[i]);
  }

  return 0;
}

/* Read Tcam entry from hw */
STATIC
INT32
Mvpp2PrsHwRead (
  IN MVPP2_SHARED *Priv,
  IN OUT MVPP2_PRS_ENTRY *Pe
  )
{
  INT32 i;

  if (Pe->Index > MVPP2_PRS_TCAM_SRAM_SIZE - 1) {
    return MVPP2_EINVAL;
  }

  /* Write Tcam Index - indirect access */
  Mvpp2Write (Priv, MVPP2_PRS_TCAM_IDX_REG, Pe->Index);

  Pe->Tcam.Word[MVPP2_PRS_TCAM_INV_WORD] =
    Mvpp2Read (Priv, MVPP2_PRS_TCAM_DATA_REG(MVPP2_PRS_TCAM_INV_WORD));
  if (Pe->Tcam.Word[MVPP2_PRS_TCAM_INV_WORD] & MVPP2_PRS_TCAM_INV_MASK) {
    return MVPP2_PRS_TCAM_ENTRY_INVALID;
  }

  for (i = 0; i < MVPP2_PRS_TCAM_WORDS; i++) {
    Pe->Tcam.Word[i] = Mvpp2Read (Priv, MVPP2_PRS_TCAM_DATA_REG(i));
  }

  /* Write Sram Index - indirect access */
  Mvpp2Write (Priv, MVPP2_PRS_SRAM_IDX_REG, Pe->Index);
  for (i = 0; i < MVPP2_PRS_SRAM_WORDS; i++) {
    Pe->Sram.Word[i] = Mvpp2Read (Priv, MVPP2_PRS_SRAM_DATA_REG(i));
  }

  return 0;
}

/* Invalidate Tcam hw entry */
STATIC
VOID
Mvpp2PrsHwInv (
  IN MVPP2_SHARED *Priv,
  IN INT32 Index
  )
{
  /* Write Index - indirect access */
  Mvpp2Write (Priv, MVPP2_PRS_TCAM_IDX_REG, Index);
  Mvpp2Write (Priv, MVPP2_PRS_TCAM_DATA_REG(MVPP2_PRS_TCAM_INV_WORD),
        MVPP2_PRS_TCAM_INV_MASK);
}

/* Enable shadow table entry and set its lookup ID */
STATIC
VOID
Mvpp2PrsShadowSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 Index,
  IN INT32 Lu
  )
{
  Priv->PrsShadow[Index].Valid = TRUE;
  Priv->PrsShadow[Index].Lu = Lu;
}

/* Update Ri fields in shadow table entry */
STATIC
VOID
Mvpp2PrsShadowRiSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 Index,
  IN UINT32 Ri,
  IN UINT32 RiMask
  )
{
  Priv->PrsShadow[Index].RiMask = RiMask;
  Priv->PrsShadow[Index].Ri = Ri;
}

/* Update lookup field in Tcam sw entry */
STATIC
VOID
Mvpp2PrsTcamLuSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 Lu
  )
{
  INT32 EnableOff = MVPP2_PRS_TCAM_EN_OFFS (MVPP2_PRS_TCAM_LU_BYTE);

  Pe->Tcam.Byte[MVPP2_PRS_TCAM_LU_BYTE] = Lu;
  Pe->Tcam.Byte[EnableOff] = MVPP2_PRS_LU_MASK;
}

/* Update Mask for single Port in Tcam sw entry */
STATIC
VOID
Mvpp2PrsTcamPortSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 PortId,
  IN BOOLEAN Add
  )
{
  INT32 EnableOff = MVPP2_PRS_TCAM_EN_OFFS (MVPP2_PRS_TCAM_PORT_BYTE);

  if (Add) {
    Pe->Tcam.Byte[EnableOff] &= ~(1 << PortId);
  } else {
    Pe->Tcam.Byte[EnableOff] |= 1 << PortId;
  }
}

/* Update Port map in Tcam sw entry */
STATIC
VOID
Mvpp2PrsTcamPortMapSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 PortMask
  )
{
  INT32 EnableOff = MVPP2_PRS_TCAM_EN_OFFS (MVPP2_PRS_TCAM_PORT_BYTE);
  UINT8 Mask = MVPP2_PRS_PORT_MASK;

  Pe->Tcam.Byte[MVPP2_PRS_TCAM_PORT_BYTE] = 0;
  Pe->Tcam.Byte[EnableOff] &= ~Mask;
  Pe->Tcam.Byte[EnableOff] |= ~PortMask & MVPP2_PRS_PORT_MASK;
}

/* Obtain Port map from Tcam sw entry */
STATIC
UINT32
Mvpp2PrsTcamPortMapGet (
  IN MVPP2_PRS_ENTRY *Pe
  )
{
  INT32 EnableOff = MVPP2_PRS_TCAM_EN_OFFS (MVPP2_PRS_TCAM_PORT_BYTE);

  return ~(Pe->Tcam.Byte[EnableOff]) & MVPP2_PRS_PORT_MASK;
}

/* Set Byte of data and its enable bits in Tcam sw entry */
STATIC
VOID
Mvpp2PrsTcamDataByteSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 Offs,
  IN UINT8 Byte,
  IN UINT8 Enable
  )
{
  Pe->Tcam.Byte[MVPP2_PRS_TCAM_DATA_BYTE(Offs)] = Byte;
  Pe->Tcam.Byte[MVPP2_PRS_TCAM_DATA_BYTE_EN(Offs)] = Enable;
}

/* Get Byte of data and its enable bits from Tcam sw entry */
STATIC
VOID
Mvpp2PrsTcamDataByteGet (
  IN MVPP2_PRS_ENTRY *Pe,
  IN UINT32 Offs,
  OUT UINT8 *Byte,
  OUT UINT8 *Enable
  )
{
  *Byte = Pe->Tcam.Byte[MVPP2_PRS_TCAM_DATA_BYTE(Offs)];
  *Enable = Pe->Tcam.Byte[MVPP2_PRS_TCAM_DATA_BYTE_EN(Offs)];
}

/* Compare Tcam data bytes with a pattern */
STATIC
BOOLEAN
Mvpp2PrsTcamDataCmp (
  IN MVPP2_PRS_ENTRY *Pe,
  IN INT32 Offset,
  IN UINT16 Data
  )
{
  INT32 ByteOffset = MVPP2_PRS_TCAM_DATA_BYTE(Offset);
  UINT16 TcamData;

  TcamData = (Pe->Tcam.Byte[ByteOffset + 1] << 8) | Pe->Tcam.Byte[ByteOffset];
  if (TcamData != Data) {
    return FALSE;
  }

  return TRUE;
}

/* Update ai bits in Tcam sw entry */
STATIC
VOID
Mvpp2PrsTcamAiUpdate (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 Bits,
  IN UINT32 Enable
  )
{
  INT32 i, AiIdx = MVPP2_PRS_TCAM_AI_BYTE;

  for (i = 0; i < MVPP2_PRS_AI_BITS; i++) {

    if (!(Enable & BIT (i))) {
      continue;
    }

    if (Bits & BIT (i)) {
      Pe->Tcam.Byte[AiIdx] |= 1 << i;
    } else {
      Pe->Tcam.Byte[AiIdx] &= ~(1 << i);
    }
  }

  Pe->Tcam.Byte[MVPP2_PRS_TCAM_EN_OFFS (AiIdx)] |= Enable;
}

/* Get ai bits from Tcam sw entry */
STATIC
INT32
Mvpp2PrsTcamAiGet (
  IN MVPP2_PRS_ENTRY *Pe
  )
{
  return Pe->Tcam.Byte[MVPP2_PRS_TCAM_AI_BYTE];
}

/* Get word of data and its enable bits from Tcam sw entry */
STATIC
VOID
Mvpp2PrsTcamDataWordGet (
  IN MVPP2_PRS_ENTRY *Pe,
  IN UINT32 DataOffset,
  OUT UINT32 *Word,
  OUT UINT32 *Enable
  )
{
  INT32 Index, Position;
  UINT8 Byte, Mask;

  for (Index = 0; Index < 4; Index++) {
    Position = (DataOffset * sizeof (INT32)) + Index;
    Mvpp2PrsTcamDataByteGet (Pe, Position, &Byte, &Mask);
    ((UINT8 *)Word)[Index] = Byte;
    ((UINT8 *)Enable)[Index] = Mask;
  }
}

/* Set ethertype in Tcam sw entry */
STATIC
VOID
Mvpp2PrsMatchEtype (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN INT32 Offset,
  IN UINT16 EtherType
  )
{
  Mvpp2PrsTcamDataByteSet (Pe, Offset + 0, EtherType >> 8, 0xff);
  Mvpp2PrsTcamDataByteSet (Pe, Offset + 1, EtherType & 0xff, 0xff);
}

/* Set bits in Sram sw entry */
STATIC
VOID
Mvpp2PrsSramBitsSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN INT32 BitNum,
  IN INT32 Val
  )
{
  Pe->Sram.Byte[MVPP2_BIT_TO_BYTE(BitNum)] |= (Val << (BitNum % 8));
}

/* Clear bits in Sram sw entry */
STATIC
VOID
Mvpp2PrsSramBitsClear (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN INT32 BitNum,
  IN INT32 Val
  )
{
  Pe->Sram.Byte[MVPP2_BIT_TO_BYTE(BitNum)] &= ~(Val << (BitNum % 8));
}

/* Update Ri bits in Sram sw entry */
STATIC
VOID
Mvpp2PrsSramRiUpdate (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 bits,
  IN UINT32 Mask
  )
{
  UINT32 i;

  for (i = 0; i < MVPP2_PRS_SRAM_RI_CTRL_BITS; i++) {
    INT32 RiOff = MVPP2_PRS_SRAM_RI_OFFS;

    if (!(Mask & BIT (i))) {
      continue;
    }

    if (bits & BIT (i)) {
      Mvpp2PrsSramBitsSet (Pe, RiOff + i, 1);
    } else {
      Mvpp2PrsSramBitsClear (Pe, RiOff + i, 1);
    }

    Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_RI_CTRL_OFFS + i, 1);
  }
}

/* Obtain Ri bits from Sram sw entry */
STATIC
INT32
Mvpp2PrsSramRiGet (
  IN MVPP2_PRS_ENTRY *Pe
  )
{
  return Pe->Sram.Word[MVPP2_PRS_SRAM_RI_WORD];
}

/* Update ai bits in Sram sw entry */
STATIC
VOID
Mvpp2PrsSramAiUpdate (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 Bits,
  UINT32 Mask
  )
{
  UINT32 i;
  INT32 AiOff = MVPP2_PRS_SRAM_AI_OFFS;

  for (i = 0; i < MVPP2_PRS_SRAM_AI_CTRL_BITS; i++) {

    if (!(Mask & BIT (i))) {
      continue;
    }

    if (Bits & BIT (i)) {
      Mvpp2PrsSramBitsSet (Pe, AiOff + i, 1);
    } else {
      Mvpp2PrsSramBitsClear (Pe, AiOff + i, 1);
    }

    Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_AI_CTRL_OFFS + i, 1);
  }
}

/* Read ai bits from Sram sw entry */
STATIC
INT32
Mvpp2PrsSramAiGet (
  IN MVPP2_PRS_ENTRY *Pe
  )
{
  UINT8 bits;
  INT32 AiOff = MVPP2_BIT_TO_BYTE(MVPP2_PRS_SRAM_AI_OFFS);
  INT32 AiEnOff = AiOff + 1;
  INT32 AiShift = MVPP2_PRS_SRAM_AI_OFFS % 8;

  bits = (Pe->Sram.Byte[AiOff] >> AiShift) |
         (Pe->Sram.Byte[AiEnOff] << (8 - AiShift));

  return bits;
}

/*
 * In Sram sw entry set lookup ID field of the
 * Tcam key to be used in the next lookup iteration
 */
STATIC
VOID
Mvpp2PrsSramNextLuSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 Lu
  )
{
  INT32 SramNextOff = MVPP2_PRS_SRAM_NEXT_LU_OFFS;

  Mvpp2PrsSramBitsClear (Pe, SramNextOff, MVPP2_PRS_SRAM_NEXT_LU_MASK);
  Mvpp2PrsSramBitsSet (Pe, SramNextOff, Lu);
}

/*
 * In the Sram sw entry set sign and value of the next lookup Offset
 * and the Offset value generated to the classifier
 */
STATIC
VOID
Mvpp2PrsSramShiftSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN INT32 Shift,
  IN UINT32 Op
  )
{
  /* Set sign */
  if (Shift < 0) {
    Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_SHIFT_SIGN_BIT, 1);
    Shift = -Shift;
  } else {
    Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_SHIFT_SIGN_BIT, 1);
  }

  /* Set value */
  Pe->Sram.Byte[MVPP2_BIT_TO_BYTE(MVPP2_PRS_SRAM_SHIFT_OFFS)] = (UINT8)Shift;

  /* Reset and set operation */
  Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_OP_SEL_SHIFT_OFFS, MVPP2_PRS_SRAM_OP_SEL_SHIFT_MASK);
  Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_OP_SEL_SHIFT_OFFS, Op);

  /* Set base Offset as current */
  Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_OP_SEL_BASE_OFFS, 1);
}

/*
 * In the Sram sw entry set sign and value of the user defined offset
 * generated for the classifier
 */
STATIC
VOID
Mvpp2PrsSramOffsetSet (
  IN OUT MVPP2_PRS_ENTRY *Pe,
  IN UINT32 Type,
  IN INT32 Offset,
  IN UINT32 Op
  )
{
  UINT8 UdfByte = MVPP2_BIT_TO_BYTE(MVPP2_PRS_SRAM_UDF_OFFS + MVPP2_PRS_SRAM_UDF_BITS);
  UINT8 UdfByteOffset = (8 - (MVPP2_PRS_SRAM_UDF_OFFS % 8));
  UINT8 OpSelUdfByte = MVPP2_BIT_TO_BYTE(MVPP2_PRS_SRAM_OP_SEL_UDF_OFFS + MVPP2_PRS_SRAM_OP_SEL_UDF_BITS);
  UINT8 OpSelUdfByteOffset = (8 - (MVPP2_PRS_SRAM_OP_SEL_UDF_OFFS % 8));

  /* Set sign */
  if (Offset < 0) {
    Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_UDF_SIGN_BIT, 1);
    Offset = -Offset;
  } else {
    Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_UDF_SIGN_BIT, 1);
  }

  /* Set value */
  Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_UDF_OFFS, MVPP2_PRS_SRAM_UDF_MASK);
  Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_UDF_OFFS, Offset);

  Pe->Sram.Byte[UdfByte] &= ~(MVPP2_PRS_SRAM_UDF_MASK >> UdfByteOffset);
  Pe->Sram.Byte[UdfByte] |= (Offset >> UdfByteOffset);

  /* Set Offset Type */
  Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_UDF_TYPE_OFFS, MVPP2_PRS_SRAM_UDF_TYPE_MASK);
  Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_UDF_TYPE_OFFS, Type);

  /* Set Offset operation */
  Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_OP_SEL_UDF_OFFS, MVPP2_PRS_SRAM_OP_SEL_UDF_MASK);
  Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_OP_SEL_UDF_OFFS, Op);

  Pe->Sram.Byte[OpSelUdfByte] &= ~(MVPP2_PRS_SRAM_OP_SEL_UDF_MASK >> OpSelUdfByteOffset);
  Pe->Sram.Byte[OpSelUdfByte] |= (Op >> OpSelUdfByteOffset);

  /* Set base Offset as current */
  Mvpp2PrsSramBitsClear (Pe, MVPP2_PRS_SRAM_OP_SEL_BASE_OFFS, 1);
}

/* Find parser Flow entry */
STATIC
MVPP2_PRS_ENTRY *
Mvpp2PrsFlowFind (
  IN MVPP2_SHARED *Priv,
  IN INT32 Flow
  )
{
  MVPP2_PRS_ENTRY *Pe;
  INT32 Tid;
  UINT32 Word, Enable;

  Pe = Mvpp2Alloc (sizeof (*Pe));
  if (Pe == NULL) {
    return NULL;
  }

  Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_FLOWS);

  /* Go through the all entires with MVPP2_PRS_LU_FLOWS */
  for (Tid = MVPP2_PRS_TCAM_SRAM_SIZE - 1; Tid >= 0; Tid--) {
    UINT8 Bits;

    if (!Priv->PrsShadow[Tid].Valid || Priv->PrsShadow[Tid].Lu != MVPP2_PRS_LU_FLOWS) {
      continue;
    }

    Pe->Index = Tid;
    Mvpp2PrsHwRead (Priv, Pe);

    /*
     * Check result info, because there maybe
     * several TCAM lines to generate the same Flow
     */
    Mvpp2PrsTcamDataWordGet (Pe, 0, &Word, &Enable);
    if ((Word != 0) || (Enable != 0)) {
      continue;
    }

    Bits = Mvpp2PrsSramAiGet (Pe);

    /* Sram store classification lookup ID in AI Bits [5:0] */
    if ((Bits & MVPP2_PRS_FLOW_ID_MASK) == Flow) {
      return Pe;
    }
  }

  Mvpp2Free (Pe);

  return NULL;
}

/* Return first free Tcam Index, seeking from start to end */
STATIC
INT32
Mvpp2PrsTcamFirstFree (
  IN MVPP2_SHARED *Priv,
  IN UINT8 Start,
  IN UINT8 End
  )
{
  INT32 Tid;

  if (Start > End) {
    Mvpp2SwapVariables (Start, End);
  }

  if (End >= MVPP2_PRS_TCAM_SRAM_SIZE) {
    End = MVPP2_PRS_TCAM_SRAM_SIZE - 1;
  }

  for (Tid = Start; Tid <= End; Tid++) {
    if (!Priv->PrsShadow[Tid].Valid) {
      return Tid;
    }
  }

  return MVPP2_EINVAL;
}

/* Enable/disable dropping all mac Da's */
STATIC
VOID
Mvpp2PrsMacDropAllSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN BOOLEAN Add
  )
{
  MVPP2_PRS_ENTRY Pe;

  if (Priv->PrsShadow[MVPP2_PE_DROP_ALL].Valid) {
    /* Entry exist - update PortId only */
    Pe.Index = MVPP2_PE_DROP_ALL;
    Mvpp2PrsHwRead (Priv, &Pe);
  } else {
    /* Entry doesn't exist - create new */
    Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_MAC);
    Pe.Index = MVPP2_PE_DROP_ALL;

    /* Non-promiscuous mode for all Ports - DROP unknown packets */
    Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_DROP_MASK, MVPP2_PRS_RI_DROP_MASK);

    Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
    Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);

    /* Update shadow table */
    Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_MAC);

    /* Mask all Ports */
    Mvpp2PrsTcamPortMapSet (&Pe, 0);
  }

  /* Update PortId Mask */
  Mvpp2PrsTcamPortSet (&Pe, PortId, Add);

  Mvpp2PrsHwWrite (Priv, &Pe);
}

/* Set port to promiscuous mode */
VOID
Mvpp2PrsMacPromiscSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN BOOLEAN Add
  )
{
  MVPP2_PRS_ENTRY Pe;

  /* Promiscuous mode - Accept unknown packets */

  if (Priv->PrsShadow[MVPP2_PE_MAC_PROMISCUOUS].Valid) {
    /* Entry exist - update port only */
    Pe.Index = MVPP2_PE_MAC_PROMISCUOUS;
    Mvpp2PrsHwRead (Priv, &Pe);
  } else {
    /* Entry doesn't exist - create new */
    Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_MAC);
    Pe.Index = MVPP2_PE_MAC_PROMISCUOUS;

    /* Continue - set next lookup */
    Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_DSA);

    /* Set result info bits */
    Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L2_UCAST, MVPP2_PRS_RI_L2_CAST_MASK);

    /* Shift to ethertype with 2 of MAC Address length */
    Mvpp2PrsSramShiftSet (&Pe, 2 * MV_ETH_ALEN, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

    /* Mask all Ports */
    Mvpp2PrsTcamPortMapSet (&Pe, 0);

    /* Update shadow table */
    Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_MAC);
  }

  /* Update port Mask */
  Mvpp2PrsTcamPortSet (&Pe, PortId, Add);

  Mvpp2PrsHwWrite (Priv, &Pe);
}

/* Accept multicast */
VOID
Mvpp2PrsMacMultiSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN INT32 Index,
  IN BOOLEAN Add
  )
{
  MVPP2_PRS_ENTRY Pe;
  UINT8 DaMc;

  /*
   * Ethernet multicast Address first Byte is
   * 0x01 for IPv4 and 0x33 for IPv6
   */
  DaMc = (Index == MVPP2_PE_MAC_MC_ALL) ? 0x01 : 0x33;

  if (Priv->PrsShadow[Index].Valid) {
    /* Entry exist - update port only */
    Pe.Index = Index;
    Mvpp2PrsHwRead (Priv, &Pe);
  } else {
    /* Entry doesn't exist - create new */
    Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_MAC);
    Pe.Index = Index;

    /* Continue - set next lookup */
    Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_DSA);

    /* Set result info bits */
    Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L2_MCAST, MVPP2_PRS_RI_L2_CAST_MASK);

    /* Update Tcam entry data first Byte */
    Mvpp2PrsTcamDataByteSet (&Pe, 0, DaMc, 0xff);

    /* Shift to ethertype */
    Mvpp2PrsSramShiftSet (&Pe, 2 * MV_ETH_ALEN, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

    /* Mask all ports */
    Mvpp2PrsTcamPortMapSet (&Pe, 0);

    /* Update shadow table */
    Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_MAC);
  }

  /* Update port Mask */
  Mvpp2PrsTcamPortSet (&Pe, PortId, Add);

  Mvpp2PrsHwWrite (Priv, &Pe);
}

/* Set entry for dsa packets */
STATIC
VOID
Mvpp2PrsDsaTagSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN BOOLEAN Add,
  IN BOOLEAN Tagged,
  IN BOOLEAN Extend
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid, Shift;

  if (Extend) {
    Tid = Tagged ? MVPP2_PE_EDSA_TAGGED : MVPP2_PE_EDSA_UNTAGGED;
    Shift = 8;
  } else {
    Tid = Tagged ? MVPP2_PE_DSA_TAGGED : MVPP2_PE_DSA_UNTAGGED;
    Shift = 4;
  }

  if (Priv->PrsShadow[Tid].Valid) {
    /* Entry exist - update port only */
    Pe.Index = Tid;
    Mvpp2PrsHwRead (Priv, &Pe);
  } else {
    /* Entry doesn't exist - create new */
    Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_DSA);
    Pe.Index = Tid;

    /* Shift 4 bytes if DSA tag or 8 bytes in case of EDSA tag*/
    Mvpp2PrsSramShiftSet (&Pe, Shift, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

    /* Update shadow table */
    Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_DSA);

    if (Tagged) {
      /* Set Tagged bit in DSA tag */
      Mvpp2PrsTcamDataByteSet (&Pe, 0, MVPP2_PRS_TCAM_DSA_TAGGED_BIT, MVPP2_PRS_TCAM_DSA_TAGGED_BIT);

      /* Clear all ai bits for next iteration */
      Mvpp2PrsSramAiUpdate (&Pe, 0, MVPP2_PRS_SRAM_AI_MASK);

      /* If packet is Tagged continue check vlans */
      Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_VLAN);
    } else {
      /* Set result info bits to 'no vlans' */
      Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_VLAN_NONE, MVPP2_PRS_RI_VLAN_MASK);
      Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_L2);
    }

    /* Mask all Ports */
    Mvpp2PrsTcamPortMapSet (&Pe, 0);
  }

  /* Update port Mask */
  Mvpp2PrsTcamPortSet (&Pe, PortId, Add);

  Mvpp2PrsHwWrite (Priv, &Pe);
}

/* Set entry for dsa ethertype */
STATIC
VOID
Mvpp2PrsDsaTagEthertypeSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN BOOLEAN Add,
  IN BOOLEAN Tagged,
  IN BOOLEAN Extend
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid, Shift, PortMask;

  if (Extend) {
    Tid = Tagged ? MVPP2_PE_ETYPE_EDSA_TAGGED : MVPP2_PE_ETYPE_EDSA_UNTAGGED;
    PortMask = 0;
    Shift = 8;
  } else {
    Tid = Tagged ? MVPP2_PE_ETYPE_DSA_TAGGED : MVPP2_PE_ETYPE_DSA_UNTAGGED;
    PortMask = MVPP2_PRS_PORT_MASK;
    Shift = 4;
  }

  if (Priv->PrsShadow[Tid].Valid) {
    /* Entry exist - update PortId only */
    Pe.Index = Tid;
    Mvpp2PrsHwRead (Priv, &Pe);
  } else {
    /* Entry doesn't exist - create new */
    Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_DSA);
    Pe.Index = Tid;

    /*
     * Set ethertype at offset 0 for DSA and
     * clear it at offset 2 - obtained from Marvell.
     */
    Mvpp2PrsMatchEtype (&Pe, 0, MV_ETH_P_EDSA);
    Mvpp2PrsMatchEtype (&Pe, 2, 0);

    Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_DSA_MASK,
           MVPP2_PRS_RI_DSA_MASK);

    /* Shift ethertype + 2 Byte reserved + tag */
    Mvpp2PrsSramShiftSet (&Pe, 2 + MVPP2_ETH_TYPE_LEN + Shift,
           MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

    /* Update shadow table */
    Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_DSA);

    if (Tagged) {
      /* Set Tagged bit in DSA tag */
      Mvpp2PrsTcamDataByteSet (
                      &Pe,
                      MVPP2_ETH_TYPE_LEN + 2 + 3,
                      MVPP2_PRS_TCAM_DSA_TAGGED_BIT,
                      MVPP2_PRS_TCAM_DSA_TAGGED_BIT
                    );

      /* Clear all ai bits for next iteration */
      Mvpp2PrsSramAiUpdate (&Pe, 0, MVPP2_PRS_SRAM_AI_MASK);

      /* If packet is Tagged continue check vlans */
      Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_VLAN);
    } else {
      /* Set result info bits to 'no vlans' */
      Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_VLAN_NONE, MVPP2_PRS_RI_VLAN_MASK);
      Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_L2);
    }

    /* Mask/unmask all ports, depending on dsa type */
    Mvpp2PrsTcamPortMapSet (&Pe, PortMask);
  }

  /* Update port Mask */
  Mvpp2PrsTcamPortSet (&Pe, PortId, Add);

  Mvpp2PrsHwWrite (Priv, &Pe);
}

/* Search for existing single/triple vlan entry */
STATIC
MVPP2_PRS_ENTRY *
Mvpp2PrsVlanFind (
  IN MVPP2_SHARED *Priv,
  IN UINT16 Tpid,
  IN INT32 Ai
  )
{
  MVPP2_PRS_ENTRY *Pe;
  INT32 Tid;

  Pe = Mvpp2Alloc (sizeof (*Pe));
  if (Pe == NULL) {
    return NULL;
  }

  Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_VLAN);

  /* Go through the all entries with MVPP2_PRS_LU_VLAN */
  for (Tid = MVPP2_PE_FIRST_FREE_TID; Tid <= MVPP2_PE_LAST_FREE_TID; Tid++) {
    UINT32 RiBits, AiBits;
    BOOLEAN match;

    if (!Priv->PrsShadow[Tid].Valid || Priv->PrsShadow[Tid].Lu != MVPP2_PRS_LU_VLAN) {
      continue;
    }

    Pe->Index = Tid;

    Mvpp2PrsHwRead (Priv, Pe);
    match = Mvpp2PrsTcamDataCmp (Pe, 0, Mvpp2SwapBytes16 (Tpid));
    if (!match) {
      continue;
    }

    /* Get vlan type */
    RiBits = Mvpp2PrsSramRiGet (Pe);
    RiBits &= MVPP2_PRS_RI_VLAN_MASK;

    /* Get current Ai value from Tcam */
    AiBits = Mvpp2PrsTcamAiGet (Pe);

    /* Clear double vlan bit */
    AiBits &= ~MVPP2_PRS_DBL_VLAN_AI_BIT;

    if (Ai != AiBits) {
      continue;
    }

    if (RiBits == MVPP2_PRS_RI_VLAN_SINGLE || RiBits == MVPP2_PRS_RI_VLAN_TRIPLE) {
      return Pe;
    }
  }

  Mvpp2Free (Pe);

  return NULL;
}

/* Add/update single/triple vlan entry */
INT32
Mvpp2PrsVlanAdd (
  IN MVPP2_SHARED *Priv,
  IN UINT16 Tpid,
  IN INT32 Ai,
  IN UINT32 PortMap
  )
{
  MVPP2_PRS_ENTRY *Pe;
  INT32 TidAux, Tid;
  INT32 Ret = 0;

  Pe = Mvpp2PrsVlanFind (Priv, Tpid, Ai);

  if (!Pe) {
    /* Create new Tcam entry */
    Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_LAST_FREE_TID, MVPP2_PE_FIRST_FREE_TID);
    if (Tid < 0) {
      return Tid;
    }

    Pe = Mvpp2Alloc (sizeof (*Pe));
    if (Pe == NULL) {
      return MVPP2_ENOMEM;
    }

    /* Get last double vlan Tid */
    for (TidAux = MVPP2_PE_LAST_FREE_TID; TidAux >= MVPP2_PE_FIRST_FREE_TID; TidAux--) {
      UINT32 RiBits;

      if (!Priv->PrsShadow[TidAux].Valid || Priv->PrsShadow[TidAux].Lu != MVPP2_PRS_LU_VLAN) {
        continue;
      }

      Pe->Index = TidAux;
      Mvpp2PrsHwRead (Priv, Pe);
      RiBits = Mvpp2PrsSramRiGet (Pe);
      if ((RiBits & MVPP2_PRS_RI_VLAN_MASK) == MVPP2_PRS_RI_VLAN_DOUBLE) {
        break;
      }
    }

    if (Tid <= TidAux) {
      Ret = MVPP2_EINVAL;
      goto error;
    }

    Mvpp2Memset (Pe, 0 , sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_VLAN);
    Pe->Index = Tid;

    /* Set VLAN type's offset to 0 bytes - obtained from Marvell */
    Mvpp2PrsMatchEtype (Pe, 0, Tpid);

    Mvpp2PrsSramNextLuSet (Pe, MVPP2_PRS_LU_L2);

    /* Shift 4 bytes - skip 1 vlan tag */
    Mvpp2PrsSramShiftSet (Pe, MVPP2_VLAN_TAG_LEN,
           MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

    /* Clear all Ai bits for next iteration */
    Mvpp2PrsSramAiUpdate (Pe, 0, MVPP2_PRS_SRAM_AI_MASK);

    if (Ai == MVPP2_PRS_SINGLE_VLAN_AI) {
      Mvpp2PrsSramRiUpdate (Pe, MVPP2_PRS_RI_VLAN_SINGLE, MVPP2_PRS_RI_VLAN_MASK);
    } else {
      Ai |= MVPP2_PRS_DBL_VLAN_AI_BIT;
      Mvpp2PrsSramRiUpdate (Pe, MVPP2_PRS_RI_VLAN_TRIPLE, MVPP2_PRS_RI_VLAN_MASK);
    }

    Mvpp2PrsTcamAiUpdate (Pe, Ai, MVPP2_PRS_SRAM_AI_MASK);

    Mvpp2PrsShadowSet (Priv, Pe->Index, MVPP2_PRS_LU_VLAN);
  }

  /* Update Ports' Mask */
  Mvpp2PrsTcamPortMapSet (Pe, PortMap);
  Mvpp2PrsHwWrite (Priv, Pe);

error:
  Mvpp2Free (Pe);

  return Ret;
}

/* Get first free double vlan ai number */
INT32
Mvpp2PrsDoubleVlanAiFreeGet (
  IN MVPP2_SHARED *Priv
  )
{
  INT32 i;

  for (i = 1; i < MVPP2_PRS_DBL_VLANS_MAX; i++) {
    if (!Priv->PrsDoubleVlans[i]) {
      return i;
    }
  }

  return MVPP2_EINVAL;
}

/* Search for existing double vlan entry */
MVPP2_PRS_ENTRY *Mvpp2PrsDoubleVlanFind (
  IN MVPP2_SHARED *Priv,
  IN UINT16 Tpid1,
  IN UINT16 Tpid2
  )
{
  MVPP2_PRS_ENTRY *Pe;
  INT32 Tid;

  Pe = Mvpp2Alloc (sizeof (*Pe));
  if (Pe == NULL) {
    return NULL;
  }

  Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_VLAN);

  /* Go through the all entries with MVPP2_PRS_LU_VLAN */
  for (Tid = MVPP2_PE_FIRST_FREE_TID; Tid <= MVPP2_PE_LAST_FREE_TID; Tid++) {
    UINT32 RiMask;
    BOOLEAN match;

    if (!Priv->PrsShadow[Tid].Valid || Priv->PrsShadow[Tid].Lu != MVPP2_PRS_LU_VLAN) {
      continue;
    }

    Pe->Index = Tid;
    Mvpp2PrsHwRead (Priv, Pe);

    match = Mvpp2PrsTcamDataCmp (Pe, 0, Mvpp2SwapBytes16 (Tpid1)) &&
            Mvpp2PrsTcamDataCmp (Pe, 4, Mvpp2SwapBytes16 (Tpid2));

    if (!match) {
      continue;
    }

    RiMask = Mvpp2PrsSramRiGet (Pe) & MVPP2_PRS_RI_VLAN_MASK;
    if (RiMask == MVPP2_PRS_RI_VLAN_DOUBLE) {
      return Pe;
    }
  }

  Mvpp2Free (Pe);

  return NULL;
}

/* Add or update double vlan entry */
INT32
Mvpp2PrsDoubleVlanAdd (
  IN MVPP2_SHARED *Priv,
  IN UINT16 Tpid1,
  IN UINT16 Tpid2,
  IN UINT32 PortMap
  )
{
  MVPP2_PRS_ENTRY *Pe;
  INT32 TidAux, Tid, Ai, Ret = 0;

  Pe = Mvpp2PrsDoubleVlanFind (Priv, Tpid1, Tpid2);

  if (!Pe) {
    /* Create new Tcam entry */
    Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
    if (Tid < 0) {
      return Tid;
    }

    Pe = Mvpp2Alloc (sizeof (*Pe));
    if (Pe == NULL) {
      return MVPP2_ENOMEM;
    }

    /* Set Ai value for new double vlan entry */
    Ai = Mvpp2PrsDoubleVlanAiFreeGet (Priv);
    if (Ai < 0) {
      Ret = Ai;
      goto error;
    }

    /* Get first single/triple vlan Tid */
    for (TidAux = MVPP2_PE_FIRST_FREE_TID; TidAux <= MVPP2_PE_LAST_FREE_TID; TidAux++) {
      UINT32 RiBits;

      if (!Priv->PrsShadow[TidAux].Valid || Priv->PrsShadow[TidAux].Lu != MVPP2_PRS_LU_VLAN) {
        continue;
      }

      Pe->Index = TidAux;
      Mvpp2PrsHwRead (Priv, Pe);
      RiBits = Mvpp2PrsSramRiGet (Pe);
      RiBits &= MVPP2_PRS_RI_VLAN_MASK;

      if (RiBits == MVPP2_PRS_RI_VLAN_SINGLE || RiBits == MVPP2_PRS_RI_VLAN_TRIPLE) {
        break;
      }
    }

    if (Tid >= TidAux) {
      Ret = MVPP2_ERANGE;
      goto error;
    }

    Mvpp2Memset (Pe, 0, sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_VLAN);
    Pe->Index = Tid;

    Priv->PrsDoubleVlans[Ai] = TRUE;

    /* Set both VLAN types' offsets to 0 and 4 bytes - obtained from Marvell */
    Mvpp2PrsMatchEtype (Pe, 0, Tpid1);
    Mvpp2PrsMatchEtype (Pe, 4, Tpid2);

    Mvpp2PrsSramNextLuSet (Pe, MVPP2_PRS_LU_VLAN);

    /* Shift 8 bytes - skip 2 vlan tags */
    Mvpp2PrsSramShiftSet (Pe, 2 * MVPP2_VLAN_TAG_LEN, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);
    Mvpp2PrsSramRiUpdate (Pe, MVPP2_PRS_RI_VLAN_DOUBLE, MVPP2_PRS_RI_VLAN_MASK);
    Mvpp2PrsSramAiUpdate (Pe, Ai | MVPP2_PRS_DBL_VLAN_AI_BIT, MVPP2_PRS_SRAM_AI_MASK);

    Mvpp2PrsShadowSet (Priv, Pe->Index, MVPP2_PRS_LU_VLAN);
  }

  /* Update Ports' Mask */
  Mvpp2PrsTcamPortMapSet (Pe, PortMap);
  Mvpp2PrsHwWrite (Priv, Pe);

error:
  Mvpp2Free (Pe);
  return Ret;
}

/* IPv4 header parsing for fragmentation and L4 Offset */
STATIC
INT32
Mvpp2PrsIp4Proto (
  IN MVPP2_SHARED *Priv,
  IN UINT16 Proto,
  IN UINT32 Ri,
  IN UINT32 RiMask
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid;

  if ((Proto != MV_IPPR_TCP) && (Proto != MV_IPPR_UDP) && (Proto != MV_IPPR_IGMP)) {
    return MVPP2_EINVAL;
  }

  /* Fragmented packet */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Pe.Index = Tid;

  /* Set next Lu to IPv4 - 12 bytes shift */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Mvpp2PrsSramShiftSet (&Pe, 12, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

  /* Set L4 offset 4 bytes relative IPv4 header size (current position) */
  Mvpp2PrsSramOffsetSet (
                  &Pe,
                  MVPP2_PRS_SRAM_UDF_TYPE_L4,
                  sizeof (Mvpp2Iphdr) - 4,
                  MVPP2_PRS_SRAM_OP_SEL_UDF_ADD
                );

  Mvpp2PrsSramAiUpdate (&Pe, MVPP2_PRS_IPV4_DIP_AI_BIT, MVPP2_PRS_IPV4_DIP_AI_BIT);
  Mvpp2PrsSramRiUpdate (&Pe, Ri | MVPP2_PRS_RI_IP_FRAG_MASK, RiMask | MVPP2_PRS_RI_IP_FRAG_MASK);

  Mvpp2PrsTcamDataByteSet (&Pe, 5, Proto, MVPP2_PRS_TCAM_PROTO_MASK);
  Mvpp2PrsTcamAiUpdate (&Pe, 0, MVPP2_PRS_IPV4_DIP_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Not fragmented packet */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Pe.Index = Tid;

  /* Clear Ri before updating */
  Pe.Sram.Word[MVPP2_PRS_SRAM_RI_WORD] = 0x0;
  Pe.Sram.Word[MVPP2_PRS_SRAM_RI_CTRL_WORD] = 0x0;
  Mvpp2PrsSramRiUpdate (&Pe, Ri, RiMask);

  Mvpp2PrsTcamDataByteSet (&Pe, 2, 0x00, MVPP2_PRS_TCAM_PROTO_MASK_L);
  Mvpp2PrsTcamDataByteSet (&Pe, 3, 0x00, MVPP2_PRS_TCAM_PROTO_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* IPv4 L3 multicast or broadcast */
STATIC
INT32
Mvpp2PrsIp4Cast (
  IN MVPP2_SHARED *Priv,
  IN UINT16 L3Cast
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Mask, Tid;

  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Pe.Index = Tid;

  switch (L3Cast) {
  case MVPP2_PRS_L3_MULTI_CAST:
    Mvpp2PrsTcamDataByteSet (&Pe, 0, MVPP2_PRS_IPV4_MC, MVPP2_PRS_IPV4_MC_MASK);
    Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_MCAST, MVPP2_PRS_RI_L3_ADDR_MASK);
    break;
  case  MVPP2_PRS_L3_BROAD_CAST:
    Mask = MVPP2_PRS_IPV4_BC_MASK;
    Mvpp2PrsTcamDataByteSet (&Pe, 0, Mask, Mask);
    Mvpp2PrsTcamDataByteSet (&Pe, 1, Mask, Mask);
    Mvpp2PrsTcamDataByteSet (&Pe, 2, Mask, Mask);
    Mvpp2PrsTcamDataByteSet (&Pe, 3, Mask, Mask);
    Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_BCAST, MVPP2_PRS_RI_L3_ADDR_MASK);
    break;
  default:
    return MVPP2_EINVAL;
  }

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);

  Mvpp2PrsTcamAiUpdate (&Pe, MVPP2_PRS_IPV4_DIP_AI_BIT, MVPP2_PRS_IPV4_DIP_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* Set entries for protocols over IPv6  */
STATIC
INT32
Mvpp2PrsIp6Proto (
  IN MVPP2_SHARED *Priv,
  IN UINT16 Proto,
  IN UINT32 Ri,
  IN UINT32 RiMask
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid;

  if ((Proto != MV_IPPR_TCP) && (Proto != MV_IPPR_UDP) &&
      (Proto != MV_IPPR_ICMPV6) && (Proto != MV_IPPR_IPIP))
  {
    return MVPP2_EINVAL;
  }

  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Pe.Index = Tid;

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramRiUpdate (&Pe, Ri, RiMask);

  /* Set offset for protocol 6 bytes relative to IPv6 header size */
  Mvpp2PrsSramOffsetSet (
                  &Pe,
                  MVPP2_PRS_SRAM_UDF_TYPE_L4,
                  sizeof (Mvpp2Ipv6hdr) - 6,
                  MVPP2_PRS_SRAM_OP_SEL_UDF_ADD
                );

  Mvpp2PrsTcamDataByteSet (&Pe, 0, Proto, MVPP2_PRS_TCAM_PROTO_MASK);
  Mvpp2PrsTcamAiUpdate (&Pe, MVPP2_PRS_IPV6_NO_EXT_AI_BIT, MVPP2_PRS_IPV6_NO_EXT_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Write HW */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP6);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* IPv6 L3 multicast entry */
STATIC
INT32
Mvpp2PrsIp6Cast (
  IN MVPP2_SHARED *Priv,
  IN UINT16 L3Cast
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid;

  if (L3Cast != MVPP2_PRS_L3_MULTI_CAST) {
    return MVPP2_EINVAL;
  }

  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Pe.Index = Tid;

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_MCAST, MVPP2_PRS_RI_L3_ADDR_MASK);
  Mvpp2PrsSramAiUpdate (&Pe, MVPP2_PRS_IPV6_NO_EXT_AI_BIT, MVPP2_PRS_IPV6_NO_EXT_AI_BIT);

  /* Shift back to IPv6 by 18 bytes - byte count provided by Marvell */
  Mvpp2PrsSramShiftSet (&Pe, -18, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

  Mvpp2PrsTcamDataByteSet (&Pe, 0, MVPP2_PRS_IPV6_MC, MVPP2_PRS_IPV6_MC_MASK);
  Mvpp2PrsTcamAiUpdate (&Pe, 0, MVPP2_PRS_IPV6_NO_EXT_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP6);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* Parser per-Port initialization */
STATIC
VOID
Mvpp2PrsHwPortInit (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN INT32 LuFirst,
  IN INT32 LuMax,
  IN INT32 Offset
  )
{
  UINT32 Val;

  /* Set lookup ID */
  Val = Mvpp2Read (Priv, MVPP2_PRS_INIT_LOOKUP_REG);
  Val &= ~MVPP2_PRS_PORT_LU_MASK (PortId);
  Val |=  MVPP2_PRS_PORT_LU_VAL (PortId, LuFirst);
  Mvpp2Write (Priv, MVPP2_PRS_INIT_LOOKUP_REG, Val);

  /* Set maximum number of loops for packet received from PortId */
  Val = Mvpp2Read (Priv, MVPP2_PRS_MAX_LOOP_REG(PortId));
  Val &= ~MVPP2_PRS_MAX_LOOP_MASK (PortId);
  Val |= MVPP2_PRS_MAX_LOOP_VAL (PortId, LuMax);
  Mvpp2Write (Priv, MVPP2_PRS_MAX_LOOP_REG(PortId), Val);

  /*
   * Set initial Offset for packet header extraction for the first
   * searching loop
   */
  Val = Mvpp2Read (Priv, MVPP2_PRS_INIT_OFFS_REG(PortId));
  Val &= ~MVPP2_PRS_INIT_OFF_MASK (PortId);
  Val |= MVPP2_PRS_INIT_OFF_VAL (PortId, Offset);
  Mvpp2Write (Priv, MVPP2_PRS_INIT_OFFS_REG(PortId), Val);
}

/* Default Flow entries initialization for all Ports */
STATIC
VOID
Mvpp2PrsDefFlowInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 PortId;

  for (PortId = 0; PortId < MVPP2_MAX_PORTS; PortId++) {
    Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
    Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
    Pe.Index = MVPP2_PE_FIRST_DEFAULT_FLOW - PortId;

    /* Mask all Ports */
    Mvpp2PrsTcamPortMapSet (&Pe, 0);

    /* Set Flow ID*/
    Mvpp2PrsSramAiUpdate (&Pe, PortId, MVPP2_PRS_FLOW_ID_MASK);
    Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_DONE_BIT, 1);

    /* Update shadow table and hw entry */
    Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_FLOWS);
    Mvpp2PrsHwWrite (Priv, &Pe);
  }
}

/* Set default entry for Marvell Header field */
STATIC
VOID
Mvpp2PrsMhInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));

  Pe.Index = MVPP2_PE_MH_DEFAULT;
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_MH);
  Mvpp2PrsSramShiftSet (&Pe, MVPP2_MH_SIZE, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_MAC);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_MH);
  Mvpp2PrsHwWrite (Priv, &Pe);
}

/*
 * Set default entires (place holder) for promiscuous, non-promiscuous and
 * multicast MAC Addresses
 */
STATIC
VOID
Mvpp2PrsMacInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));

  /* Non-promiscuous mode for all Ports - DROP unknown packets */
  Pe.Index = MVPP2_PE_MAC_NON_PROMISCUOUS;
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_MAC);

  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_DROP_MASK, MVPP2_PRS_RI_DROP_MASK);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_MAC);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Place holders only - no Ports */
  Mvpp2PrsMacDropAllSet (Priv, 0, FALSE);
  Mvpp2PrsMacPromiscSet (Priv, 0, FALSE);
  Mvpp2PrsMacMultiSet (Priv, MVPP2_PE_MAC_MC_ALL, 0, FALSE);
  Mvpp2PrsMacMultiSet (Priv, MVPP2_PE_MAC_MC_IP6, 0, FALSE);
}

/* Set default entries for various types of dsa packets */
STATIC
VOID
Mvpp2PrsDsaInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;

  /* None tagged EDSA entry - place holder */
  Mvpp2PrsDsaTagSet (Priv, 0, FALSE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_EDSA);

  /* Tagged EDSA entry - place holder */
  Mvpp2PrsDsaTagSet (Priv, 0, FALSE, MVPP2_PRS_TAGGED, MVPP2_PRS_EDSA);

  /* None tagged DSA entry - place holder */
  Mvpp2PrsDsaTagSet (Priv, 0, FALSE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_DSA);

  /* Tagged DSA entry - place holder */
  Mvpp2PrsDsaTagSet (Priv, 0, FALSE, MVPP2_PRS_TAGGED, MVPP2_PRS_DSA);

  /* None tagged EDSA ethertype entry - place holder*/
  Mvpp2PrsDsaTagEthertypeSet (Priv, 0, FALSE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_EDSA);

  /* Tagged EDSA ethertype entry - place holder*/
  Mvpp2PrsDsaTagEthertypeSet (Priv, 0, FALSE, MVPP2_PRS_TAGGED, MVPP2_PRS_EDSA);

  /* None tagged DSA ethertype entry */
  Mvpp2PrsDsaTagEthertypeSet (Priv, 0, TRUE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_DSA);

  /* Tagged DSA ethertype entry */
  Mvpp2PrsDsaTagEthertypeSet (Priv, 0, TRUE, MVPP2_PRS_TAGGED, MVPP2_PRS_DSA);

  /* Set default entry, in case DSA or EDSA tag not found */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_DSA);
  Pe.Index = MVPP2_PE_DSA_DEFAULT;
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_VLAN);

  /* Shift 0 bytes */
  Mvpp2PrsSramShiftSet (&Pe, 0, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_MAC);

  /* Clear all Sram ai bits for next iteration */
  Mvpp2PrsSramAiUpdate (&Pe, 0, MVPP2_PRS_SRAM_AI_MASK);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  Mvpp2PrsHwWrite (Priv, &Pe);
}

/* Match basic ethertypes */
STATIC
INT32
Mvpp2PrsEtypeInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid;

  /* Ethertype: PPPoE */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_L2);
  Pe.Index = Tid;

  /* Set PPPoE type offset to 0 - obtained from Marvell */
  Mvpp2PrsMatchEtype (&Pe, 0, MV_ETH_P_PPP_SES);

  Mvpp2PrsSramShiftSet (&Pe, MVPP2_PPPOE_HDR_SIZE, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_PPPOE);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_PPPOE_MASK, MVPP2_PRS_RI_PPPOE_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_L2);
  Priv->PrsShadow[Pe.Index].Udf = MVPP2_PRS_UDF_L2_DEF;
  Priv->PrsShadow[Pe.Index].Finish = FALSE;
  Mvpp2PrsShadowRiSet (Priv, Pe.Index, MVPP2_PRS_RI_PPPOE_MASK, MVPP2_PRS_RI_PPPOE_MASK);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Ethertype: ARP */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_L2);
  Pe.Index = Tid;

  /* Set ARP type offset to 0 - obtained from Marvell */
  Mvpp2PrsMatchEtype (&Pe, 0, MV_ETH_P_ARP);

  /* Generate Flow in the next iteration*/
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_ARP, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Set L3 Offset */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_L2);
  Priv->PrsShadow[Pe.Index].Udf = MVPP2_PRS_UDF_L2_DEF;
  Priv->PrsShadow[Pe.Index].Finish = TRUE;
  Mvpp2PrsShadowRiSet (Priv, Pe.Index, MVPP2_PRS_RI_L3_ARP, MVPP2_PRS_RI_L3_PROTO_MASK);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Ethertype: LBTD */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_L2);
  Pe.Index = Tid;

  /* Set LBTD type offset to 0 - obtained from Marvell */
  Mvpp2PrsMatchEtype (&Pe, 0, MVPP2_IP_LBDT_TYPE);

  /* Generate Flow in the next iteration*/
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramRiUpdate (
                  &Pe,
                  MVPP2_PRS_RI_CPU_CODE_RX_SPEC | MVPP2_PRS_RI_UDF3_RX_SPECIAL,
                  MVPP2_PRS_RI_CPU_CODE_MASK | MVPP2_PRS_RI_UDF3_MASK
                );

  /* Set L3 Offset */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_L2);
  Priv->PrsShadow[Pe.Index].Udf = MVPP2_PRS_UDF_L2_DEF;
  Priv->PrsShadow[Pe.Index].Finish = TRUE;
  Mvpp2PrsShadowRiSet (
                  Priv,
                  Pe.Index,
                  MVPP2_PRS_RI_CPU_CODE_RX_SPEC | MVPP2_PRS_RI_UDF3_RX_SPECIAL,
                  MVPP2_PRS_RI_CPU_CODE_MASK | MVPP2_PRS_RI_UDF3_MASK
                );

  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Ethertype: IPv4 without options */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_L2);
  Pe.Index = Tid;

  /* Set IPv4 type offset to 0 - obtained from Marvell */
  Mvpp2PrsMatchEtype (&Pe, 0, MV_ETH_P_IP);
  Mvpp2PrsTcamDataByteSet (
                  &Pe,
                  MVPP2_ETH_TYPE_LEN,
                  MVPP2_PRS_IPV4_HEAD | MVPP2_PRS_IPV4_IHL,
                  MVPP2_PRS_IPV4_HEAD_MASK | MVPP2_PRS_IPV4_IHL_MASK
                );

  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_IP4, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Skip EthType + 4 bytes of IP header */
  Mvpp2PrsSramShiftSet (&Pe, MVPP2_ETH_TYPE_LEN + 4, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

  /* Set L3 Offset */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_L2);
  Priv->PrsShadow[Pe.Index].Udf = MVPP2_PRS_UDF_L2_DEF;
  Priv->PrsShadow[Pe.Index].Finish = FALSE;
  Mvpp2PrsShadowRiSet (Priv, Pe.Index, MVPP2_PRS_RI_L3_IP4, MVPP2_PRS_RI_L3_PROTO_MASK);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Ethertype: IPv4 with options */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Pe.Index = Tid;

  /* Clear Tcam data before updating */
  Pe.Tcam.Byte[MVPP2_PRS_TCAM_DATA_BYTE(MVPP2_ETH_TYPE_LEN)] = 0x0;
  Pe.Tcam.Byte[MVPP2_PRS_TCAM_DATA_BYTE_EN(MVPP2_ETH_TYPE_LEN)] = 0x0;

  Mvpp2PrsTcamDataByteSet (&Pe, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_IPV4_HEAD, MVPP2_PRS_IPV4_HEAD_MASK);

  /* Clear Ri before updating */
  Pe.Sram.Word[MVPP2_PRS_SRAM_RI_WORD] = 0x0;
  Pe.Sram.Word[MVPP2_PRS_SRAM_RI_CTRL_WORD] = 0x0;
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_IP4_OPT, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_L2);
  Priv->PrsShadow[Pe.Index].Udf = MVPP2_PRS_UDF_L2_DEF;
  Priv->PrsShadow[Pe.Index].Finish = FALSE;
  Mvpp2PrsShadowRiSet (Priv, Pe.Index, MVPP2_PRS_RI_L3_IP4_OPT, MVPP2_PRS_RI_L3_PROTO_MASK);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Ethertype: IPv6 without options */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_L2);
  Pe.Index = Tid;

  /* Set IPv6 type offset to 0 - obtained from Marvell */
  Mvpp2PrsMatchEtype (&Pe, 0, MV_ETH_P_IPV6);

  /* Skip DIP of IPV6 header - value provided by Marvell */
  Mvpp2PrsSramShiftSet (&Pe, MVPP2_ETH_TYPE_LEN + 8 + MVPP2_MAX_L3_ADDR_SIZE, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_IP6, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Set L3 Offset */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_L2);
  Priv->PrsShadow[Pe.Index].Udf = MVPP2_PRS_UDF_L2_DEF;
  Priv->PrsShadow[Pe.Index].Finish = FALSE;
  Mvpp2PrsShadowRiSet (Priv, Pe.Index, MVPP2_PRS_RI_L3_IP6, MVPP2_PRS_RI_L3_PROTO_MASK);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Default entry for MVPP2_PRS_LU_L2 - Unknown ethtype */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_L2);
  Pe.Index = MVPP2_PE_ETH_TYPE_UN;

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Generate Flow in the next iteration*/
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_UN, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Set L3 Offset even it's unknown L3 */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_L2);
  Priv->PrsShadow[Pe.Index].Udf = MVPP2_PRS_UDF_L2_DEF;
  Priv->PrsShadow[Pe.Index].Finish = TRUE;
  Mvpp2PrsShadowRiSet (Priv, Pe.Index, MVPP2_PRS_RI_L3_UN, MVPP2_PRS_RI_L3_PROTO_MASK);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/*
 * Configure vlan entries and detect up to 2 successive VLAN tags.
 * Possible options:
 * 0x8100, 0x88A8
 * 0x8100, 0x8100
 * 0x8100, 0x88A8
 */
STATIC
INT32
Mvpp2PrsVlanInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Err;

  /* Double VLAN: 0x8100, 0x88A8 */
  Err = Mvpp2PrsDoubleVlanAdd (Priv, MV_ETH_P_8021Q, MV_ETH_P_8021AD, MVPP2_PRS_PORT_MASK);
  if (Err != 0) {
    return Err;
  }

  /* Double VLAN: 0x8100, 0x8100 */
  Err = Mvpp2PrsDoubleVlanAdd (Priv, MV_ETH_P_8021Q, MV_ETH_P_8021Q, MVPP2_PRS_PORT_MASK);
  if (Err != 0) {
    return Err;
  }

  /* Single VLAN: 0x88a8 */
  Err = Mvpp2PrsVlanAdd (Priv, MV_ETH_P_8021AD, MVPP2_PRS_SINGLE_VLAN_AI, MVPP2_PRS_PORT_MASK);
  if (Err != 0) {
    return Err;
  }

  /* Single VLAN: 0x8100 */
  Err = Mvpp2PrsVlanAdd (Priv, MV_ETH_P_8021Q, MVPP2_PRS_SINGLE_VLAN_AI, MVPP2_PRS_PORT_MASK);
  if (Err != 0) {
    return Err;
  }

  /* Set default double vlan entry */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_VLAN);
  Pe.Index = MVPP2_PE_VLAN_DBL;

  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_L2);

  /* Clear ai for next iterations */
  Mvpp2PrsSramAiUpdate (&Pe, 0, MVPP2_PRS_SRAM_AI_MASK);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_VLAN_DOUBLE, MVPP2_PRS_RI_VLAN_MASK);

  Mvpp2PrsTcamAiUpdate (&Pe, MVPP2_PRS_DBL_VLAN_AI_BIT, MVPP2_PRS_DBL_VLAN_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_VLAN);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Set default vlan none entry */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_VLAN);
  Pe.Index = MVPP2_PE_VLAN_NONE;

  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_L2);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_VLAN_NONE, MVPP2_PRS_RI_VLAN_MASK);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_VLAN);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* Set entries for PPPoE ethertype */
STATIC
INT32
Mvpp2PrsPppoeInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid;

  /* IPv4 over PPPoE with options */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_PPPOE);
  Pe.Index = Tid;

  /* Set IPv4 over PPPoE type offset to 0 - obtained from Marvell */
  Mvpp2PrsMatchEtype (&Pe, 0, MV_PPP_IP);

  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_IP4_OPT, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Skip EthType + 4 bytes of IP header */
  Mvpp2PrsSramShiftSet (&Pe, MVPP2_ETH_TYPE_LEN + 4, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

  /* Set L3 Offset */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_PPPOE);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* IPv4 over PPPoE without options */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Pe.Index = Tid;

  Mvpp2PrsTcamDataByteSet (
                  &Pe,
                  MVPP2_ETH_TYPE_LEN,
                  MVPP2_PRS_IPV4_HEAD | MVPP2_PRS_IPV4_IHL,
                  MVPP2_PRS_IPV4_HEAD_MASK | MVPP2_PRS_IPV4_IHL_MASK
                );

  /* Clear Ri before updating */
  Pe.Sram.Word[MVPP2_PRS_SRAM_RI_WORD] = 0x0;
  Pe.Sram.Word[MVPP2_PRS_SRAM_RI_CTRL_WORD] = 0x0;
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_IP4, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_PPPOE);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* IPv6 over PPPoE */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_PPPOE);
  Pe.Index = Tid;

  /* Set IPv6 over PPPoE type offset to 0 - obtained from Marvell */
  Mvpp2PrsMatchEtype (&Pe, 0, MV_PPP_IPV6);

  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_IP6, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Skip EthType + 4 bytes of IPv6 header */
  Mvpp2PrsSramShiftSet (&Pe, MVPP2_ETH_TYPE_LEN + 4, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

  /* Set L3 Offset */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_PPPOE);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Non-IP over PPPoE */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_PPPOE);
  Pe.Index = Tid;

  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_UN, MVPP2_PRS_RI_L3_PROTO_MASK);

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);

  /* Set L3 Offset even if it's unknown L3 */
  Mvpp2PrsSramOffsetSet (&Pe, MVPP2_PRS_SRAM_UDF_TYPE_L3, MVPP2_ETH_TYPE_LEN, MVPP2_PRS_SRAM_OP_SEL_UDF_ADD);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_PPPOE);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* Initialize entries for IPv4 */
STATIC
INT32
Mvpp2PrsIp4Init (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Err;

  /* Set entries for TCP, UDP and IGMP over IPv4 */
  Err = Mvpp2PrsIp4Proto (Priv, MV_IPPR_TCP, MVPP2_PRS_RI_L4_TCP, MVPP2_PRS_RI_L4_PROTO_MASK);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsIp4Proto (Priv, MV_IPPR_UDP, MVPP2_PRS_RI_L4_UDP, MVPP2_PRS_RI_L4_PROTO_MASK);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsIp4Proto (
                  Priv,
                  MV_IPPR_IGMP,
                  MVPP2_PRS_RI_CPU_CODE_RX_SPEC | MVPP2_PRS_RI_UDF3_RX_SPECIAL,
                  MVPP2_PRS_RI_CPU_CODE_MASK | MVPP2_PRS_RI_UDF3_MASK
                );

  if (Err != 0) {
    return Err;
  }

  /* IPv4 Broadcast */
  Err = Mvpp2PrsIp4Cast (Priv, MVPP2_PRS_L3_BROAD_CAST);
  if (Err != 0) {
    return Err;
  }

  /* IPv4 Multicast */
  Err = Mvpp2PrsIp4Cast (Priv, MVPP2_PRS_L3_MULTI_CAST);
  if (Err != 0) {
    return Err;
  }

  /* Default IPv4 entry for unknown protocols */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Pe.Index = MVPP2_PE_IP4_PROTO_UN;

  /* Set next Lu to IPv4 and shift by 12 bytes - obtained from Marvell*/
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Mvpp2PrsSramShiftSet (&Pe, 12, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

  /* Set L4 offset 4 bytes relative IPv4 header size (current position) */
  Mvpp2PrsSramOffsetSet (
                  &Pe,
                  MVPP2_PRS_SRAM_UDF_TYPE_L4,
                  sizeof (Mvpp2Iphdr) - 4,
                  MVPP2_PRS_SRAM_OP_SEL_UDF_ADD
                );

  Mvpp2PrsSramAiUpdate (&Pe, MVPP2_PRS_IPV4_DIP_AI_BIT, MVPP2_PRS_IPV4_DIP_AI_BIT);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L4_OTHER, MVPP2_PRS_RI_L4_PROTO_MASK);

  Mvpp2PrsTcamAiUpdate (&Pe, 0, MVPP2_PRS_IPV4_DIP_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Default IPv4 entry for unicast Address */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP4);
  Pe.Index = MVPP2_PE_IP4_ADDR_UN;

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_UCAST, MVPP2_PRS_RI_L3_ADDR_MASK);

  Mvpp2PrsTcamAiUpdate (&Pe, MVPP2_PRS_IPV4_DIP_AI_BIT, MVPP2_PRS_IPV4_DIP_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* Initialize entries for IPv6 */
STATIC
INT32
Mvpp2PrsIp6Init (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Tid, Err;

  /* Set entries for TCP, UDP and ICMP over IPv6 */
  Err = Mvpp2PrsIp6Proto (Priv, MV_IPPR_TCP, MVPP2_PRS_RI_L4_TCP, MVPP2_PRS_RI_L4_PROTO_MASK);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsIp6Proto (Priv, MV_IPPR_UDP, MVPP2_PRS_RI_L4_UDP, MVPP2_PRS_RI_L4_PROTO_MASK);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsIp6Proto (
                  Priv,
                  MV_IPPR_ICMPV6,
                  MVPP2_PRS_RI_CPU_CODE_RX_SPEC | MVPP2_PRS_RI_UDF3_RX_SPECIAL,
                  MVPP2_PRS_RI_CPU_CODE_MASK | MVPP2_PRS_RI_UDF3_MASK
                );

  if (Err != 0) {
    return Err;
  }

  /*
   * IPv4 is the last header. This is similar case as 6-TCP or 17-UDP
   * Result Info: UDF7=1, DS lite
   */
  Err = Mvpp2PrsIp6Proto (Priv, MV_IPPR_IPIP, MVPP2_PRS_RI_UDF7_IP6_LITE, MVPP2_PRS_RI_UDF7_MASK);
  if (Err != 0) {
    return Err;
  }

  /* IPv6 multicast */
  Err = Mvpp2PrsIp6Cast (Priv, MVPP2_PRS_L3_MULTI_CAST);
  if (Err != 0) {
    return Err;
  }

  /* Entry for checking hop limit */
  Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, MVPP2_PE_LAST_FREE_TID);
  if (Tid < 0) {
    return Tid;
  }

  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Pe.Index = Tid;

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramRiUpdate (
                  &Pe,
                  MVPP2_PRS_RI_L3_UN | MVPP2_PRS_RI_DROP_MASK,
                  MVPP2_PRS_RI_L3_PROTO_MASK | MVPP2_PRS_RI_DROP_MASK
                );

  Mvpp2PrsTcamDataByteSet (&Pe, 1, 0x00, MVPP2_PRS_IPV6_HOP_MASK);
  Mvpp2PrsTcamAiUpdate (&Pe, MVPP2_PRS_IPV6_NO_EXT_AI_BIT, MVPP2_PRS_IPV6_NO_EXT_AI_BIT);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Default IPv6 entry for unknown protocols */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Pe.Index = MVPP2_PE_IP6_PROTO_UN;

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L4_OTHER, MVPP2_PRS_RI_L4_PROTO_MASK);

  /* Set L4 offset 6 bytes relative IPv6 header size (current position) */
  Mvpp2PrsSramOffsetSet (
                  &Pe,
                  MVPP2_PRS_SRAM_UDF_TYPE_L4,
                  sizeof (Mvpp2Ipv6hdr) - 6,
                  MVPP2_PRS_SRAM_OP_SEL_UDF_ADD
                );

  Mvpp2PrsTcamAiUpdate (&Pe, MVPP2_PRS_IPV6_NO_EXT_AI_BIT, MVPP2_PRS_IPV6_NO_EXT_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Default IPv6 entry for unknown ext protocols */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Pe.Index = MVPP2_PE_IP6_EXT_PROTO_UN;

  /* Finished: go to Flowid generation */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_FLOWS);
  Mvpp2PrsSramBitsSet (&Pe, MVPP2_PRS_SRAM_LU_GEN_BIT, 1);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L4_OTHER, MVPP2_PRS_RI_L4_PROTO_MASK);

  Mvpp2PrsTcamAiUpdate (&Pe, MVPP2_PRS_IPV6_EXT_AI_BIT, MVPP2_PRS_IPV6_EXT_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP4);
  Mvpp2PrsHwWrite (Priv, &Pe);

  /* Default IPv6 entry for unicast Address */
  Mvpp2Memset (&Pe, 0, sizeof (MVPP2_PRS_ENTRY));
  Mvpp2PrsTcamLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Pe.Index = MVPP2_PE_IP6_ADDR_UN;

  /* Finished: go to IPv6 again */
  Mvpp2PrsSramNextLuSet (&Pe, MVPP2_PRS_LU_IP6);
  Mvpp2PrsSramRiUpdate (&Pe, MVPP2_PRS_RI_L3_UCAST, MVPP2_PRS_RI_L3_ADDR_MASK);
  Mvpp2PrsSramAiUpdate (&Pe, MVPP2_PRS_IPV6_NO_EXT_AI_BIT, MVPP2_PRS_IPV6_NO_EXT_AI_BIT);

  /* Shift back to IPv6 by 18 bytes - byte count provided by Marvell */
  Mvpp2PrsSramShiftSet (&Pe, -18, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);
  Mvpp2PrsTcamAiUpdate (&Pe, 0, MVPP2_PRS_IPV6_NO_EXT_AI_BIT);

  /* Unmask all Ports */
  Mvpp2PrsTcamPortMapSet (&Pe, MVPP2_PRS_PORT_MASK);

  /* Update shadow table and hw entry */
  Mvpp2PrsShadowSet (Priv, Pe.Index, MVPP2_PRS_LU_IP6);
  Mvpp2PrsHwWrite (Priv, &Pe);

  return 0;
}

/* Parser default initialization */
INT32
Mvpp2PrsDefaultInit (
  IN MVPP2_SHARED *Priv
  )
{
  INT32 Err, Index, i;

  /* Enable Tcam table */
  Mvpp2Write (Priv, MVPP2_PRS_TCAM_CTRL_REG, MVPP2_PRS_TCAM_EN_MASK);

  /* Clear all Tcam and Sram entries */
  for (Index = 0; Index < MVPP2_PRS_TCAM_SRAM_SIZE; Index++) {
    Mvpp2Write (Priv, MVPP2_PRS_TCAM_IDX_REG, Index);
    for (i = 0; i < MVPP2_PRS_TCAM_WORDS; i++) {
      Mvpp2Write (Priv, MVPP2_PRS_TCAM_DATA_REG(i), 0);
    }

    Mvpp2Write (Priv, MVPP2_PRS_SRAM_IDX_REG, Index);
    for (i = 0; i < MVPP2_PRS_SRAM_WORDS; i++) {
      Mvpp2Write (Priv, MVPP2_PRS_SRAM_DATA_REG(i), 0);
    }
  }

  /* Invalidate all Tcam entries */
  for (Index = 0; Index < MVPP2_PRS_TCAM_SRAM_SIZE; Index++) {
    Mvpp2PrsHwInv (Priv, Index);
  }

  /* Always start from lookup = 0 */
  for (Index = 0; Index < MVPP2_MAX_PORTS; Index++) {
    Mvpp2PrsHwPortInit (Priv, Index, MVPP2_PRS_LU_MH, MVPP2_PRS_PORT_LU_MAX, 0);
  }

  Mvpp2PrsDefFlowInit (Priv);

  Mvpp2PrsMhInit (Priv);

  Mvpp2PrsMacInit (Priv);

  Mvpp2PrsDsaInit (Priv);

  Err = Mvpp2PrsEtypeInit (Priv);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsVlanInit (Priv);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsPppoeInit (Priv);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsIp6Init (Priv);
  if (Err != 0) {
    return Err;
  }

  Err = Mvpp2PrsIp4Init (Priv);
  if (Err != 0) {
    return Err;
  }

  return 0;
}

/* Compare MAC DA with Tcam entry data */
STATIC
BOOLEAN
Mvpp2PrsMacRangeEquals (
  IN MVPP2_PRS_ENTRY *Pe,
  IN const UINT8 *Da,
  IN UINT8 *Mask
  )
{
  UINT8 TcamByte, TcamMask;
  INT32 Index;

  for (Index = 0; Index < MV_ETH_ALEN; Index++) {
    Mvpp2PrsTcamDataByteGet (Pe, Index, &TcamByte, &TcamMask);
    if (TcamMask != Mask[Index]) {
      return FALSE;
    }

    if ((TcamMask & TcamByte) != (Da[Index] & Mask[Index])) {
      return FALSE;
    }
  }

  return TRUE;
}

/* Find Tcam entry with matched pair <MAC DA, Port> */
STATIC
MVPP2_PRS_ENTRY *
Mvpp2PrsMacDaRangeFind (
  IN MVPP2_SHARED *Priv,
  IN INT32 Pmap,
  IN const UINT8 *Da,
  IN UINT8 *Mask,
  IN INT32 UdfType
  )
{
  MVPP2_PRS_ENTRY *Pe;
  INT32 Tid;

  Pe = Mvpp2Alloc (sizeof (*Pe));
  if (Pe == NULL) {
    return NULL;
  }
  Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_MAC);

  /* Go through the all entires with MVPP2_PRS_LU_MAC */
  for (Tid = MVPP2_PE_FIRST_FREE_TID; Tid <= MVPP2_PE_LAST_FREE_TID; Tid++) {
    UINT32 EntryPmap;

    if (!Priv->PrsShadow[Tid].Valid ||
        (Priv->PrsShadow[Tid].Lu != MVPP2_PRS_LU_MAC) ||
        (Priv->PrsShadow[Tid].Udf != UdfType))
    {
      continue;
    }

    Pe->Index = Tid;
    Mvpp2PrsHwRead (Priv, Pe);
    EntryPmap = Mvpp2PrsTcamPortMapGet (Pe);

    if (Mvpp2PrsMacRangeEquals (Pe, Da, Mask) && EntryPmap == Pmap) {
      return Pe;
    }
  }

  Mvpp2Free (Pe);

  return NULL;
}

/* Update parser's mac Da entry */
INT32
Mvpp2PrsMacDaAccept (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN const UINT8 *Da,
  IN BOOLEAN Add
  )
{
  MVPP2_PRS_ENTRY *Pe;
  UINT32 Pmap, Len, Ri;
  UINT8 Mask[MV_ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  INT32 Tid;

  /* Scan TCAM and see if entry with this <MAC DA, PortId> already exist */
  Pe = Mvpp2PrsMacDaRangeFind (Priv, (1 << PortId), Da, Mask, MVPP2_PRS_UDF_MAC_DEF);

  /* No such entry */
  if (Pe == NULL) {
    if (!Add) {
      return 0;
    }

    /* Create new TCAM entry */
    /* Find first range mac entry*/
    for (Tid = MVPP2_PE_FIRST_FREE_TID; Tid <= MVPP2_PE_LAST_FREE_TID; Tid++) {
      if (Priv->PrsShadow[Tid].Valid &&
          (Priv->PrsShadow[Tid].Lu == MVPP2_PRS_LU_MAC) &&
          (Priv->PrsShadow[Tid].Udf == MVPP2_PRS_UDF_MAC_RANGE))
      {
        break;
      }
    }


    /* Go through the all entries from first to last */
    Tid = Mvpp2PrsTcamFirstFree (Priv, MVPP2_PE_FIRST_FREE_TID, Tid - 1);
    if (Tid < 0) {
      return Tid;
    }

    Pe = Mvpp2Alloc (sizeof (*Pe));
    if (Pe == NULL) {
      return -1;
    }

    Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_MAC);
    Pe->Index = Tid;

    /* Mask all Ports */
    Mvpp2PrsTcamPortMapSet (Pe, 0);
  }

  /* Update PortId Mask */
  Mvpp2PrsTcamPortSet (Pe, PortId, Add);

  /* Invalidate the entry if no Ports are left enabled */
  Pmap = Mvpp2PrsTcamPortMapGet (Pe);
  if (Pmap == 0) {
    if (Add) {
      Mvpp2Free (Pe);
      return -1;
    }

    Mvpp2PrsHwInv (Priv, Pe->Index);
    Priv->PrsShadow[Pe->Index].Valid = FALSE;

    Mvpp2Free (Pe);

    return 0;
  }

  /* Continue - set next lookup */
  Mvpp2PrsSramNextLuSet (Pe, MVPP2_PRS_LU_DSA);

  /* Set match on DA */
  Len = MV_ETH_ALEN;
  while (Len--) {
    Mvpp2PrsTcamDataByteSet (Pe, Len, Da[Len], 0xff);
  }

  /* Set result info bits */
  if (Mvpp2IsBroadcastEtherAddr (Da)) {
    Ri = MVPP2_PRS_RI_L2_BCAST;
  } else if (Mvpp2IsMulticastEtherAddr (Da)) {
    Ri = MVPP2_PRS_RI_L2_MCAST;
  } else {
    Ri = MVPP2_PRS_RI_L2_UCAST | MVPP2_PRS_RI_MAC_ME_MASK;
  }

  Mvpp2PrsSramRiUpdate (Pe, Ri, MVPP2_PRS_RI_L2_CAST_MASK | MVPP2_PRS_RI_MAC_ME_MASK);
  Mvpp2PrsShadowRiSet (Priv, Pe->Index, Ri, MVPP2_PRS_RI_L2_CAST_MASK | MVPP2_PRS_RI_MAC_ME_MASK);

  /* Shift to ethertype */
  Mvpp2PrsSramShiftSet (Pe, 2 * MV_ETH_ALEN, MVPP2_PRS_SRAM_OP_SEL_SHIFT_ADD);

  /* Update shadow table and hw entry */
  Priv->PrsShadow[Pe->Index].Udf = MVPP2_PRS_UDF_MAC_DEF;
  Mvpp2PrsShadowSet (Priv, Pe->Index, MVPP2_PRS_LU_MAC);
  Mvpp2PrsHwWrite (Priv, Pe);

  Mvpp2Free (Pe);

  return 0;
}

/* Delete all Port's multicast simple (not range) entries */
VOID
Mvpp2PrsMcastDelAll (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId
  )
{
  MVPP2_PRS_ENTRY Pe;
  INT32 Index, Tid;

  for (Tid = MVPP2_PE_FIRST_FREE_TID; Tid <= MVPP2_PE_LAST_FREE_TID; Tid++) {
    UINT8 Da[MV_ETH_ALEN], DaMask[MV_ETH_ALEN];

    if (!Priv->PrsShadow[Tid].Valid ||
        (Priv->PrsShadow[Tid].Lu != MVPP2_PRS_LU_MAC) ||
        (Priv->PrsShadow[Tid].Udf != MVPP2_PRS_UDF_MAC_DEF))
    {
      continue;
    }

    /* Only simple mac entries */
    Pe.Index = Tid;
    Mvpp2PrsHwRead (Priv, &Pe);

    /* Read mac Addr from entry */
    for (Index = 0; Index < MV_ETH_ALEN; Index++) {
      Mvpp2PrsTcamDataByteGet (&Pe, Index, &Da[Index], &DaMask[Index]);
    }

    if (Mvpp2IsMulticastEtherAddr (Da) && !Mvpp2IsBroadcastEtherAddr (Da)) {
      /* Delete this entry */
      Mvpp2PrsMacDaAccept (Priv, PortId, Da, FALSE);
    }
  }
}

INT32
Mvpp2PrsTagModeSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN INT32 Type
  )
{
  switch (Type) {
  case MVPP2_TAG_TYPE_EDSA:
    /* Add PortId to EDSA entries */
    Mvpp2PrsDsaTagSet (Priv, PortId, TRUE, MVPP2_PRS_TAGGED, MVPP2_PRS_EDSA);
    Mvpp2PrsDsaTagSet (Priv, PortId, TRUE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_EDSA);
    /* Remove PortId from DSA entries */
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_TAGGED, MVPP2_PRS_DSA);
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_DSA);
    break;
  case MVPP2_TAG_TYPE_DSA:
    /* Add PortId to DSA entries */
    Mvpp2PrsDsaTagSet (Priv, PortId, TRUE, MVPP2_PRS_TAGGED, MVPP2_PRS_DSA);
    Mvpp2PrsDsaTagSet (Priv, PortId, TRUE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_DSA);

    /* Remove PortId from EDSA entries */
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_TAGGED, MVPP2_PRS_EDSA);
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_EDSA);
    break;
  case MVPP2_TAG_TYPE_MH:
  case MVPP2_TAG_TYPE_NONE:
    /* Remove PortId form EDSA and DSA entries */
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_TAGGED, MVPP2_PRS_DSA);
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_DSA);
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_TAGGED, MVPP2_PRS_EDSA);
    Mvpp2PrsDsaTagSet (Priv, PortId, FALSE, MVPP2_PRS_UNTAGGED, MVPP2_PRS_EDSA);
    break;
  default:
    if ((Type < 0) || (Type > MVPP2_TAG_TYPE_EDSA)) {
      return MVPP2_EINVAL;
    }
  }

  return 0;
}

/* Set prs Flow for the Port */
INT32
Mvpp2PrsDefFlow (
  IN PP2DXE_PORT *Port
  )
{
  MVPP2_PRS_ENTRY *Pe;
  INT32 Tid;

  Pe = Mvpp2PrsFlowFind (Port->Priv, Port->Id);

  /* Such entry not exist */
  if (Pe == NULL) {
    /* Go through the all entires from last to first */
    Tid = Mvpp2PrsTcamFirstFree (Port->Priv, MVPP2_PE_LAST_FREE_TID, MVPP2_PE_FIRST_FREE_TID);
    if (Tid < 0) {
      return Tid;
    }

    Pe = Mvpp2Alloc (sizeof (*Pe));
    if (Pe == NULL) {
      return MVPP2_ENOMEM;
    }

    Mvpp2PrsTcamLuSet (Pe, MVPP2_PRS_LU_FLOWS);
    Pe->Index = Tid;

    /* Set Flow ID*/
    Mvpp2PrsSramAiUpdate (Pe, Port->Id, MVPP2_PRS_FLOW_ID_MASK);
    Mvpp2PrsSramBitsSet (Pe, MVPP2_PRS_SRAM_LU_DONE_BIT, 1);

    /* Update shadow table */
    Mvpp2PrsShadowSet (Port->Priv, Pe->Index, MVPP2_PRS_LU_FLOWS);
  }

  Mvpp2PrsTcamPortMapSet (Pe, (1 << Port->Id));
  Mvpp2PrsHwWrite (Port->Priv, Pe);
  Mvpp2Free (Pe);

  return 0;
}

/* Classifier configuration routines */

/* Update classification Flow table RegValisters */
STATIC
VOID
Mvpp2ClsFlowWrite (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_CLS_FLOW_ENTRY *Fe
  )
{
  Mvpp2Write (Priv, MVPP2_CLS_FLOW_INDEX_REG, Fe->Index);
  Mvpp2Write (Priv, MVPP2_CLS_FLOW_TBL0_REG, Fe->Data[0]);
  Mvpp2Write (Priv, MVPP2_CLS_FLOW_TBL1_REG, Fe->Data[1]);
  Mvpp2Write (Priv, MVPP2_CLS_FLOW_TBL2_REG, Fe->Data[2]);
}

/* Update classification lookup table RegValister */
VOID
Mvpp2ClsLookupWrite (
  IN MVPP2_SHARED *Priv,
  IN OUT MVPP2_CLS_LOOKUP_ENTRY *Le
  )
{
  UINT32 Val;

  Val = (Le->Way << MVPP2_CLS_LKP_INDEX_WAY_OFFS) | Le->Lkpid;
  Mvpp2Write (Priv, MVPP2_CLS_LKP_INDEX_REG, Val);
  Mvpp2Write (Priv, MVPP2_CLS_LKP_TBL_REG, Le->Data);
}

/* Classifier default initialization */
VOID
Mvpp2ClsInit (
  IN MVPP2_SHARED *Priv
  )
{
  MVPP2_CLS_LOOKUP_ENTRY Le;
  MVPP2_CLS_FLOW_ENTRY Fe;
  INT32 Index;

  /* Enable classifier */
  Mvpp2Write (Priv, MVPP2_CLS_MODE_REG, MVPP2_CLS_MODE_ACTIVE_MASK);

  /* Clear classifier Flow table */
  Mvpp2Memset (&Fe.Data, 0, MVPP2_CLS_FLOWS_TBL_DATA_WORDS);
  for (Index = 0; Index < MVPP2_CLS_FLOWS_TBL_SIZE; Index++) {
    Fe.Index = Index;
    Mvpp2ClsFlowWrite (Priv, &Fe);
  }

  /* Clear classifier lookup table */
  Le.Data = 0;
  for (Index = 0; Index < MVPP2_CLS_LKP_TBL_SIZE; Index++) {
    Le.Lkpid = Index;
    Le.Way = 0;
    Mvpp2ClsLookupWrite (Priv, &Le);

    Le.Way = 1;
    Mvpp2ClsLookupWrite (Priv, &Le);
  }
}

VOID
Mvpp2ClsPortConfig (
  IN PP2DXE_PORT *Port
  )
{
  MVPP2_CLS_LOOKUP_ENTRY Le;
  UINT32 Val;

  /* Set way for the Port */
  Val = Mvpp2Read (Port->Priv, MVPP2_CLS_PORT_WAY_REG);
  Val &= ~MVPP2_CLS_PORT_WAY_MASK (Port->Id);
  Mvpp2Write (Port->Priv, MVPP2_CLS_PORT_WAY_REG, Val);

  /*
   * Pick the entry to be accessed in lookup ID decoding table
   * according to the way and lkpid.
   */
  Le.Lkpid = Port->Id;
  Le.Way = 0;
  Le.Data = 0;

  /* Set initial CPU Queue for receiving packets */
  Le.Data &= ~MVPP2_CLS_LKP_TBL_RXQ_MASK;
  Le.Data |= Port->FirstRxq;

  /* Disable classification engines */
  Le.Data &= ~MVPP2_CLS_LKP_TBL_LOOKUP_EN_MASK;

  /* Update lookup ID table entry */
  Mvpp2ClsLookupWrite (Port->Priv, &Le);
}

/* Set CPU Queue number for oversize packets */
VOID
Mvpp2ClsOversizeRxqSet (
  IN PP2DXE_PORT *Port
  )
{

  Mvpp2Write (
          Port->Priv,
          MVPP2_CLS_OVERSIZE_RXQ_LOW_REG(Port->Id),
          Port->FirstRxq & MVPP2_CLS_OVERSIZE_RXQ_LOW_MASK
        );
}

/* BM helper routines */

VOID
Mvpp2BmPoolHwCreate (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_BMS_POOL *BmPool,
  IN INT32 Size
  )
{
  BmPool->Size = Size;

  Mvpp2Write (Priv, MVPP2_BM_POOL_BASE_REG(BmPool->Id), Lower32Bits (BmPool->PhysAddr));
  Mvpp2Write (Priv, MVPP22_BM_POOL_BASE_HIGH_REG, (Upper32Bits (BmPool->PhysAddr) & MVPP22_BM_POOL_BASE_HIGH_REG));
  Mvpp2Write (Priv, MVPP2_BM_POOL_SIZE_REG(BmPool->Id), BmPool->Size);
}

/* Set Pool buffer Size */
VOID
Mvpp2BmPoolBufsizeSet (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_BMS_POOL *BmPool,
  IN INT32 BufSize
  )
{
  UINT32 Val;

  BmPool->BufSize = BufSize;

  Val = MVPP2_ALIGN (BufSize, 1 << MVPP2_POOL_BUF_SIZE_OFFSET);
  Mvpp2Write (Priv, MVPP2_POOL_BUF_SIZE_REG(BmPool->Id), Val);
}

VOID
Mvpp2BmStop (
  IN MVPP2_SHARED *Priv,
  IN INT32 Pool
  )
{
  UINT32 Val, i;

  for (i = 0; i < MVPP2_BM_SIZE; i++) {
    Mvpp2Read (Priv, MVPP2_BM_PHY_ALLOC_REG(Pool));
  }

  Val = Mvpp2Read (Priv, MVPP2_BM_POOL_CTRL_REG(Pool));
  Val |= MVPP2_BM_STOP_MASK;
  Mvpp2Write (Priv, MVPP2_BM_POOL_CTRL_REG(Pool), Val);
}

VOID
Mvpp2BmIrqClear (
  IN MVPP2_SHARED *Priv,
  IN INT32 Pool
  )
{
  /* Mask BM all interrupts */
  Mvpp2Write (Priv, MVPP2_BM_INTR_MASK_REG(Pool), 0);

  /* Clear BM cause RegValister */
  Mvpp2Write (Priv, MVPP2_BM_INTR_CAUSE_REG(Pool), 0);
}

/* Attach long Pool to Rxq */
VOID
Mvpp2RxqLongPoolSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Lrxq,
  IN INT32 LongPool
  )
{
  UINT32 Val;
  INT32 Prxq;

  /* Get Queue physical ID */
  Prxq = Port->Rxqs[Lrxq].Id;

  Val = Mvpp2Read (Port->Priv, MVPP2_RXQ_CONFIG_REG(Prxq));
  Val &= ~MVPP2_RXQ_POOL_LONG_MASK;
  Val |= ((LongPool << MVPP2_RXQ_POOL_LONG_OFFS) & MVPP2_RXQ_POOL_LONG_MASK);

  Mvpp2Write (Port->Priv, MVPP2_RXQ_CONFIG_REG(Prxq), Val);
}

/* Attach short Pool to Rxq */
VOID
Mvpp2RxqShortPoolSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Lrxq,
  IN INT32 ShortPool
  )
{
  UINT32 Val;
  INT32 Prxq;

  /* Get Queue physical ID */
  Prxq = Port->Rxqs[Lrxq].Id;

  Val = Mvpp2Read (Port->Priv, MVPP2_RXQ_CONFIG_REG(Prxq));
  Val &= ~MVPP2_RXQ_POOL_SHORT_MASK;
  Val |= ((ShortPool << MVPP2_RXQ_POOL_SHORT_OFFS) & MVPP2_RXQ_POOL_SHORT_MASK);

  Mvpp2Write (Port->Priv, MVPP2_RXQ_CONFIG_REG(Prxq), Val);
}

/* Release multicast buffer */
VOID
Mvpp2BmPoolMcPut (
  IN PP2DXE_PORT *Port,
  IN INT32 Pool,
  IN UINT32 BufPhysAddr,
  IN UINT32 BufVirtAddr,
  IN INT32 McId
  )
{
  UINT32 Val = 0;

  Val |= (McId & MVPP2_BM_MC_ID_MASK);
  Mvpp2Write (Port->Priv, MVPP2_BM_MC_RLS_REG, Val);

  Mvpp2BmPoolPut (Port->Priv, Pool, BufPhysAddr | MVPP2_BM_PHY_RLS_MC_BUFF_MASK, BufVirtAddr);
}

/* Refill BM Pool */
VOID
Mvpp2PoolRefill (
  IN PP2DXE_PORT *Port,
  IN UINT32 Bm,
  IN UINT32 PhysAddr,
  IN UINT32 cookie
  )
{
  INT32 Pool = Mvpp2BmCookiePoolGet (Bm);

  Mvpp2BmPoolPut (Port->Priv, Pool, PhysAddr, cookie);
}

INTN
Mvpp2BmPoolCtrl (
  IN MVPP2_SHARED *Priv,
  IN INTN Pool,
  IN enum Mvpp2Command Cmd
  )
{
  UINT32 RegVal = 0;
  RegVal = Mvpp2Read (Priv, MVPP2_BM_POOL_CTRL_REG(Pool));

  switch (Cmd) {
  case MVPP2_START:
    RegVal |= MVPP2_BM_START_MASK;
    break;

  case MVPP2_STOP:
    RegVal |= MVPP2_BM_STOP_MASK;
    break;

  default:
    return -1;
  }
  Mvpp2Write (Priv, MVPP2_BM_POOL_CTRL_REG(Pool), RegVal);

  return 0;
}

/* Mask the current CPU's Rx/Tx interrupts */
VOID
Mvpp2InterruptsMask (
  IN VOID *arg
  )
{
  PP2DXE_PORT *Port = arg;

  Mvpp2Write (Port->Priv, MVPP2_ISR_RX_TX_MASK_REG(Port->Id), 0);
}

/* Unmask the current CPU's Rx/Tx interrupts */
VOID
Mvpp2InterruptsUnmask (
  IN VOID *arg
  )
{
  PP2DXE_PORT *Port = arg;

  Mvpp2Write (
          Port->Priv,
          MVPP2_ISR_RX_TX_MASK_REG(Port->Id),
          (MVPP2_CAUSE_MISC_SUM_MASK | MVPP2_CAUSE_RXQ_OCCUP_DESC_ALL_MASK)
        );
}

/* MAC configuration routines */

STATIC
VOID
Mvpp2PortMiiSet (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;

  Val = Mvpp2GmacRead (Port, MVPP2_GMAC_CTRL_2_REG);

  switch (Port->PhyInterface) {
  case MV_MODE_SGMII:
    Val |= MVPP2_GMAC_INBAND_AN_MASK;
    break;
  case MV_MODE_RGMII:
    Val |= MVPP2_GMAC_PORT_RGMII_MASK;
  default:
    Val &= ~MVPP2_GMAC_PCS_ENABLE_MASK;
  }

  Mvpp2GmacWrite (Port, MVPP2_GMAC_CTRL_2_REG, Val);
}

STATIC
VOID Mvpp2PortFcAdvEnable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;

  Val = Mvpp2GmacRead (Port, MVPP2_GMAC_AUTONEG_CONFIG);
  Val |= MVPP2_GMAC_FC_ADV_EN;
  Mvpp2GmacWrite (Port, MVPP2_GMAC_AUTONEG_CONFIG, Val);
}

VOID
Mvpp2PortEnable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;

  Val = Mvpp2GmacRead (Port, MVPP2_GMAC_CTRL_0_REG);
  Val |= MVPP2_GMAC_PORT_EN_MASK;
  Val |= MVPP2_GMAC_MIB_CNTR_EN_MASK;
  Mvpp2GmacWrite (Port, MVPP2_GMAC_CTRL_0_REG, Val);
}

VOID
Mvpp2PortDisable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;

  Val = Mvpp2GmacRead (Port, MVPP2_GMAC_CTRL_0_REG);
  Val &= ~(MVPP2_GMAC_PORT_EN_MASK);
  Mvpp2GmacWrite (Port, MVPP2_GMAC_CTRL_0_REG, Val);
}

/* Set IEEE 802.3x Flow Control Xon Packet Transmission Mode */
STATIC
VOID
Mvpp2PortPeriodicXonDisable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;

  Val = Mvpp2GmacRead (Port, MVPP2_GMAC_CTRL_1_REG) & ~MVPP2_GMAC_PERIODIC_XON_EN_MASK;
  Mvpp2GmacWrite (Port, MVPP2_GMAC_CTRL_1_REG, Val);
}

/* Configure loopback Port */
STATIC
VOID
Mvpp2PortReset (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;

  Val = Mvpp2GmacRead (Port, MVPP2_GMAC_CTRL_2_REG) & ~MVPP2_GMAC_PORT_RESET_MASK;
  Mvpp2GmacWrite (Port, MVPP2_GMAC_CTRL_2_REG, Val);

  while (Mvpp2GmacRead (Port, MVPP2_GMAC_CTRL_2_REG) & MVPP2_GMAC_PORT_RESET_MASK) {
    continue;
  }
}

/* Set defaults to the MVPP2 Port */
VOID
Mvpp2DefaultsSet (
  IN PP2DXE_PORT *Port
  )
{
  INT32 TxPortNum, Val, Queue, pTxq;

  /* Disable Legacy WRR, Disable EJP, Release from Reset */
  TxPortNum = Mvpp2EgressPort (Port);
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_PORT_INDEX_REG, TxPortNum);
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_CMD_1_REG, 0);

  /* Close bandwidth for all Queues */
  for (Queue = 0; Queue < MVPP2_MAX_TXQ; Queue++) {
    pTxq = Mvpp2TxqPhys (Port->Id, Queue);
    Mvpp2Write (Port->Priv, MVPP2_TXQ_SCHED_TOKEN_CNTR_REG(pTxq), 0);
  }


  /* Set refill period to 1 Usec, refill tokens and bucket Size to maximum */
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_PERIOD_REG, Port->Priv->Tclk / MVPP2_USEC_PER_SEC);
  Val = Mvpp2Read (Port->Priv, MVPP2_TXP_SCHED_REFILL_REG);
  Val &= ~MVPP2_TXP_REFILL_PERIOD_ALL_MASK;
  Val |= MVPP2_TXP_REFILL_PERIOD_MASK (1);
  Val |= MVPP2_TXP_REFILL_TOKENS_ALL_MASK;
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_REFILL_REG, Val);
  Val = MVPP2_TXP_TOKEN_SIZE_MAX;
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_TOKEN_SIZE_REG, Val);

  /* Set MaximumLowLatencyPacketSize value to 256 */
  Mvpp2Write (
          Port->Priv,
          MVPP2_RX_CTRL_REG(Port->Id),
          MVPP2_RX_USE_PSEUDO_FOR_CSUM_MASK | MVPP2_RX_LOW_LATENCY_PKT_SIZE (256)
        );

  /* Mask all interrupts to all present cpus */
  Mvpp2InterruptsDisable (Port, 0x1);
}

/* Enable/disable receiving packets */
VOID
Mvpp2IngressEnable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;
  INT32 Lrxq, Queue;

  for (Lrxq = 0; Lrxq < RxqNumber; Lrxq++) {
    Queue = Port->Rxqs[Lrxq].Id;
    Val = Mvpp2Read (Port->Priv, MVPP2_RXQ_CONFIG_REG(Queue));
    Val &= ~MVPP2_RXQ_DISABLE_MASK;
    Mvpp2Write (Port->Priv, MVPP2_RXQ_CONFIG_REG(Queue), Val);
  }
}

VOID
Mvpp2IngressDisable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;
  INT32 Lrxq, Queue;

  for (Lrxq = 0; Lrxq < RxqNumber; Lrxq++) {
    Queue = Port->Rxqs[Lrxq].Id;
    Val = Mvpp2Read (Port->Priv, MVPP2_RXQ_CONFIG_REG(Queue));
    Val |= MVPP2_RXQ_DISABLE_MASK;
    Mvpp2Write (Port->Priv, MVPP2_RXQ_CONFIG_REG(Queue), Val);
  }
}

/* Enable transmit via physical egress Queue - HW starts take descriptors from DRAM */
VOID
Mvpp2EgressEnable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 qmap;
  INT32 Queue;
  INT32 TxPortNum = Mvpp2EgressPort (Port);

  /* Enable all initialized TXs. */
  qmap = 0;
  for (Queue = 0; Queue < TxqNumber; Queue++) {
    MVPP2_TX_QUEUE *Txq = &Port->Txqs[Queue];

    if (Txq->Descs != NULL) {
      qmap |= (1 << Queue);
    }
  }

  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_PORT_INDEX_REG, TxPortNum);
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_Q_CMD_REG, qmap);
}

/* Disable transmit via physical egress Queue - HW doesn't take descriptors from DRAM */
VOID
Mvpp2EgressDisable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegData;
  INT32 Delay;
  INT32 TxPortNum = Mvpp2EgressPort (Port);

  /* Issue stop command for active channels only */
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_PORT_INDEX_REG, TxPortNum);
  RegData = (Mvpp2Read (Port->Priv, MVPP2_TXP_SCHED_Q_CMD_REG)) & MVPP2_TXP_SCHED_ENQ_MASK;
  if (RegData != 0) {
    Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_Q_CMD_REG, (RegData << MVPP2_TXP_SCHED_DISQ_OFFSET));
  }

  /* Wait for all Tx activity to terminate. */
  Delay = 0;
  do {
    if (Delay >= MVPP2_TX_DISABLE_TIMEOUT_MSEC) {
      Mvpp2Printf ("Tx stop timed out, status=0x%08x\n", RegData);
      break;
    }
    Mvpp2Mdelay (1);
    Delay++;

    /* Check Port TX Command RegValister that all Tx Queues are stopped */
    RegData = Mvpp2Read (Port->Priv, MVPP2_TXP_SCHED_Q_CMD_REG);
  } while (RegData & MVPP2_TXP_SCHED_ENQ_MASK);
}

/* Rx descriptors helper methods */

/* Set rx Queue Offset */
STATIC
VOID
Mvpp2RxqOffsetSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Prxq,
  IN INT32 Offset
  )
{
  UINT32 Val;

  /* Convert Offset from bytes to units of 32 bytes */
  Offset = Offset >> 5;

  /* Clear previous value */
  Val = Mvpp2Read (Port->Priv, MVPP2_RXQ_CONFIG_REG(Prxq));
  Val &= ~MVPP2_RXQ_PACKET_OFFSET_MASK;

  /* Update packet Offset in received buffer */
  Val |= ((Offset << MVPP2_RXQ_PACKET_OFFSET_OFFS) & MVPP2_RXQ_PACKET_OFFSET_MASK);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_CONFIG_REG(Prxq), Val);
}

/* Obtain BM cookie information from descriptor */
UINT32
Mvpp2BmCookieBuild (
  IN MVPP2_RX_DESC *RxDesc,
  IN INT32 Cpu
  )
{
  INT32 Pool;
  UINT32 ret;

  Pool = (RxDesc->status & MVPP2_RXD_BM_POOL_ID_MASK) >> MVPP2_RXD_BM_POOL_ID_OFFS;

  ret = ((Pool & 0xFF) << MVPP2_BM_COOKIE_POOL_OFFS) | ((Cpu & 0xFF) << MVPP2_BM_COOKIE_CPU_OFFS);

  return ret;
}

/* Tx descriptors helper methods */

INT32
Mvpp2TxqDrainSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Txq,
  IN BOOLEAN En
  )
{
  UINT32 RegVal;
  INT32 pTxq = Mvpp2TxqPhys (Port->Id, Txq);

  Mvpp2Write (Port->Priv, MVPP2_TXQ_NUM_REG, pTxq);
  RegVal = Mvpp2Read (Port->Priv, MVPP2_TXQ_PREF_BUF_REG);

  if (En) {
    RegVal |= MVPP2_TXQ_DRAIN_EN_MASK;
  } else {
    RegVal &= ~MVPP2_TXQ_DRAIN_EN_MASK;
  }

  Mvpp2Write (Port->Priv, MVPP2_TXQ_PREF_BUF_REG, RegVal);

  return 0;
}

/* Get number of Tx descriptors waiting to be transmitted by HW */
INT32
Mvpp2TxqPendDescNumGet (
  IN PP2DXE_PORT *Port,
  IN MVPP2_TX_QUEUE *Txq
  )
{
  UINT32 Val;

  Mvpp2Write (Port->Priv, MVPP2_TXQ_NUM_REG, Txq->Id);
  Val = Mvpp2Read (Port->Priv, MVPP2_TXQ_PENDING_REG);

  return Val & MVPP2_TXQ_PENDING_MASK;
}

/* Get number of occupied aggRegValated Tx descriptors */
UINT32
Mvpp2AggrTxqPendDescNumGet (
  IN MVPP2_SHARED *Priv,
  IN INT32 Cpu
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Read (Priv, MVPP2_AGGR_TXQ_STATUS_REG(Cpu));

  return RegVal & MVPP2_AGGR_TXQ_PENDING_MASK;
}

/* Get pointer to next Tx descriptor to be processed (send) by HW */
MVPP2_TX_DESC *
Mvpp2TxqNextDescGet (
  MVPP2_TX_QUEUE *Txq
  )
{
  INT32 TxDesc = Txq->NextDescToProc;

  Txq->NextDescToProc = MVPP2_QUEUE_NEXT_DESC (Txq, TxDesc);

  return Txq->Descs + TxDesc;
}

/* Update HW with number of aggRegValated Tx descriptors to be sent */
VOID
Mvpp2AggrTxqPendDescAdd (
  IN PP2DXE_PORT *Port,
  IN INT32 Pending
  )
{
  /* AggRegValated access - relevant TXQ number is written in TX desc */
  Mvpp2Write (Port->Priv, MVPP2_AGGR_TXQ_UPDATE_REG, Pending);
}

/*
 * Check if there are enough free descriptors in aggRegValated Txq.
 * If not, update the number of occupied descriptors and repeat the check.
 */
INT32
Mvpp2AggrDescNumCheck (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_TX_QUEUE *AggrTxq,
  IN INT32 Num,
  IN INT32 Cpu
  )
{
  UINT32 Val;

  if ((AggrTxq->count + Num) > AggrTxq->Size) {
    /* Update number of occupied aggRegValated Tx descriptors */
    Val = Mvpp2Read (Priv, MVPP2_AGGR_TXQ_STATUS_REG(Cpu));
    AggrTxq->count = Val & MVPP2_AGGR_TXQ_PENDING_MASK;
  }

  if ((AggrTxq->count + Num) > AggrTxq->Size) {
    return MVPP2_ENOMEM;
  }

  return 0;
}

/* Reserved Tx descriptors allocation request */
INT32
Mvpp2TxqAllocReservedDesc (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_TX_QUEUE *Txq,
  IN INT32 Num
  )
{
  UINT32 Val;

  Val = (Txq->Id << MVPP2_TXQ_RSVD_REQ_Q_OFFSET) | Num;
  Mvpp2Write (Priv, MVPP2_TXQ_RSVD_REQ_REG, Val);

  Val = Mvpp2Read (Priv, MVPP2_TXQ_RSVD_RSLT_REG);

  return Val & MVPP2_TXQ_RSVD_RSLT_MASK;
}

/*
 * Release the last allocated Tx descriptor. Useful to handle DMA
 * mapping failures in the Tx path.
 */
VOID
Mvpp2TxqDescPut (
  IN MVPP2_TX_QUEUE *Txq
  )
{
  if (Txq->NextDescToProc == 0) {
    Txq->NextDescToProc = Txq->LastDesc - 1;
  } else {
    Txq->NextDescToProc--;
  }
}

/* Set Tx descriptors fields relevant for CSUM calculation */
UINT32
Mvpp2TxqDescCsum (
  IN INT32 L3Offs,
  IN INT32 L3Proto,
  IN INT32 IpHdrLen,
  IN INT32 L4Proto
  )
{
  UINT32 command;

  /*
   * Fields: L3_Offset, IP_hdrlen, L3_type, G_IPV4Chk,
   * G_L4_chk, L4_type required only for checksum calculation
   */
  command = (L3Offs << MVPP2_TXD_L3_OFF_SHIFT);
  command |= (IpHdrLen << MVPP2_TXD_IP_HLEN_SHIFT);
  command |= MVPP2_TXD_IP_CSUM_DISABLE;

  if (L3Proto == Mvpp2SwapBytes16 (MV_ETH_P_IP)) {
    command &= ~MVPP2_TXD_IP_CSUM_DISABLE;  /* enable IPv4 csum */
    command &= ~MVPP2_TXD_L3_IP6;           /* enable IPv4 */
  } else {
    command |= MVPP2_TXD_L3_IP6;            /* enable IPv6 */
  }

  if (L4Proto == MV_IPPR_TCP) {
    command &= ~MVPP2_TXD_L4_UDP;           /* enable TCP */
    command &= ~MVPP2_TXD_L4_CSUM_FRAG;     /* generate L4 csum */
  } else if (L4Proto == MV_IPPR_UDP) {
    command |= MVPP2_TXD_L4_UDP;            /* enable UDP */
    command &= ~MVPP2_TXD_L4_CSUM_FRAG;     /* generate L4 csum */
  } else {
    command |= MVPP2_TXD_L4_CSUM_NOT;
  }

  return command;
}

/* Clear counter of sent packets */
VOID
Mvpp2TxqSentCounterClear (
  IN OUT VOID *arg
  )
{
  PP2DXE_PORT *Port = arg;
  INT32 Queue;

  for (Queue = 0; Queue < TxqNumber; Queue++) {
    INT32 Id = Port->Txqs[Queue].Id;

    Mvpp2Read (Port->Priv, MVPP2_TXQ_SENT_REG(Id));
  }
}

/* Change maximum receive Size of the Port */
VOID
Mvpp2GmacMaxRxSizeSet (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val;

  Val = Mvpp2GmacRead (Port, MVPP2_GMAC_CTRL_0_REG);
  Val &= ~MVPP2_GMAC_MAX_RX_SIZE_MASK;
  Val |= (((Port->PktSize - MVPP2_MH_SIZE) / 2) << MVPP2_GMAC_MAX_RX_SIZE_OFFS);
  Mvpp2GmacWrite (Port, MVPP2_GMAC_CTRL_0_REG, Val);
}

/* Set max sizes for Tx Queues */
VOID
Mvpp2TxpMaxTxSizeSet (
  IN PP2DXE_PORT *Port
  )
{
  UINT32  Val, Size, mtu;
  INT32 Txq, TxPortNum;

  mtu = Port->PktSize * 8;
  if (mtu > MVPP2_TXP_MTU_MAX) {
    mtu = MVPP2_TXP_MTU_MAX;
  }

  /* WA for wrong Token bucket update: Set MTU value = 3*real MTU value */
  mtu = 3 * mtu;

  /* Indirect access to RegValisters */
  TxPortNum = Mvpp2EgressPort (Port);
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_PORT_INDEX_REG, TxPortNum);

  /* Set MTU */
  Val = Mvpp2Read (Port->Priv, MVPP2_TXP_SCHED_MTU_REG);
  Val &= ~MVPP2_TXP_MTU_MAX;
  Val |= mtu;
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_MTU_REG, Val);

  /* TXP token Size and all TXQs token Size must be larger that MTU */
  Val = Mvpp2Read (Port->Priv, MVPP2_TXP_SCHED_TOKEN_SIZE_REG);
  Size = Val & MVPP2_TXP_TOKEN_SIZE_MAX;
  if (Size < mtu) {
    Size = mtu;
    Val &= ~MVPP2_TXP_TOKEN_SIZE_MAX;
    Val |= Size;
    Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_TOKEN_SIZE_REG, Val);
  }

  for (Txq = 0; Txq < TxqNumber; Txq++) {
    Val = Mvpp2Read (Port->Priv, MVPP2_TXQ_SCHED_TOKEN_SIZE_REG(Txq));
    Size = Val & MVPP2_TXQ_TOKEN_SIZE_MAX;

    if (Size < mtu) {
      Size = mtu;
      Val &= ~MVPP2_TXQ_TOKEN_SIZE_MAX;
      Val |= Size;
      Mvpp2Write (Port->Priv, MVPP2_TXQ_SCHED_TOKEN_SIZE_REG(Txq), Val);
    }
  }
}

/*
 * Set the number of packets that will be received before Rx interrupt
 * will be generated by HW.
 */
VOID
Mvpp2RxPktsCoalSet (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq,
  IN UINT32 Pkts
  )
{
  UINT32 Val;

  Val = (Pkts & MVPP2_OCCUPIED_THRESH_MASK);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_NUM_REG, Rxq->Id);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_THRESH_REG, Val);

  Rxq->PktsCoal = Pkts;
}

/* Set the time Delay in Usec before Rx INT32errupt */
VOID
Mvpp2RxTimeCoalSet (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq,
  IN UINT32 Usec
  )
{
  UINT32 Val;

  Val = (Port->Priv->Tclk / MVPP2_USEC_PER_SEC) * Usec;
  Mvpp2Write (Port->Priv, MVPP2_ISR_RX_THRESHOLD_REG(Rxq->Id), Val);

  Rxq->TimeCoal = Usec;
}

/* Rx/Tx Queue initialization/cleanup methods */

/* Configure RXQ's */
VOID
Mvpp2RxqHwInit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq
  )
{
  Rxq->LastDesc = Rxq->Size - 1;

  /* Zero occupied and non-occupied counters - direct access */
  Mvpp2Write (Port->Priv, MVPP2_RXQ_STATUS_REG(Rxq->Id), 0);

  /* Set Rx descriptors Queue starting Address - indirect access */
  Mvpp2Write (Port->Priv, MVPP2_RXQ_NUM_REG, Rxq->Id);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_DESC_ADDR_REG, Rxq->DescsPhys >> MVPP22_DESC_ADDR_SHIFT);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_DESC_SIZE_REG, Rxq->Size);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_INDEX_REG, 0);

  /* Set Offset */
  Mvpp2RxqOffsetSet (Port, Rxq->Id, MVPP2_RXQ_OFFSET);

  /* Set coalescing pkts and time */
  Mvpp2RxPktsCoalSet (Port, Rxq, MVPP2_RX_COAL_PKTS);
  Mvpp2RxTimeCoalSet (Port, Rxq, Rxq->TimeCoal);

  /* Add number of descriptors ready for receiving packets */
  Mvpp2RxqStatusUpdate (Port, Rxq->Id, 0, Rxq->Size);
}

/* Push packets received by the RXQ to BM Pool */
VOID
Mvpp2RxqDropPkts (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq,
  IN INT32 Cpu
  )
{
  INT32 RxReceived;

  RxReceived = Mvpp2RxqReceived (Port, Rxq->Id);
  if (!RxReceived) {
    return;
  }

  Mvpp2RxqStatusUpdate (Port, Rxq->Id, RxReceived, RxReceived);
}

VOID
Mvpp2RxqHwDeinit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq
  )
{
  Rxq->Descs = NULL;
  Rxq->LastDesc = 0;
  Rxq->NextDescToProc = 0;
  Rxq->DescsPhys = 0;

  /*
   * Clear Rx descriptors Queue starting Address and Size;
   * free descriptor number
   */
  Mvpp2Write (Port->Priv, MVPP2_RXQ_STATUS_REG(Rxq->Id), 0);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_NUM_REG, Rxq->Id);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_DESC_ADDR_REG, 0);
  Mvpp2Write (Port->Priv, MVPP2_RXQ_DESC_SIZE_REG, 0);
}

/* Configure TXQ's */
VOID
Mvpp2TxqHwInit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_TX_QUEUE *Txq
  )
{
  INT32 Desc, DescPerTxq, TxPortNum;
  UINT32 Val;

  Txq->LastDesc = Txq->Size - 1;

  /* Set Tx descriptors Queue starting Address - indirect access */
  Mvpp2Write (Port->Priv, MVPP2_TXQ_NUM_REG, Txq->Id);
  Mvpp2Write (Port->Priv, MVPP2_TXQ_DESC_ADDR_REG, Txq->DescsPhys);
  Mvpp2Write (Port->Priv, MVPP2_TXQ_DESC_SIZE_REG, Txq->Size & MVPP2_TXQ_DESC_SIZE_MASK);
  Mvpp2Write (Port->Priv, MVPP2_TXQ_INDEX_REG, 0);
  Mvpp2Write (Port->Priv, MVPP2_TXQ_RSVD_CLR_REG, Txq->Id << MVPP2_TXQ_RSVD_CLR_OFFSET);
  Val = Mvpp2Read (Port->Priv, MVPP2_TXQ_PENDING_REG);
  Val &= ~MVPP2_TXQ_PENDING_MASK;
  Mvpp2Write (Port->Priv, MVPP2_TXQ_PENDING_REG, Val);

  /*
   * Calculate base Address in prefetch buffer. We reserve 16 descriptors
   * for each existing TXQ.
   * TCONTS for PON Port must be continuous from 0 to MVPP2_MAX_TCONT
   * GBE Ports assumed to be continious from 0 to MVPP2_MAX_PORTS
   */
  DescPerTxq = 16;
  Desc = (Port->Id * MVPP2_MAX_TXQ * DescPerTxq) + (Txq->LogId * DescPerTxq);

  Mvpp2Write (
          Port->Priv,
          MVPP2_TXQ_PREF_BUF_REG,
          MVPP2_PREF_BUF_PTR (Desc) | MVPP2_PREF_BUF_SIZE_16 | MVPP2_PREF_BUF_THRESH (DescPerTxq/2)
        );

  /* WRR / EJP configuration - indirect access */
  TxPortNum = Mvpp2EgressPort (Port);
  Mvpp2Write (Port->Priv, MVPP2_TXP_SCHED_PORT_INDEX_REG, TxPortNum);

  Val = Mvpp2Read (Port->Priv, MVPP2_TXQ_SCHED_REFILL_REG(Txq->LogId));
  Val &= ~MVPP2_TXQ_REFILL_PERIOD_ALL_MASK;
  Val |= MVPP2_TXQ_REFILL_PERIOD_MASK (1);
  Val |= MVPP2_TXQ_REFILL_TOKENS_ALL_MASK;
  Mvpp2Write (Port->Priv, MVPP2_TXQ_SCHED_REFILL_REG(Txq->LogId), Val);

  Val = MVPP2_TXQ_TOKEN_SIZE_MAX;
  Mvpp2Write (Port->Priv, MVPP2_TXQ_SCHED_TOKEN_SIZE_REG(Txq->LogId), Val);
}

VOID
Mvpp2TxqHwDeinit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_TX_QUEUE *Txq
  )
{
  Txq->Descs = NULL;
  Txq->LastDesc = 0;
  Txq->NextDescToProc = 0;
  Txq->DescsPhys = 0;

  /* Set minimum bandwidth for disabled TXQs */
  Mvpp2Write (Port->Priv, MVPP2_TXQ_SCHED_TOKEN_CNTR_REG(Txq->Id), 0);

  /* Set Tx descriptors Queue starting Address and Size */
  Mvpp2Write (Port->Priv, MVPP2_TXQ_NUM_REG, Txq->Id);
  Mvpp2Write (Port->Priv, MVPP2_TXQ_DESC_ADDR_REG, 0);
  Mvpp2Write (Port->Priv, MVPP2_TXQ_DESC_SIZE_REG, 0);
}

/* Allocate and initialize descriptors for aggr TXQ */
VOID
Mvpp2AggrTxqHwInit (
  IN OUT MVPP2_TX_QUEUE *AggrTxq,
  IN INT32 DescNum,
  IN INT32 Cpu,
  IN MVPP2_SHARED *Priv
  )
{
  AggrTxq->LastDesc = AggrTxq->Size - 1;

  /* Aggr TXQ no Reset WA */
  AggrTxq->NextDescToProc = Mvpp2Read (Priv, MVPP2_AGGR_TXQ_INDEX_REG(Cpu));

  /* Set Tx descriptors Queue starting Address (indirect access) */
  Mvpp2Write (Priv, MVPP2_AGGR_TXQ_DESC_ADDR_REG(Cpu), AggrTxq->DescsPhys >> MVPP22_DESC_ADDR_SHIFT);
  Mvpp2Write (Priv, MVPP2_AGGR_TXQ_DESC_SIZE_REG(Cpu), DescNum & MVPP2_AGGR_TXQ_DESC_SIZE_MASK);
}

/* Enable gmac */
VOID
Mvpp2PortPowerUp (
  IN PP2DXE_PORT *Port
  )
{
  Mvpp2PortMiiSet (Port);
  Mvpp2PortPeriodicXonDisable (Port);
  Mvpp2PortFcAdvEnable (Port);
  Mvpp2PortReset (Port);
}

/* Initialize Rx FIFO's */
VOID
Mvpp2RxFifoInit (
  IN MVPP2_SHARED *Priv
  )
{
  INT32 PortId;

  for (PortId = 0; PortId < MVPP2_MAX_PORTS; PortId++) {
    Mvpp2Write (Priv, MVPP2_RX_DATA_FIFO_SIZE_REG(PortId), MVPP2_RX_FIFO_PORT_DATA_SIZE);
    Mvpp2Write (Priv, MVPP2_RX_ATTR_FIFO_SIZE_REG(PortId), MVPP2_RX_FIFO_PORT_ATTR_SIZE);
  }

  Mvpp2Write (Priv, MVPP2_RX_MIN_PKT_SIZE_REG, MVPP2_RX_FIFO_PORT_MIN_PKT);
  Mvpp2Write (Priv, MVPP2_RX_FIFO_INIT_REG, 0x1);
}

VOID
MvGop110NetcActivePort (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_NETCOMP_PORTS_CONTROL_1);
  RegVal &= ~(NETC_PORTS_ACTIVE_MASK (PortId));

  Val <<= NETC_PORTS_ACTIVE_OFFSET (PortId);
  Val &= NETC_PORTS_ACTIVE_MASK (PortId);

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_NETCOMP_PORTS_CONTROL_1, RegVal);
}

STATIC
VOID
MvGop110NetcXauiEnable (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, SD1_CONTROL_1_REG);
  RegVal &= ~SD1_CONTROL_XAUI_EN_MASK;

  Val <<= SD1_CONTROL_XAUI_EN_OFFSET;
  Val &= SD1_CONTROL_XAUI_EN_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, SD1_CONTROL_1_REG, RegVal);
}

STATIC
VOID
MvGop110NetcRxaui0Enable (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, SD1_CONTROL_1_REG);
  RegVal &= ~SD1_CONTROL_RXAUI0_L23_EN_MASK;

  Val <<= SD1_CONTROL_RXAUI0_L23_EN_OFFSET;
  Val &= SD1_CONTROL_RXAUI0_L23_EN_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, SD1_CONTROL_1_REG, RegVal);
}

STATIC
VOID
MvGop110NetcRxaui1Enable (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, SD1_CONTROL_1_REG);
  RegVal &= ~SD1_CONTROL_RXAUI1_L45_EN_MASK;

  Val <<= SD1_CONTROL_RXAUI1_L45_EN_OFFSET;
  Val &= SD1_CONTROL_RXAUI1_L45_EN_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, SD1_CONTROL_1_REG, RegVal);
}

STATIC
VOID
MvGop110NetcMiiMode (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_NETCOMP_CONTROL_0);
  RegVal &= ~NETC_GBE_PORT1_MII_MODE_MASK;

  Val <<= NETC_GBE_PORT1_MII_MODE_OFFSET;
  Val &= NETC_GBE_PORT1_MII_MODE_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_NETCOMP_CONTROL_0, RegVal);
}

STATIC
VOID
MvGop110NetcGopReset (
  IN PP2DXE_PORT *Port,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_GOP_SOFT_RESET_1_REG);
  RegVal &= ~NETC_GOP_SOFT_RESET_MASK;

  Val <<= NETC_GOP_SOFT_RESET_OFFSET;
  Val &= NETC_GOP_SOFT_RESET_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_GOP_SOFT_RESET_1_REG, RegVal);
}

STATIC
VOID
MvGop110NetcGopClockLogicSet (
  IN PP2DXE_PORT *Port,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_NETCOMP_PORTS_CONTROL_0);
  RegVal &= ~NETC_CLK_DIV_PHASE_MASK;

  Val <<= NETC_CLK_DIV_PHASE_OFFSET;
  Val &= NETC_CLK_DIV_PHASE_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_NETCOMP_PORTS_CONTROL_0, RegVal);
}

STATIC
VOID
MvGop110NetcPortRfReset (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_NETCOMP_PORTS_CONTROL_1);
  RegVal &= ~(NETC_PORT_GIG_RF_RESET_MASK (PortId));

  Val <<= NETC_PORT_GIG_RF_RESET_OFFSET (PortId);
  Val &= NETC_PORT_GIG_RF_RESET_MASK (PortId);

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_NETCOMP_PORTS_CONTROL_1, RegVal);
}

STATIC
VOID
MvGop110NetcGbeSgmiiModeSelect (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN UINT32 Val
  )
{
  UINT32 RegVal, Mask, Offset;

  if (PortId == 2) {
    Mask = NETC_GBE_PORT0_SGMII_MODE_MASK;
    Offset = NETC_GBE_PORT0_SGMII_MODE_OFFSET;
  } else {
    Mask = NETC_GBE_PORT1_SGMII_MODE_MASK;
    Offset = NETC_GBE_PORT1_SGMII_MODE_OFFSET;
  }
  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_NETCOMP_CONTROL_0);
  RegVal &= ~Mask;

  Val <<= Offset;
  Val &= Mask;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_NETCOMP_CONTROL_0, RegVal);
}

STATIC
VOID
MvGop110NetcBusWidthSelect (
  IN PP2DXE_PORT *Port,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_NETCOMP_PORTS_CONTROL_0);
  RegVal &= ~NETC_BUS_WIDTH_SELECT_MASK;

  Val <<= NETC_BUS_WIDTH_SELECT_OFFSET;
  Val &= NETC_BUS_WIDTH_SELECT_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_NETCOMP_PORTS_CONTROL_0, RegVal);
}

STATIC
VOID
MvGop110NetcSampleStagesTiming (
  IN PP2DXE_PORT *Port,
  IN UINT32 Val
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2Rfu1Read (Port->Priv, MV_NETCOMP_PORTS_CONTROL_0);
  RegVal &= ~NETC_GIG_RX_DATA_SAMPLE_MASK;

  Val <<= NETC_GIG_RX_DATA_SAMPLE_OFFSET;
  Val &= NETC_GIG_RX_DATA_SAMPLE_MASK;

  RegVal |= Val;

  Mvpp2Rfu1Write (Port->Priv, MV_NETCOMP_PORTS_CONTROL_0, RegVal);
}

STATIC
VOID
MvGop110NetcMacToXgmii (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN enum MvNetcPhase Phase
  )
{
  switch (Phase) {
  case MV_NETC_FIRST_PHASE:

    /* Set Bus Width to HB mode = 1 */
    MvGop110NetcBusWidthSelect (Port, 0x1);

    /* Select RGMII mode */
    MvGop110NetcGbeSgmiiModeSelect (Port, PortId, MV_NETC_GBE_XMII);
    break;
  case MV_NETC_SECOND_PHASE:

    /* De-assert the relevant PortId HB Reset */
    MvGop110NetcPortRfReset (Port, PortId, 0x1);
    break;
  }
}

STATIC
VOID
MvGop110NetcMacToSgmii (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN enum MvNetcPhase Phase
  )
{
  switch (Phase) {
  case MV_NETC_FIRST_PHASE:

    /* Set Bus Width to HB mode = 1 */
    MvGop110NetcBusWidthSelect (Port, 1);

    /* Select SGMII mode */
    if (PortId >= 1) {
      MvGop110NetcGbeSgmiiModeSelect (Port, PortId, MV_NETC_GBE_SGMII);
    }

    /* Configure the sample stages */
    MvGop110NetcSampleStagesTiming (Port, 0);
    break;
  case MV_NETC_SECOND_PHASE:

    /* De-assert the relevant PortId HB Reset */
    MvGop110NetcPortRfReset (Port, PortId, 1);
    break;
  }
}

STATIC
VOID
MvGop110NetcMacToRxaui (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN enum MvNetcPhase Phase,
  IN enum MvNetcLanes Lanes
  )
{
  /* Currently only RXAUI0 supPorted */
  if (PortId != 0)
    return;

  switch (Phase) {
  case MV_NETC_FIRST_PHASE:

    /* RXAUI Serdes/s Clock alignment */
    if (Lanes == MV_NETC_LANE_23) {
      MvGop110NetcRxaui0Enable (Port, PortId, 1);
    } else {
      MvGop110NetcRxaui1Enable (Port, PortId, 1);
    }
    break;
  case MV_NETC_SECOND_PHASE:

    /* De-assert the relevant PortId HB Reset */
    MvGop110NetcPortRfReset (Port, PortId, 1);
    break;
  }
}

STATIC
VOID
MvGop110NetcMacToXaui (
  IN PP2DXE_PORT *Port,
  IN UINT32 PortId,
  IN enum MvNetcPhase Phase
  )
{
  switch (Phase) {
  case MV_NETC_FIRST_PHASE:

    /* RXAUI Serdes/s Clock alignment */
    MvGop110NetcXauiEnable (Port, PortId, 1);
    break;
  case MV_NETC_SECOND_PHASE:

    /* De-assert the relevant PortId HB Reset */
    MvGop110NetcPortRfReset (Port, PortId, 1);
    break;
  }
}

INT32
MvGop110NetcInit (
  IN PP2DXE_PORT *Port,
  IN UINT32 NetCompConfig,
  IN enum MvNetcPhase Phase
  )
{
  UINT32 c = NetCompConfig;

  if (c & MV_NETC_GE_MAC0_RXAUI_L23) {
    MvGop110NetcMacToRxaui (Port, 0, Phase, MV_NETC_LANE_23);
  }

  if (c & MV_NETC_GE_MAC0_RXAUI_L45) {
    MvGop110NetcMacToRxaui (Port, 0, Phase, MV_NETC_LANE_45);
  }

  if (c & MV_NETC_GE_MAC0_XAUI) {
    MvGop110NetcMacToXaui (Port, 0, Phase);
  }

  if (c & MV_NETC_GE_MAC2_SGMII) {
    MvGop110NetcMacToSgmii (Port, 2, Phase);
  } else {
    MvGop110NetcMacToXgmii (Port, 2, Phase);
  }

  if (c & MV_NETC_GE_MAC3_SGMII) {
    MvGop110NetcMacToSgmii (Port, 3, Phase);
  } else {
    MvGop110NetcMacToXgmii (Port, 3, Phase);
    if (c & MV_NETC_GE_MAC3_RGMII) {
      MvGop110NetcMiiMode (Port, 3, MV_NETC_GBE_RGMII);
    } else {
      MvGop110NetcMiiMode (Port, 3, MV_NETC_GBE_MII);
    }
  }

  /* Activate gop Ports 0, 2, 3 */
  MvGop110NetcActivePort (Port, 0, 1);
  MvGop110NetcActivePort (Port, 2, 1);
  MvGop110NetcActivePort (Port, 3, 1);

  if (Phase == MV_NETC_SECOND_PHASE) {

    /* Enable the GOP internal clock logic */
    MvGop110NetcGopClockLogicSet (Port, 1);

    /* De-assert GOP unit Reset */
    MvGop110NetcGopReset (Port, 1);
  }

  return 0;
}

UINT32
MvpPp2xGop110NetcCfgCreate (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val = 0;

    if (Port->GopIndex == 0) {
      if (Port->PhyInterface == MV_MODE_XAUI) {
        Val |= MV_NETC_GE_MAC0_XAUI;
      } else if (Port->PhyInterface == MV_MODE_RXAUI) {
        Val |= MV_NETC_GE_MAC0_RXAUI_L23;
      }
    }

    if (Port->GopIndex == 2) {
      if (Port->PhyInterface == MV_MODE_SGMII) {
        Val |= MV_NETC_GE_MAC2_SGMII;
      }
    }

    if (Port->GopIndex == 3) {
      if (Port->PhyInterface == MV_MODE_SGMII) {
        Val |= MV_NETC_GE_MAC3_SGMII;
      } else if (Port->PhyInterface == MV_MODE_RGMII) {
        Val |= MV_NETC_GE_MAC3_RGMII;
      }
    }

  return Val;
}

/*
 * Initialize physical Port. Configure the Port mode and
 * all its elements accordingly.
 */
INT32
MvGop110PortInit (
  IN PP2DXE_PORT *Port
  )
{

  switch (Port->PhyInterface) {
  case MV_MODE_RGMII:
    MvGop110GmacReset (Port, RESET);

    /* Configure PCS */
    MvGop110GpcsModeCfg (Port, FALSE);
    MvGop110BypassClkCfg (Port, TRUE);

    /* Configure MAC */
    MvGop110GmacModeCfg (Port);

    /* PCS unreset */
    MvGop110GpcsReset (Port, UNRESET);

    /* MAC unreset */
    MvGop110GmacReset (Port, UNRESET);
  break;
  case MV_MODE_SGMII:
  case MV_MODE_QSGMII:

    /* Configure PCS */
    MvGop110GpcsModeCfg (Port, TRUE);

    /* Configure MAC */
    MvGop110GmacModeCfg (Port);

    /* Select proper MAC mode */
    MvGop110Xlg2GigMacCfg (Port);

    /* PCS unreset */
    MvGop110GpcsReset (Port, UNRESET);

    /* MAC unreset */
    MvGop110GmacReset (Port, UNRESET);
  break;
  case MV_MODE_SFI:
    /* Configure PCS */
    MvGopXpcsModeCfg (Port, MVPP2_SFI_LANE_COUNT);

    MvGopMpcsModeCfg (Port);

    /* Configure MAC */
    MvGopXlgMacModeCfg (Port);

    /* PCS unreset */
    MvGopXpcsUnreset (Port);

    /* MAC unreset */
    MvGopXlgMacUnreset (Port);
    break;
  default:
    return -1;
  }

  return 0;
}

/* Set the MAC to Reset or exit from Reset */
INT32
MvGop110GmacReset (
  IN PP2DXE_PORT *Port,
  IN enum MvReset ResetCmd
  )
{
  UINT32 RegAddr;
  UINT32 Val;

  RegAddr = MVPP2_PORT_CTRL2_REG;

  Val = MvGop110GmacRead (Port, RegAddr);

  if (ResetCmd == RESET) {
    Val |= MVPP2_PORT_CTRL2_PORTMACRESET_MASK;
  } else {
    Val &= ~MVPP2_PORT_CTRL2_PORTMACRESET_MASK;
  }

  MvGop110GmacWrite (Port, RegAddr, Val);

  return 0;
}

/* Enable/Disable Port to work with Gig PCS */
INT32
MvGop110GpcsModeCfg (
  IN PP2DXE_PORT *Port,
  BOOLEAN En
  )
{
  UINT32 Val;

  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL2_REG);

  if (En) {
    Val |= MVPP2_PORT_CTRL2_PCS_EN_MASK;
  } else {
    Val &= ~MVPP2_PORT_CTRL2_PCS_EN_MASK;
  }

  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL2_REG, Val);

  return 0;
}

INT32
MvGop110BypassClkCfg (
  IN PP2DXE_PORT *Port,
  IN BOOLEAN En
  )
{
  UINT32 Val;

  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL2_REG);

  if (En) {
    Val |= MVPP2_PORT_CTRL2_CLK_125_BYPS_EN_MASK;
  } else {
    Val &= ~MVPP2_PORT_CTRL2_CLK_125_BYPS_EN_MASK;
  }

  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL2_REG, Val);

  return 0;
}

INT32
MvGop110GpcsReset (
  IN PP2DXE_PORT *Port,
  IN enum MvReset ResetCmd
  )
{
  UINT32 RegData;

  RegData = MvGop110GmacRead (Port, MVPP2_PORT_CTRL2_REG);

  if (ResetCmd == RESET) {
    U32_SET_FIELD (
            RegData,
            MVPP2_PORT_CTRL2_SGMII_MODE_MASK,
            0
          );

  } else {
    U32_SET_FIELD (
            RegData,
            MVPP2_PORT_CTRL2_SGMII_MODE_MASK,
            1 << MVPP2_PORT_CTRL2_SGMII_MODE_OFFS
          );

  }

  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL2_REG, RegData);

  return 0;
}

VOID
MvGop110Xlg2GigMacCfg (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegVal;

  /* Relevant only for MAC0 (XLG0 and GMAC0) */
  if (Port->GopIndex > 0) {
    return;
  }

  /* Configure 1Gig MAC mode */
  RegVal = Mvpp2XlgRead (Port, MV_XLG_PORT_MAC_CTRL3_REG);
  U32_SET_FIELD (
          RegVal,
          MV_XLG_MAC_CTRL3_MACMODESELECT_MASK,
          (0 << MV_XLG_MAC_CTRL3_MACMODESELECT_OFFS)
        );

  Mvpp2XlgWrite (Port, MV_XLG_PORT_MAC_CTRL3_REG, RegVal);
}

/* Set the internal mux's to the required MAC in the GOP */
INT32
MvGop110GmacModeCfg (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegAddr;
  UINT32 Val;

  /* Set TX FIFO thresholds */
  switch (Port->PhyInterface) {
  case MV_MODE_SGMII:
    if (Port->Speed == MV_PORT_SPEED_2500) {
      MvGop110GmacSgmii25Cfg (Port);
    } else {
      MvGop110GmacSgmiiCfg (Port);
    }
  break;
  case MV_MODE_RGMII:
    MvGop110GmacRgmiiCfg (Port);
  break;
  case MV_MODE_QSGMII:
    MvGop110GmacQsgmiiCfg (Port);
  break;
  default:
    return -1;
  }

  /* Jumbo frame supPort - 0x1400*2= 0x2800 bytes */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL0_REG);
  U32_SET_FIELD (
          Val,
          MVPP2_PORT_CTRL0_FRAMESIZELIMIT_MASK,
          (0x1400 << MVPP2_PORT_CTRL0_FRAMESIZELIMIT_OFFS)
        );

  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL0_REG, Val);

  /* PeriodicXonEn disable */
  RegAddr = MVPP2_PORT_CTRL1_REG;
  Val = MvGop110GmacRead (Port, RegAddr);
  Val &= ~MVPP2_PORT_CTRL1_EN_PERIODIC_FC_XON_MASK;
  MvGop110GmacWrite (Port, RegAddr, Val);

  /* Mask all Ports interrupts */
  MvGop110GmacPortLinkEventMask (Port);

#if MV_PP2x_INTERRUPT
  /* Unmask link change interrupt */
  Val = MvGop110GmacRead (Port, MVPP2_INTERRUPT_MASK_REG);
  Val |= MVPP2_INTERRUPT_CAUSE_LINK_CHANGE_MASK;
  Val |= 1; /* Unmask summary bit */
  MvGop110GmacWrite (Port, MVPP2_INTERRUPT_MASK_REG, Val);
#endif

  return 0;
}

VOID
MvGop110GmacRgmiiCfg (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val, thresh, an;

  /* Configure minimal level of the Tx FIFO before the lower part starts to read a packet*/
  thresh = MV_RGMII_TX_FIFO_MIN_TH;
  Val = MvGop110GmacRead (Port, MVPP2_PORT_FIFO_CFG_1_REG);
  U32_SET_FIELD (
          Val,
          MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_MASK,
          (thresh << MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_OFFS)
        );

  MvGop110GmacWrite (Port, MVPP2_PORT_FIFO_CFG_1_REG, Val);

  /* Disable bypass of sync module */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL4_REG);
  Val |= MVPP2_PORT_CTRL4_SYNC_BYPASS_MASK;

  /* Configure DP clock select according to mode */
  Val &= ~MVPP2_PORT_CTRL4_DP_CLK_SEL_MASK;
  Val |= MVPP2_PORT_CTRL4_QSGMII_BYPASS_ACTIVE_MASK;
  Val |= MVPP2_PORT_CTRL4_EXT_PIN_GMII_SEL_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL4_REG, Val);

  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL2_REG);
  Val &= ~MVPP2_PORT_CTRL2_DIS_PADING_OFFS;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL2_REG, Val);

  /* Configure GIG MAC to SGMII mode */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL0_REG);
  Val &= ~MVPP2_PORT_CTRL0_PORTTYPE_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL0_REG, Val);

  /* configure AN 0xb8e8 */
  an = MVPP2_PORT_AUTO_NEG_CFG_AN_BYPASS_EN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_AN_SPEED_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_FC_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_FDX_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_CHOOSE_SAMPLE_TX_CONFIG_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_AUTO_NEG_CFG_REG, an);
}

VOID
MvGop110GmacSgmii25Cfg (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val, thresh, an;

  /*
   * Configure minimal level of the Tx FIFO before
   * the lower part starts to read a packet.
   */
  thresh = MV_SGMII2_5_TX_FIFO_MIN_TH;
  Val = MvGop110GmacRead (Port, MVPP2_PORT_FIFO_CFG_1_REG);
  U32_SET_FIELD (
          Val,
          MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_MASK,
          (thresh << MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_OFFS)
        );

  MvGop110GmacWrite (Port, MVPP2_PORT_FIFO_CFG_1_REG, Val);

  /* Disable bypass of sync module */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL4_REG);
  Val |= MVPP2_PORT_CTRL4_SYNC_BYPASS_MASK;

  /* Configure DP clock select according to mode */
  Val |= MVPP2_PORT_CTRL4_DP_CLK_SEL_MASK;

  /* Configure QSGMII bypass according to mode */
  Val |= MVPP2_PORT_CTRL4_QSGMII_BYPASS_ACTIVE_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL4_REG, Val);

  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL2_REG);
  Val |= MVPP2_PORT_CTRL2_DIS_PADING_OFFS;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL2_REG, Val);

  /* Configure GIG MAC to 1000Base-X mode connected to a fiber transceiver */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL0_REG);
  Val |= MVPP2_PORT_CTRL0_PORTTYPE_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL0_REG, Val);

  /* configure AN 0x9268 */
  an = MVPP2_PORT_AUTO_NEG_CFG_EN_PCS_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_AN_BYPASS_EN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_SET_MII_SPEED_MASK  |
       MVPP2_PORT_AUTO_NEG_CFG_SET_GMII_SPEED_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_ADV_PAUSE_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_SET_FULL_DX_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_CHOOSE_SAMPLE_TX_CONFIG_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_AUTO_NEG_CFG_REG, an);
}

VOID
MvGop110GmacSgmiiCfg (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val, thresh, an;

  /*
   * Configure minimal level of the Tx FIFO before
   * the lower part starts to read a packet.
   */
  thresh = MV_SGMII_TX_FIFO_MIN_TH;
  Val = MvGop110GmacRead (Port, MVPP2_PORT_FIFO_CFG_1_REG);
  U32_SET_FIELD (Val, MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_MASK,
    (thresh << MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_OFFS));
  MvGop110GmacWrite (Port, MVPP2_PORT_FIFO_CFG_1_REG, Val);

  /* Disable bypass of sync module */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL4_REG);
  Val |= MVPP2_PORT_CTRL4_SYNC_BYPASS_MASK;

  /* Configure DP clock select according to mode */
  Val &= ~MVPP2_PORT_CTRL4_DP_CLK_SEL_MASK;

  /* Configure QSGMII bypass according to mode */
  Val |= MVPP2_PORT_CTRL4_QSGMII_BYPASS_ACTIVE_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL4_REG, Val);

  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL2_REG);
  Val |= MVPP2_PORT_CTRL2_DIS_PADING_OFFS;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL2_REG, Val);

  /* Configure GIG MAC to SGMII mode */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL0_REG);
  Val &= ~MVPP2_PORT_CTRL0_PORTTYPE_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL0_REG, Val);

  /* Configure AN */
  an = MVPP2_PORT_AUTO_NEG_CFG_EN_PCS_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_AN_BYPASS_EN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_AN_SPEED_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_FC_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_FDX_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_CHOOSE_SAMPLE_TX_CONFIG_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_AUTO_NEG_CFG_REG, an);
}

VOID
MvGop110GmacQsgmiiCfg (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 Val, thresh, an;

  /*
   * Configure minimal level of the Tx FIFO before
   * the lower part starts to read a packet.
   */
  thresh = MV_SGMII_TX_FIFO_MIN_TH;
  Val = MvGop110GmacRead (Port, MVPP2_PORT_FIFO_CFG_1_REG);
  U32_SET_FIELD (
          Val,
          MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_MASK,
          (thresh << MVPP2_PORT_FIFO_CFG_1_TX_FIFO_MIN_TH_OFFS)
        );

  MvGop110GmacWrite (Port, MVPP2_PORT_FIFO_CFG_1_REG, Val);

  /* Disable bypass of sync module */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL4_REG);
  Val |= MVPP2_PORT_CTRL4_SYNC_BYPASS_MASK;

  /* Configure DP clock select according to mode */
  Val &= ~MVPP2_PORT_CTRL4_DP_CLK_SEL_MASK;
  Val &= ~MVPP2_PORT_CTRL4_EXT_PIN_GMII_SEL_MASK;

  /* Configure QSGMII bypass according to mode */
  Val &= ~MVPP2_PORT_CTRL4_QSGMII_BYPASS_ACTIVE_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL4_REG, Val);

  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL2_REG);
  Val &= ~MVPP2_PORT_CTRL2_DIS_PADING_OFFS;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL2_REG, Val);

  /* Configure GIG MAC to SGMII mode */
  Val = MvGop110GmacRead (Port, MVPP2_PORT_CTRL0_REG);
  Val &= ~MVPP2_PORT_CTRL0_PORTTYPE_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL0_REG, Val);

  /* Configure AN 0xB8EC */
  an = MVPP2_PORT_AUTO_NEG_CFG_EN_PCS_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_AN_BYPASS_EN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_AN_SPEED_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_FC_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_EN_FDX_AN_MASK |
       MVPP2_PORT_AUTO_NEG_CFG_CHOOSE_SAMPLE_TX_CONFIG_MASK;
  MvGop110GmacWrite (Port, MVPP2_PORT_AUTO_NEG_CFG_REG, an);
}

INT32
Mvpp2SmiPhyAddrCfg (
  IN PP2DXE_PORT *Port,
  IN INT32 PortId,
  IN INT32 Addr
  )
{
  Mvpp2SmiWrite (Port->Priv, MV_SMI_PHY_ADDRESS_REG(PortId), Addr);

  return 0;
}

/* Set the internal mux's to the required PCS */
EFI_STATUS
MvGopXpcsModeCfg (
  IN PP2DXE_PORT *Port,
  IN INT32 NumOfLanes
  )
{
  UINT8 LaneCoeff;

  switch (NumOfLanes) {
  case 1:
  case 2:
  case 4:
    LaneCoeff = NumOfLanes >> 1;
  default:
    return EFI_INVALID_PARAMETER;
  }

  /* Configure XG MAC mode */
  MmioAndThenOr32 (Port->Priv->XpcsBase + MVPP22_XPCS_GLOBAL_CFG_0_REG,
    ~(MVPP22_XPCS_PCSMODE_MASK | MVPP22_XPCS_LANEACTIVE_MASK),
    LaneCoeff << MVPP22_XPCS_LANEACTIVE_OFFS);

  return EFI_SUCCESS;
}

VOID
MvGopMpcsModeCfg (
  IN PP2DXE_PORT *Port
  )
{
  /* Configure MPCS40G COMMON CONTROL */
  MmioAnd32 (Port->Priv->MpcsBase + MVPP22_MPCS40G_COMMON_CONTROL,
    ~MVPP22_MPCS_FORWARD_ERROR_CORRECTION_MASK);

  /* Configure MPCS CLOCK RESET */
  MmioAndThenOr32 (Port->Priv->MpcsBase + MVPP22_MPCS_CLOCK_RESET,
    ~(MVPP22_MPCS_CLK_DIVISION_RATIO_MASK | MVPP22_MPCS_CLK_DIV_PHASE_SET_MASK),
    MVPP22_MPCS_CLK_DIVISION_RATIO_DEFAULT | MVPP22_MPCS_MAC_CLK_RESET_MASK |
    MVPP22_MPCS_RX_SD_CLK_RESET_MASK | MVPP22_MPCS_TX_SD_CLK_RESET_MASK);
}

/* Set the internal mux's to the required MAC in the GOP */
VOID
MvGopXlgMacModeCfg (
  IN PP2DXE_PORT *Port
  )
{
  /* Configure 10G MAC mode */
  MmioOr32 (Port->XlgBase + MV_XLG_PORT_MAC_CTRL0_REG, MV_XLG_MAC_CTRL0_RXFCEN_MASK);

  MmioAndThenOr32 (Port->XlgBase + MV_XLG_PORT_MAC_CTRL3_REG,
    ~MV_XLG_MAC_CTRL3_MACMODESELECT_MASK,
    MV_XLG_MAC_CTRL3_MACMODESELECT_10G);

  MmioAndThenOr32 (Port->XlgBase + MV_XLG_PORT_MAC_CTRL4_REG,
    ~(MV_XLG_MAC_CTRL4_MAC_MODE_DMA_1G_MASK | MV_XLG_MAC_CTRL4_EN_IDLE_CHECK_FOR_LINK_MASK),
    MV_XLG_MAC_CTRL4_FORWARD_PFC_EN_MASK | MV_XLG_MAC_CTRL4_FORWARD_802_3X_FC_EN_MASK);

  /* Configure frame size limit */
  MmioAndThenOr32 (Port->XlgBase + MV_XLG_PORT_MAC_CTRL1_REG,
    ~MV_XLG_MAC_CTRL1_FRAMESIZELIMIT_MASK,
    MV_XLG_MAC_CTRL1_FRAMESIZELIMIT_DEFAULT);

  /* Mask all port's external interrupts */
  MvGop110XlgPortLinkEventMask (Port);

  /* Unmask link change interrupt - enable automatic status update */
  MmioOr32 (Port->XlgBase + MV_XLG_INTERRUPT_MASK_REG,
    MV_XLG_INTERRUPT_LINK_CHANGE_MASK | MV_XLG_SUMMARY_INTERRUPT_MASK);
}

/* Set PCS to exit from reset */
VOID
MvGopXpcsUnreset (
  IN PP2DXE_PORT *Port
  )
{
  MmioOr32 (Port->Priv->XpcsBase + MVPP22_XPCS_GLOBAL_CFG_0_REG, MVPP22_XPCS_PCSRESET);
}

/* Set the MAC to exit from reset */
VOID
MvGopXlgMacUnreset (
  IN PP2DXE_PORT *Port
  )
{
  MmioOr32 (Port->XlgBase + MV_XLG_PORT_MAC_CTRL0_REG, MV_XLG_MAC_CTRL0_MACRESETN_MASK);
}

BOOLEAN
MvGop110XlgLinkStatusGet (
  IN PP2DXE_PORT *Port
  )
{
  return MmioRead32 (Port->XlgBase + MV_XLG_MAC_PORT_STATUS_REG) & MV_XLG_MAC_PORT_STATUS_LINKSTATUS_MASK;
}

BOOLEAN
MvGop110PortIsLinkUp (
  IN PP2DXE_PORT *Port
  )
{
  switch (Port->PhyInterface) {
  case MV_MODE_RGMII:
  case MV_MODE_SGMII:
  case MV_MODE_QSGMII:
    return MvGop110GmacLinkStatusGet (Port);
  case MV_MODE_SFI:
    return MvGop110XlgLinkStatusGet (Port);
  case MV_MODE_XAUI:
  case MV_MODE_RXAUI:
    return FALSE;
  default:
    return FALSE;
  }
}

/* Get MAC link status */
BOOLEAN
MvGop110GmacLinkStatusGet (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegAddr;
  UINT32 Val;

  RegAddr = MVPP2_PORT_STATUS0_REG;

  Val = MvGop110GmacRead (Port, RegAddr);

  return (Val & 1) ? TRUE : FALSE;
}

STATIC
VOID
MvGop110XlgPortEnable (
  IN PP2DXE_PORT *Port
  )
{
  /* Enable port and MIB counters update */
  MmioAndThenOr32 (Port->XlgBase + MV_XLG_PORT_MAC_CTRL0_REG,
    ~MV_XLG_MAC_CTRL0_MIBCNTDIS_MASK,
    MV_XLG_MAC_CTRL0_PORTEN_MASK);
}

STATIC
VOID
MvGop110XlgPortDisable (
  IN PP2DXE_PORT *Port
  )
{
  /* Mask all port's external interrupts */
  MvGop110XlgPortLinkEventMask (Port);

  MmioAnd32 (Port->XlgBase + MV_XLG_PORT_MAC_CTRL0_REG, ~MV_XLG_MAC_CTRL0_PORTEN_MASK);
}

VOID
MvGop110PortDisable (
  IN PP2DXE_PORT *Port
  )
{
  switch (Port->PhyInterface) {
  case MV_MODE_RGMII:
  case MV_MODE_SGMII:
  case MV_MODE_QSGMII:
    MvGop110GmacPortDisable (Port);
    break;
  case MV_MODE_XAUI:
  case MV_MODE_RXAUI:
  case MV_MODE_SFI:
    MvGop110XlgPortDisable (Port);
    break;
  default:
    return;
  }
}

VOID
MvGop110PortEnable (
  IN PP2DXE_PORT *Port
  )
{
  switch (Port->PhyInterface) {
  case MV_MODE_RGMII:
  case MV_MODE_SGMII:
  case MV_MODE_QSGMII:
    MvGop110GmacPortEnable (Port);
    break;
  case MV_MODE_XAUI:
  case MV_MODE_RXAUI:
  case MV_MODE_SFI:
    MvGop110XlgPortEnable (Port);
    break;
  default:
    return;
  }
}

/* Enable Port and MIB counters */
VOID
MvGop110GmacPortEnable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegVal;

  RegVal = MvGop110GmacRead (Port, MVPP2_PORT_CTRL0_REG);
  RegVal |= MVPP2_PORT_CTRL0_PORTEN_MASK;
  RegVal |= MVPP2_PORT_CTRL0_COUNT_EN_MASK;

  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL0_REG, RegVal);
}

/* Disable Port */
VOID
MvGop110GmacPortDisable (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegVal;

  /* Mask all Ports interrupts */
  MvGop110GmacPortLinkEventMask (Port);

  RegVal = MvGop110GmacRead (Port, MVPP2_PORT_CTRL0_REG);
  RegVal &= ~MVPP2_PORT_CTRL0_PORTEN_MASK;

  MvGop110GmacWrite (Port, MVPP2_PORT_CTRL0_REG, RegVal);
}

VOID
MvGop110GmacPortLinkEventMask (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegVal;

  RegVal = MvGop110GmacRead (Port, MV_GMAC_INTERRUPT_SUM_MASK_REG);
  RegVal &= ~MV_GMAC_INTERRUPT_SUM_CAUSE_LINK_CHANGE_MASK;
  MvGop110GmacWrite (Port, MV_GMAC_INTERRUPT_SUM_MASK_REG, RegVal);
}

VOID
MvGop110XlgPortLinkEventMask (
  IN PP2DXE_PORT *Port
  )
{
  MmioAnd32 (Port->XlgBase + MV_XLG_EXTERNAL_INTERRUPT_MASK_REG,
    ~MV_XLG_EXTERNAL_INTERRUPT_LINK_CHANGE_MASK);
}

INT32
MvGop110PortEventsMask (
  IN PP2DXE_PORT *Port
  )
{

  switch (Port->PhyInterface) {
  case MV_MODE_RGMII:
  case MV_MODE_SGMII:
  case MV_MODE_QSGMII:
    MvGop110GmacPortLinkEventMask (Port);
    break;
  case MV_MODE_XAUI:
  case MV_MODE_RXAUI:
  case MV_MODE_SFI:
    MvGop110XlgPortLinkEventMask (Port);
    break;
  default:
    return -1;
  }

  return 0;
}

/*
 * Sets "Force Link Pass" and clears "Do Not Force Link Fail" bits.
 * This function should only be called when the port is disabled.
 */
VOID
MvGop110GmacForceLinkUp (
  IN PP2DXE_PORT *Port
  )
{
  UINT32 RegVal;

  RegVal = MvGop110GmacRead (Port, MVPP2_PORT_AUTO_NEG_CFG_REG);

  RegVal |= MVPP2_PORT_AUTO_NEG_CFG_FORCE_LINK_UP_MASK;
  RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_FORCE_LINK_DOWN_MASK;

  MvGop110GmacWrite (Port, MVPP2_PORT_AUTO_NEG_CFG_REG, RegVal);
}

INT32
MvGop110FlCfg (
  IN PP2DXE_PORT *Port
  )
{
  switch (Port->PhyInterface) {
  case MV_MODE_RGMII:
  case MV_MODE_SGMII:
  case MV_MODE_QSGMII:
    /* Disable AN */
    MvGop110SpeedDuplexSet (Port, Port->Speed, MV_PORT_DUPLEX_FULL);
    break;
  case MV_MODE_XAUI:
  case MV_MODE_RXAUI:
  case MV_MODE_SFI:
    return 0;
  default:
    return -1;
  }

  return 0;
}

/* Set Port Speed and Duplex */
INT32
MvGop110SpeedDuplexSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Speed,
  IN enum MvPortDuplex Duplex
  )
{
  switch (Port->PhyInterface) {
  case MV_MODE_RGMII:
  case MV_MODE_SGMII:
  case MV_MODE_QSGMII:
    MvGop110GmacSpeedDuplexSet (Port, Speed, Duplex);
    break;
  case MV_MODE_XAUI:
  case MV_MODE_RXAUI:
  case MV_MODE_SFI:
    break;
  default:
    return -1;
  }

  return 0;
}

/*
 * Sets Port Speed to Auto Negotiation / 1000 / 100 / 10 Mbps.
 * Sets Port Duplex to Auto Negotiation / Full / Half Duplex.
 */
INT32
MvGop110GmacSpeedDuplexSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Speed,
  IN enum MvPortDuplex Duplex
  )
{
  UINT32 RegVal;

  RegVal = Mvpp2GmacRead (Port, MVPP2_PORT_AUTO_NEG_CFG_REG);

  switch (Speed) {
  case MV_PORT_SPEED_2500:
  case MV_PORT_SPEED_1000:
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_EN_AN_SPEED_MASK;
    RegVal |= MVPP2_PORT_AUTO_NEG_CFG_SET_GMII_SPEED_MASK;
    /* The 100/10 bit doesn't matter in this case */
    break;
  case MV_PORT_SPEED_100:
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_EN_AN_SPEED_MASK;
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_SET_GMII_SPEED_MASK;
    RegVal |= MVPP2_PORT_AUTO_NEG_CFG_SET_MII_SPEED_MASK;
    break;
  case MV_PORT_SPEED_10:
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_EN_AN_SPEED_MASK;
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_SET_GMII_SPEED_MASK;
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_SET_MII_SPEED_MASK;
    break;
  default:
    return MVPP2_EINVAL;
  }

  switch (Duplex) {
  case MV_PORT_DUPLEX_AN:
    RegVal  |= MVPP2_PORT_AUTO_NEG_CFG_EN_FDX_AN_MASK;
    /* The other bits don't matter in this case */
    break;
  case MV_PORT_DUPLEX_HALF:
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_EN_FDX_AN_MASK;
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_SET_FULL_DX_MASK;
    break;
  case MV_PORT_DUPLEX_FULL:
    RegVal &= ~MVPP2_PORT_AUTO_NEG_CFG_EN_FDX_AN_MASK;
    RegVal |= MVPP2_PORT_AUTO_NEG_CFG_SET_FULL_DX_MASK;
    break;
  default:
    return MVPP2_EINVAL;
  }

  Mvpp2GmacWrite (Port, MVPP2_PORT_AUTO_NEG_CFG_REG, RegVal);

  return 0;
}

VOID
Mvpp2AxiConfig (
  IN MVPP2_SHARED *Priv
  )
{
  /* Config AXI Read&Write Normal and Soop mode  */
  Mvpp2Write (Priv, MVPP22_AXI_BM_WR_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
  Mvpp2Write (Priv, MVPP22_AXI_BM_RD_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
  Mvpp2Write (Priv, MVPP22_AXI_AGGRQ_DESCR_RD_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
  Mvpp2Write (Priv, MVPP22_AXI_TXQ_DESCR_WR_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
  Mvpp2Write (Priv, MVPP22_AXI_TXQ_DESCR_RD_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
  Mvpp2Write (Priv, MVPP22_AXI_RXQ_DESCR_WR_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
  Mvpp2Write (Priv, MVPP22_AXI_RX_DATA_WR_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
  Mvpp2Write (Priv, MVPP22_AXI_TX_DATA_RD_ATTR_REG, MVPP22_AXI_ATTR_SNOOP_CNTRL_BIT);
}

/* Cleanup Tx Ports */
VOID
Mvpp2TxpClean (
  IN PP2DXE_PORT *Port,
  IN INT32 Txp,
  IN MVPP2_TX_QUEUE *Txq
  )
{
  INT32 Delay, Pending;
  UINT32 RegVal;

  Mvpp2Write (Port->Priv, MVPP2_TXQ_NUM_REG, Txq->Id);
  RegVal = Mvpp2Read (Port->Priv, MVPP2_TXQ_PREF_BUF_REG);
  RegVal |= MVPP2_TXQ_DRAIN_EN_MASK;
  Mvpp2Write (Port->Priv, MVPP2_TXQ_PREF_BUF_REG, RegVal);

  /*
   * The Queue has been stopped so wait for all packets
   * to be transmitted.
   */
  Delay = 0;
  do {
    if (Delay >= MVPP2_TX_PENDING_TIMEOUT_MSEC) {
      Mvpp2Printf ("Port %d: cleaning Queue %d timed out\n", Port->Id, Txq->LogId);
      break;
    }
    Mvpp2Mdelay (1);
    Delay++;

    Pending = Mvpp2TxqPendDescNumGet (Port, Txq);
  } while (Pending);

  RegVal &= ~MVPP2_TXQ_DRAIN_EN_MASK;
  Mvpp2Write (Port->Priv, MVPP2_TXQ_PREF_BUF_REG, RegVal);
}

/* Cleanup all Tx Queues */
VOID
Mvpp2CleanupTxqs (
  IN PP2DXE_PORT *Port
  )
{
  MVPP2_TX_QUEUE *Txq;
  INT32 Txp, Queue;
  UINT32 RegVal;

  RegVal = Mvpp2Read (Port->Priv, MVPP2_TX_PORT_FLUSH_REG);

  /* Reset Tx Ports and delete Tx Queues */
  for (Txp = 0; Txp < Port->TxpNum; Txp++) {
    RegVal |= MVPP2_TX_PORT_FLUSH_MASK (Port->Id);
    Mvpp2Write (Port->Priv, MVPP2_TX_PORT_FLUSH_REG, RegVal);

    for (Queue = 0; Queue < TxqNumber; Queue++) {
      Txq = &Port->Txqs[Txp * TxqNumber + Queue];
      Mvpp2TxpClean (Port, Txp, Txq);
      Mvpp2TxqHwDeinit (Port, Txq);
    }

    RegVal &= ~MVPP2_TX_PORT_FLUSH_MASK (Port->Id);
    Mvpp2Write (Port->Priv, MVPP2_TX_PORT_FLUSH_REG, RegVal);
  }
}

/* Cleanup all Rx Queues */
VOID
Mvpp2CleanupRxqs (
  IN PP2DXE_PORT *Port
  )
{
  INT32 Queue;

  for (Queue = 0; Queue < RxqNumber; Queue++) {
    Mvpp2RxqHwDeinit (Port, &Port->Rxqs[Queue]);
  }
}
