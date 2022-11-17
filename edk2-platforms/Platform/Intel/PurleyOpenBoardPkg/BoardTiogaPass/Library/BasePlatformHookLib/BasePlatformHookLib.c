/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2021, American Megatrends International LLC.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/



#include <Base.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/DebugLib.h>

#define R_ICH_IOPORT_PCI_INDEX                      0xCF8
#define R_ICH_IOPORT_PCI_DATA                       0xCFC
#define R_ICH_LPC_IO_DEC                             0x80

#define PCI_DEVICE_NUMBER_ICH_LPC                      31
#define PCI_FUNCTION_NUMBER_ICH_LPC                    0

#define PCI_CF8_ADDR(Bus, Dev, Func, Off) \
          (((Off) & 0xFF) | (((Func) & 0x07) << 8) | (((Dev) & 0x1F) << 11) | (((Bus) & 0xFF) << 16) | (1 << 31))
#define ICH_LPC_CF8_ADDR(Offset)    PCI_CF8_ADDR(0, PCI_DEVICE_NUMBER_ICH_LPC, PCI_FUNCTION_NUMBER_ICH_LPC, Offset)

#include "SioRegs.h"

//
// ---------------------------------------------
// Additional SIO Regs for Tioga Pass
// ---------------------------------------------
//
#define  SCU_BASE         0x1E6E2000

#include <Platform.h>
#include <PchAccess.h>

//
// ---------------------------------------------
// UART Register Offsets
// ---------------------------------------------
//
#define BAUD_LOW_OFFSET   0x00
#define BAUD_HIGH_OFFSET  0x01
#define IER_OFFSET        0x01
#define LCR_SHADOW_OFFSET 0x01
#define FCR_SHADOW_OFFSET 0x02
#define IR_CONTROL_OFFSET 0x02
#define FCR_OFFSET        0x02
#define EIR_OFFSET        0x02
#define BSR_OFFSET        0x03
#define LCR_OFFSET        0x03
#define MCR_OFFSET        0x04
#define LSR_OFFSET        0x05
#define MSR_OFFSET        0x06

//
// ---------------------------------------------
// UART Register Bit Defines
// ---------------------------------------------
//
#define LSR_TXRDY 0x20
#define LSR_RXDA  0x01
#define DLAB      0x01

#define UART_DATA    8
#define UART_STOP    1
#define UART_PARITY  0
#define UART_BREAK_SET  0

UINT16 gComBase  = 0x3f8;
UINTN  gBps      = 115200;
UINT8  gData     = 8;
UINT8  gStop     = 1;
UINT8  gParity   = 0;
UINT8  gBreakSet = 0;

/**

    Write AHB register.

    @param RegIndex: register index.
    @param Value: Value to write

    @retval None.

**/
VOID 
WriteAHBDword(
  UINT32 RegIndex,
  UINT32 Value
)
{
  UINT8   bValue;

  IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);  
  IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);  

  IoWrite8 (SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT, SIO_SMI);  
  IoWrite8 (0xED, 0);//short delay.  

  IoWrite8 (SIO_INDEX_PORT, 0x30); 
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT, 1);  
  IoWrite8 (0xED, 0);//short delay. 

  IoWrite8 (SIO_INDEX_PORT, 0xf8);   
  bValue = IoRead8(SIO_DATA_PORT);
  bValue &= 0xfc;
  bValue |= 2;  // 4 byte window.  
  IoWrite8 (SIO_DATA_PORT, bValue);    
  IoWrite8 (0xED, 0);//short delay.   

  IoWrite8 (SIO_INDEX_PORT, 0xf0); 
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex >> 24)& 0xff));   

  IoWrite8 (SIO_INDEX_PORT, 0xf1); 
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex >> 16)& 0xff));    

  IoWrite8 (SIO_INDEX_PORT, 0xf2); 
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex >> 8) & 0xff));     

  IoWrite8 (SIO_INDEX_PORT, 0xf3); 
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex )& 0xff)); 

  IoWrite8 (SIO_INDEX_PORT, 0xf4);
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((Value >> 24)& 0xff));   

  IoWrite8 (SIO_INDEX_PORT, 0xf5); 
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((Value >> 16)& 0xff));    

  IoWrite8 (SIO_INDEX_PORT, 0xf6); 
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((Value >> 8) & 0xff));     

  IoWrite8 (SIO_INDEX_PORT, 0xf7); 
  IoWrite8 (0xED, 0);//short delay.  
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((Value )& 0xff)); 

  // trigger write
  IoWrite8 (SIO_INDEX_PORT, 0xfe); 
  IoWrite8 (SIO_DATA_PORT, 0xcf); 

  IoWrite8 (SIO_INDEX_PORT, SIO_LOCK);  
}

/**

    Read AHB register.

    @param RegIndex: register index.

    @retval value of register.

**/
UINT32
ReadAHBDword(
  UINT32  RegIndex
){
  UINT8    bValue;
  UINT32   rdValue = 0;


  IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);  
  IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);   

  IoWrite8 (SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT, SIO_SMI);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (SIO_INDEX_PORT, 0x30);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT, 1);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (SIO_INDEX_PORT, 0xf8);
  bValue = IoRead8(SIO_DATA_PORT);
  bValue &= 0xfc;
  bValue |= 2;  // 4 byte window.
  IoWrite8 (SIO_DATA_PORT, bValue);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (SIO_INDEX_PORT, 0xf0);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex >> 24)& 0xff));

  IoWrite8 (SIO_INDEX_PORT, 0xf1);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex >> 16)& 0xff));

  IoWrite8 (SIO_INDEX_PORT, 0xf2);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex >> 8) & 0xff));

  IoWrite8 (SIO_INDEX_PORT, 0xf3);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (SIO_DATA_PORT,  (UINT8)((RegIndex )& 0xff));

  // trigger read
  IoWrite8 (SIO_INDEX_PORT, 0xfe);
  IoRead8 (SIO_DATA_PORT);


  IoWrite8 (SIO_INDEX_PORT, 0xf4);
  rdValue += IoRead8 (SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (SIO_INDEX_PORT, 0xf5);
  rdValue += IoRead8 (SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (SIO_INDEX_PORT, 0xf6);
  rdValue += IoRead8 (SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (SIO_INDEX_PORT, 0xf7);
  rdValue += IoRead8 (SIO_DATA_PORT);

  IoWrite8 (SIO_INDEX_PORT, SIO_LOCK);  
  return rdValue;

}

/**

    Initialize the AST2500 SIO.

    @param None

    @retval None

**/
VOID
InitializeSio (
  VOID
  )
{
    UINT32   Decode;
    UINT32   Enable;
    UINT32   Value;
    UINT32   Address;

     //
     // Enable LPC decode
     // Set COMA/COMB base
     //

    Decode =  ((V_PCH_LPC_IOD_COMA_3F8 << N_PCH_LPC_IOD_COMA) | (V_PCH_LPC_IOD_COMB_2F8 << N_PCH_LPC_IOD_COMB));
    Enable =  ( B_PCH_LPC_IOE_ME2 | B_PCH_LPC_IOE_SE | B_PCH_LPC_IOE_ME1 \
              | B_PCH_LPC_IOE_KE | B_PCH_LPC_IOE_CBE | B_PCH_LPC_IOE_CAE);
    IoWrite32 (R_ICH_IOPORT_PCI_INDEX, (UINT32) (ICH_LPC_CF8_ADDR (R_ICH_LPC_IO_DEC)));

    IoWrite32 (R_ICH_IOPORT_PCI_DATA, Decode | (Enable << 16));

    MmioWrite16 (PCH_PCR_ADDRESS(PID_DMI, R_PCH_PCR_DMI_LPCIOD), (UINT16)Decode);
    MmioWrite16 (PCH_PCR_ADDRESS(PID_DMI, R_PCH_PCR_DMI_LPCIOE), (UINT16)Enable);



    IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);  
    IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);  

    //
    // Initialize COM1
    //
    IoWrite8 (SIO_INDEX_PORT, REG_LOGICAL_DEVICE);     
    IoWrite8 (SIO_DATA_PORT,  SIO_UART1);         

    IoWrite8 (SIO_INDEX_PORT, ACTIVATE);     
    IoWrite8 (SIO_DATA_PORT,  0);   

    IoWrite8 (SIO_INDEX_PORT, 0x70);     
    IoWrite8 (SIO_DATA_PORT,  0x04);   

    //
    // Set Base Address to gComBase
    //
    IoWrite8 (SIO_INDEX_PORT, BASE_ADDRESS_LOW0);
    IoWrite8 (SIO_DATA_PORT, (UINT8) (gComBase & 0xFF));
    IoWrite8 (SIO_INDEX_PORT, BASE_ADDRESS_HIGH0);
    IoWrite8 (SIO_DATA_PORT, (UINT8)((gComBase >> 8) & 0xff));

    //
    // Activate COM1
    //
    IoWrite8 (SIO_INDEX_PORT, ACTIVATE);     
    IoWrite8 (SIO_DATA_PORT,  1);        
    
    IoWrite8 (SIO_INDEX_PORT, SIO_LOCK); 

    // Initialize SCU part.
    Address = (UINT32)(SCU_BASE | 0x00);
    Value = ReadAHBDword(Address);
    Value &= 0x00000000;
    Value |= 0x1688A8A8; // Unlock SCU Registers
    WriteAHBDword(Address,Value);
    
    Address = (UINT32)(SCU_BASE | 0x84);
    Value = ReadAHBDword(Address);
    Value &= 0xFF3FFFFF;
    Value |= 0x00C00000; // SCU84[23:22]="11b" Enable UART1 or video VPB Multi-Function pins
    WriteAHBDword(Address,Value);

    Address = (UINT32)(SCU_BASE | 0x90);
    Value = ReadAHBDword(Address);
    Value &= 0xFFFFFFDF;
    Value |= 0x00000000; // SCU90[5]="0b" Disable digital video Multi-Function pins
    WriteAHBDword(Address,Value);

    Address = (UINT32)(SCU_BASE | 0x94);
    Value = ReadAHBDword(Address);
    Value &= 0xFFFFFFFC;
    Value |= 0x00000000; // SCU94[1:0]="00b" Disable digital video Multi-Function pins
    WriteAHBDword(Address,Value);

    Address = (UINT32)(SCU_BASE | 0x00);
    Value = ReadAHBDword(Address);
    Value &= 0x00000000;
    Value |= 0x00000000; // Lock SCU Registers
    WriteAHBDword(Address,Value);
}

/**
  Performs platform specific initialization required for the CPU to access
  the hardware associated with a SerialPortLib instance.  This function does
  not initialize the serial port hardware itself.  Instead, it initializes
  hardware devices that are required for the CPU to access the serial port
  hardware.  This function may be called more than once.

  @retval RETURN_SUCCESS       The platform specific initialization succeeded.
  @retval RETURN_DEVICE_ERROR  The platform specific initialization could not be completed.

**/
RETURN_STATUS
EFIAPI
PlatformHookSerialPortInitialize (
  VOID
  )
{
  UINTN Divisor;
  UINT8 OutputData;
  UINT8 Data;

  InitializeSio();
  //
  // Some init is done by the platform status code initialization.
  //
  //
  // Map 5..8 to 0..3
  //
  Data = (UINT8) (gData - (UINT8) 5);

  //
  // Calculate divisor for baud generator
  //
  Divisor = 115200 / gBps;

  //
  // Set communications format
  //
  OutputData = (UINT8) ((DLAB << 7) | ((gBreakSet << 6) | ((gParity << 3) | ((gStop << 2) | Data))));
  IoWrite8 (gComBase + LCR_OFFSET, OutputData);

  //
  // Configure baud rate
  //
  IoWrite8 (gComBase + BAUD_HIGH_OFFSET, (UINT8) (Divisor >> 8));
  IoWrite8 (gComBase + BAUD_LOW_OFFSET, (UINT8) (Divisor & 0xff));

  //
  // Switch back to bank 0
  //
  OutputData = (UINT8) ((~DLAB << 7) | ((gBreakSet << 6) | ((gParity << 3) | ((gStop << 2) | Data))));
  IoWrite8 (gComBase + LCR_OFFSET, OutputData);

  return RETURN_SUCCESS;
}

