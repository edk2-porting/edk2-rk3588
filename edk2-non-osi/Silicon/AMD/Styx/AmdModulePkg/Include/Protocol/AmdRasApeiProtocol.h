/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD RAS APEI Protocol
 *
 * AMD Ras Interface Protocol GUID initialization
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      FDK
 * @e sub-project:  UEFI
 * @e \$Revision: 281924 $   @e \$Date: 2014-01-02 13:57:19 -0600 (Thu, 02 Jan 2014) $
 */
/*****************************************************************************
 *
 *  Copyright 2013 - 2016 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *
 *  This program and the accompanying materials are licensed and made available
 *  under the terms and conditions of the BSD License which accompanies this
 *  distribution. The full text of the license may be found at
 *  http://opensource.org/licenses/bsd-license.php
 *
 *  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
 *  IMPLIED.
 *
 ***************************************************************************/

#ifndef _AMD_RAS_APEI_PROTOCOL_H_
#define _AMD_RAS_APEI_PROTOCOL_H_

#include "AmdApei.h"

//
// GUID definition
//
#define AMD_RAS_APEI_PROTOCOL_GUID \
  { 0xe9dbcc60, 0x8f93, 0x47ed, 0x84, 0x78, 0x46, 0x78, 0xf1, 0x9f, 0x73, 0x4a }
// {E9DBCC60-8F93-47ed-8478-4678F19F734A}

extern EFI_GUID gAmdRasApeiProtocolGuid;

// current PPI revision
#define AMD_RAS_APEI_REV  0x01

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
typedef struct _AMD_RAS_APEI_PROTOCOL AMD_RAS_APEI_PROTOCOL;

/// APEI Interface data pointer
typedef
struct _AMD_APEI_INTERFACE {
  APEI_DRIVER_PRIVATE_DATA  *ApeiPrivData;
} AMD_APEI_INTERFACE;


/// APEI add Boot error record
typedef
EFI_STATUS
(EFIAPI *AMD_ADD_BOOT_ERROR_RECORD_ENTRY) (
  IN    UINT8   *ErrorRecord,
  IN    UINT32  RecordLen,
  IN    UINT8   ErrorType,
  IN    UINT8   SeverityType
);

/// APEI add HEST error source
typedef
EFI_STATUS
(EFIAPI *ADD_HEST_ERROR_SOURCE_ENTRY) (
  IN    UINT8   *pErrorRecord,
  IN    UINT32  RecordLen
);


/// RAS APEI Protocol Structure
typedef struct _AMD_RAS_APEI_PROTOCOL {
  AMD_APEI_INTERFACE               *AmdApeiInterface;        /// APEI Interface
  AMD_ADD_BOOT_ERROR_RECORD_ENTRY  AddBootErrorRecordEntry;  /// Boot error record to be added
  ADD_HEST_ERROR_SOURCE_ENTRY      AddHestErrorSourceEntry;  /// HEST error source to be added
} AMD_RAS_APEI_PROTOCOL;


#endif //_AMD_RAS_APEI_PROTOCOL_H_
