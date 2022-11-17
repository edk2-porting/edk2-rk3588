/** @file
  Generic Definations for Server Management Header File.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SERVER_MANAGEMENT_H_
#define _SERVER_MANAGEMENT_H_

//
// Defines
//
#define PRIM_IPMB_CHANNEL       0x0
#define BMC_LUN                 0x0
#define PRESENT_INTERFACE       0xE
#define SYSTEM_INTERFACE        0xF
#define COMPLETE_SEL_RECORD     0xFF
#define IPMI_UNSPECIFIED_ERROR  0xFF

//
// Net Function Defines.
//
#define SM_CHASSIS      0x0
#define SM_BRIDGE       0x2
#define SM_SENSOR_EVENT 0x4
#define SM_APPLICATION  0x6
#define SM_FIRMWARE     0x8
#define SM_STORAGE      0xA
#define SM_TRANSPORT    0xC
#define SM_GROUP_EXT    0x2C
#define SM_OEM_GROUP    0x2E
#define SM_INTEL_OEM    0x30
#define SM_SOL_OEM      0x34

//
// IPMI Command Definations.
//
#define IPMI_GET_DEVICE_ID          1
#define IPMI_COLD_RESET             2
#define IPMI_WARM_RESET             3
#define IPMI_GET_SELF_TEST_RESULTS  4
#define IPMI_MFG_MODE_ON            5
#define IPMI_SET_ACPI_POWER_STATE   6
#define IPMI_GET_ACPI_POWER_STATE   7
#define IPMI_GET_DEVICE_GUID        8

#define IPMI_GET_MESSAGE_FLAGS      0x31
#define IPMI_GET_MESSAGE            0x33
#define IPMI_SEND_MESSAGE           0x34

#define RESERVE_SEL_ENTRY               0x42
#define ADD_SEL_ENTRY                   0x44
#define GET_SEL_ENTRY                   0x43
#define DELETE_SEL_ENTRY                0x46
#define CLEAR_SEL_ENTRY                 0x47
#define SET_BMC_GLOBALS                 0x2E
#define GET_BMC_GLOBALS                 0x2F
#define SET_SEL_TIME                    0x49

#define GET_SELF_TEST_RESULTS           0x4

#define NMI_ENABLE_DISABLE              0xF7

//
// Controller Attributes
//
#define IPMI_SENSOR_DEVICE_SUPPORT    0x1
#define IPMB_SDR_REPOSITORY_SUPPORT   0x2
#define IPMI_SEL_DEVICE_SUPPORT       0x4
#define IPMI_FRU_INVENTORY_SUPPORT    0x8
#define IPMB_EVENT_RECEIVER_SUPPORT   0x10
#define IPMB_EVENT_GENERATOR_SUPPORT  0x20
#define ICMB_BRIDGE_SUPPORT           0x40
#define ICMB_CHASSIS_DEVICE_SUPPORT   0x80
#define SM_TCP_SUPPORT                0x100
#define SM_UDP_SUPPORT                0x200
#define SM_IPV4_SUPPORT               0x400
#define SM_IPV6_SUPPORT               0x800
#define SM_RS232_SUPPORT              0x1000

//
// Sensor Type Definations
//
typedef enum {
  SensorReserved,
  SensorTemperature,
  SensorVoltage,
  SensorCurrent,
  SensorFan,
  SensorPhysicalSecurity,
  SensorPlatformSecurityViolationAttempt,
  SensorProcessor,
  SensorPowerSupply,
  SensorPowerUnit,
  SensorCoolingDevice,
  SensorOtherUnits,
  SensorMemory,
  SensorDriveSlot,
  SensorPOSTMemoryResize,
  SensorSystemFirmwareProgress,
  SensorEventLoggingDisabled,
  SensorWatchdog1,
  SensorSystemEvent,
  SensorCriticalInterrupt,
  SensorButton,
  SensorModuleBoard,
  SensorMicrocontrollerCoprocessor,
  SensorAddinCard,
  SensorChassis,
  SensorChipSet,
  SensorOtherFRU,
  SensorCableInterconnect,
  SensorTerminator,
  SensorSystemBootInitiated,
  SensorBootError,
  SensorOSBoot,
  SensorOSCriticalStop,
  SensorSlotConnector,
  SensorSystemACPIPowerState,
  SensorWatchdog2,
  SensorPlatformAlert,
  SensorEntityPresence,
  SensorMonitorASIC,
  SensorLAN,
  SensorManagementSubsystemHealth
} SM_SENSOR_TYPE;

//
// Sensor Event Type Code
//
#define SENSOR_THRESHOLD_EVENT_TYPE 1
#define SENSOR_SPECIFIC_EVENT_TYPE  0x6F

//
// THRESHOLD SENSOR TYPE BIT MASK
//
#define LOWER_NON_CRITICAL_GOING_LOW  0x1
#define LOWER_NON_CRITICAL_GOING_HI   0x2
#define LOWER_CRITICAL_GOING_LOW      0x4
#define LOWER_CRITICAL_GOING_HI       0x8
#define LOWER_NON_RECOVER_GOING_LOW   0x10
#define LOWER_NON_RECOVER_GOING_HI    0x20
#define UPPER_NON_CRITICAL_GOING_LOW  0x40
#define UPPER_NON_CRITICAL_GOING_HI   0x80
#define UPPER_CRITICAL_GOING_LOW      0x100
#define UPPER_CRITICAL_GOING_HI       0x200
#define UPPER_NON_RECOVER_GOING_LOW   0x400
#define UPPER_NON_RECOVER_GOING_HI    0x800

//
// Server Management COM Addressing types
//
typedef enum {
  SmReserved,
  SmIpmb,
  SmIcmb1_0,
  SmIcmb0_9,
  Sm802_3_Lan,
  SmRs_232,
  SmOtherLan,
  SmPciSmBus,
  SmSmBus1_0,
  SmSmBus2_0,
  SmUsb1_x,
  SmUsb2_x,
  SmBmc
} SM_CHANNEL_MEDIA_TYPE;

typedef enum {
  SmTcp,
  SmUdp,
  SmIcmp,
  SmIpmi
} SM_PROTOCOL_TYPE;

typedef enum {
  SmMessage,
  SmRawData
} SM_DATA_TYPE;

typedef struct {
  BOOLEAN IpAddressType;
  UINT16  IpPort;
  UINT8   IpAddress[16];
} SM_IP_ADDRESS;

typedef struct {
  UINT8 SlaveAddress;
  UINT8 LunAddress;
  UINT8 NetFunction;
  UINT8 ChannelAddress;
} SM_IPMI_ADDRESS;

typedef struct {
  UINT8 SerialPortNumber;
} SM_SERIAL_ADDRESS;

typedef union {
  SM_IP_ADDRESS     IpAddress;
  SM_IPMI_ADDRESS   BmcAddress;
  SM_SERIAL_ADDRESS SerialAddress;
} SM_COM_ADDRESS_TYPE;

typedef struct {
  SM_CHANNEL_MEDIA_TYPE ChannelType;
  SM_COM_ADDRESS_TYPE   Address;
} SM_COM_ADDRESS;

#pragma pack(1)
//
// Sensor Reading Data
//
typedef enum {
  DataLinear,                                   // Linear
  DataNaturalLog,                               // Ln(x)
  DataLog10,                                    // Log10(x)
  DataLog2,                                     // Log2(x)
  Datae,                                        // e
  DataExp10,                                    // Exp 10
  DataExp2,                                     // Exp 2
  DataInverse,                                  // 1/x
  DataSqr,                                      // Sqr
  DataCube,                                     // Cube
  DataSqrt,                                     // Square Root
  DataCubeInverse                               // Cube-1 (x)
} LINERIZATION_TYPE;

typedef union {
  UINT8   SensorUint8Data[2];
  UINT16  SensorUint16Data;
} SENSOR_SPLIT_DATA;

typedef struct {
  LINERIZATION_TYPE Linearization;              // L
  UINT8             Tolerance;                  // Tolerance
  UINT8             AdditiveOffsetExp;          // k1
  UINT8             AccuracyExp;                // Accuracy Exponential
  UINT8             ResultExponent;             // k2
  SENSOR_SPLIT_DATA IntegerConstantMultiplier;  // M
  SENSOR_SPLIT_DATA AdditiveOffset;             // B
  SENSOR_SPLIT_DATA Accuracy;                   // Accuracy
} SENSOR_CONVERSION_DATA;

//
// Server Management Controller Information
//
typedef struct {
  UINT8   CompletionCode;
  UINT8   DeviceId;
  UINT8   DeviceRevision : 4;
  UINT8   Reserved : 3;
  UINT8   DeviceSdr : 1;
  UINT8   MajorFirmwareRev : 7;
  UINT8   UpdateMode : 1;
  UINT8   MinorFirmwareRev;
  UINT8   SpecificationVersion;
  UINT8   SensorDeviceSupport : 1;
  UINT8   SdrRepositorySupport : 1;
  UINT8   SelDeviceSupport : 1;
  UINT8   FruInventorySupport : 1;
  UINT8   IPMBMessageReceiver : 1;
  UINT8   IPMBMessageGenerator : 1;
  UINT8   BridgeSupport : 1;
  UINT8   ChassisSupport : 1;
  UINT8   ManufacturerId[3];
  UINT16  ProductId;
  UINT32  AuxFirmwareRevInfo;
} SM_CTRL_INFO;

typedef struct {
  UINT8 Reserved1 : 1;
  UINT8 ControllerSlaveAddress : 7;
  UINT8 FruDeviceId;
  UINT8 BusId : 3;
  UINT8 Lun : 2;
  UINT8 Reserved : 2;
  UINT8 LogicalFruDevice : 1;
  UINT8 Reserved3 : 4;
  UINT8 ChannelNumber : 4;
} FRU_DATA_INFO;
#pragma pack()

typedef enum {
  Unicode,
  BcdPlus,
  Ascii6BitPacked,
  AsciiLatin1
} SENSOR_ID_STRING_TYPE;

//
// SENSOR Structures
//
typedef struct {
  BOOLEAN                     Valid;              // Data is Valid
  SENSOR_CONVERSION_DATA      ConversionParam;    // Conversion Parameters
  UINT8                       UpperNonRec;        // Upper Non Recoverable
  UINT8                       UpperCritical;      // Upper Critical
  UINT8                       UpperNonCritical;   // Upper Non Critical
  UINT8                       LowerNonRec;        // Lower Non Recoverable
  UINT8                       LowerCritical;      // Lower Critical
  UINT8                       LowerNonCritical;   // Lower Non Critical
} SENSOR_THRESHOLD_STRUCT;

typedef struct {
  BOOLEAN                     Valid;              // Structure Valid
  SENSOR_CONVERSION_DATA      ConversionParam;    // Conversion Parameters
  SENSOR_ID_STRING_TYPE       SensorIdStringType; // Sensor ID String type
  UINT8                       NominalReading;     // Nominal Reading of the Sensor
  UINT8                       SensorId[16];       // Sensor Description
} SENSOR_READING_STRUCT;

//
//  IPMI HOB
//
typedef struct {
  UINT16  IoBasePort;
} IPMI_HOB_DATA;

//
// COM Layer Callback
//
typedef
EFI_STATUS
(EFIAPI *SM_CALLBACK_PROC) (
  OUT  EFI_STATUS                          Status,
  IN  VOID                                 *UserContext
  );

typedef struct {
  SM_CALLBACK_PROC      SmCallback;
  VOID                  *UserContext;
} SM_CALLBACK;

#endif  // _SERVER_MANAGEMENT_H_

