/** @file
  Data format for Universal Data Structure

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef   __SOCKET_POWERMANAGEMENT_CONFIGURATION_DATA_H__
#define   __SOCKET_POWERMANAGEMENT_CONFIGURATION_DATA_H__


#include <UncoreCommonIncludes.h>

extern EFI_GUID gEfiSocketPowermanagementVarGuid;
#define SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME L"SocketPowerManagementConfig"

#define NUM_CST_LAT_MSR      3

#pragma pack(1)

typedef struct {
  UINT8   WFRWAEnable;
  UINT8   UncoreFreqScaling;
  UINT8   InputUncoreFreq;
  UINT8   ProcessorEistSupport;             // Gates EIST based on CPUID ECX[7]
  UINT8   ProcessorEistEnable;              // EIST or GV3 setup option
  UINT8   GpssTimer;                        // Global P-state Selection timer setup option

  // Intel Speed Select (ISS)
  UINT8   IssTdpLevel;
  UINT8   DynamicIss;

  // Config TDP
  UINT8   ConfigTdpLevel;
  UINT8   ConfigTdpLock;

  // Individual controls for ACPI sleep states
  // ** These can be overridden by AcpiSleepState because these knobs are not available to CRB **
  //
  UINT8   AcpiS3Enable;
  UINT8   AcpiS4Enable;

  //
  //HWPM starts
  //
  UINT8   ProcessorHWPMEnable;
  UINT8   ProcessorHWPMInterrupt;
  UINT8   ProcessorEPPEnable;
  UINT8   ProcessorEppProfile;
  UINT8   ProcessorAPSrocketing;
  UINT8   ProcessorScalability;
  UINT8   ProcessorRaplPrioritization;
  UINT8   ProcessorOutofBandAlternateEPB;
  //
  //HWPM ends
  //
  UINT8   ProcessorEistPsdFunc;             // EIST/PSD Function select option
  UINT8   BootPState;                       // Boot Performance Mode

  //
  // Active PBF (Prioritized Base Frequency)
  //
  UINT8   ProcessorActivePbf;
  //
  // Configure PBF High Priority Cores
  //
  UINT8   ProcessorConfigurePbf;

  //
  // Processor Control
  //
  UINT8   TurboMode;
  UINT8   EnableXe;

  //OverClocking
  UINT8   HideOverclockingLock;
  UINT8   OverclockingLock;
  UINT8   AvxSupport;

  UINT8   AvxLicensePreGrant;
  UINT8   AvxIccpLevel;

  UINT8   TurboRatioLimitRatio[8];
  UINT8   TurboRatioLimitCores[8];

  UINT8   C2C3TT;
  UINT8   DynamicL1;                        // Enabling Dynamic L1
  UINT8   ProcessorCcxEnable;               // Enabling CPU C states of processor
  UINT8   PackageCState;                    // Package C-State Limit
  UINT8   EnableLowerLatencyMode;           // Enable Lower Latency Mode for register accesses
  UINT8   C3Enable;                         // Enable/Disable NHM C3(ACPI C2) report to OS
  UINT8   C6Enable;                         // Enable/Disable NHM C6(ACPI C3) report to OS
  UINT8   ProcessorC1eEnable;               // Enabling C1E state of processor
  UINT8   OSCx;                             // ACPI C States
  UINT8   C1AutoDemotion;                   // Enabling C1 auto demotion
  UINT8   C1AutoUnDemotion;                 // Enabling C1 auto un-demotion
  UINT8   MonitorMWait;                     // Enabling IO MWAIT

  UINT8   CStateLatencyCtrlValid[NUM_CST_LAT_MSR];       // C_STATE_LATENCY_CONTROL_x.Valid
  UINT8   CStateLatencyCtrlMultiplier[NUM_CST_LAT_MSR];  // C_STATE_LATENCY_CONTROL_x.Multiplier
  UINT16  CStateLatencyCtrlValue[NUM_CST_LAT_MSR];       // C_STATE_LATENCY_CONTROL_x.Value

  UINT8   TStateEnable;                     // T states enable?
  UINT8   OnDieThermalThrottling;           // Throtte ratio
  UINT8   ProchotLock;
  UINT8   EnableProcHot;
  UINT8   EnableThermalMonitor;
  UINT8   ThermalMonitorStatusFilter;
  UINT8   ThermalMonitorStatusFilterTimeWindow;
  UINT8   ProchotResponse;
  UINT8   EETurboDisable;
  UINT8   SapmctlValCtl;
  UINT8   PwrPerfTuning;
  UINT8   AltEngPerfBIAS;
  UINT8   PwrPerfSwitch;
  UINT8   WorkLdConfig;
  UINT16  EngAvgTimeWdw1;

  UINT8   ProchotResponseRatio;
  UINT8   TCCActivationOffset;

  UINT8   P0TtlTimeLow1;
  UINT8   P0TtlTimeHigh1;

  UINT8   PkgCLatNeg;
  UINT8   LTRSwInput;
  UINT8   SAPMControl;
  UINT8   CurrentConfig;
  UINT8   PriPlnCurCfgValCtl;
  UINT8   Psi3Code;
  UINT16  CurrentLimit;

  UINT8   Psi3Thshld;
  UINT8   Psi2Code;
  UINT8   Psi2Thshld;
  UINT8   Psi1Code;
  UINT8   Psi1Thshld;

  //Power Management Setup options
  UINT8   PkgCstEntryValCtl;
  UINT8   NativeAspmEnable;

  // PRIMARY_PLANE_CURRENT_CONFIG_CONTROL  0x601
  UINT8   PpcccLock;

  UINT8   SnpLatVld;
  UINT8   SnpLatOvrd;
  UINT8   SnpLatMult;
  UINT16  SnpLatVal;
  UINT16  NonSnpLatVld;
  UINT8   NonSnpLatOvrd;
  UINT8   NonSnpLatMult;
  UINT16   NonSnpLatVal;

  // DYNAMIC_PERF_POWER_CTL (CSR 1:30:2:0x64)
  UINT8   EepLOverride;
  UINT8   EepLOverrideEn;
  UINT8   ITurboOvrdEn;
  UINT8   CstDemotOvrdEN;
  UINT8   TrboDemotOvrdEn;
  UINT8   UncrPerfPlmtOvrdEn;
  UINT8   EetOverrideEn;
  UINT8   IoBwPlmtOvrdEn;
  UINT8   ImcApmOvrdEn;                      // unused
  UINT8   IomApmOvrdEn;
  UINT8   KtiApmOvrdEn;
  UINT8   PerfPLmtThshld;

  // SAPMCTL_CFG  (CSR 1:30:1:0xB0)
  UINT8   Iio0PkgcClkGateDis[MAX_SOCKET]; //Bit[0]
  UINT8   Iio1PkgcClkGateDis[MAX_SOCKET]; //Bit[1]
  UINT8   Iio2PkgcClkGateDis[MAX_SOCKET]; //Bit[2]
  UINT8   Kti01PkgcClkGateDis[MAX_SOCKET]; //Bit[3]
  UINT8   Kti23PkgcClkGateDis[MAX_SOCKET];  //Bit[4]
  UINT8   Kti45PkgcClkGateDis[MAX_SOCKET];  //Bit[5]
  UINT8   P0pllOffEna[MAX_SOCKET]; //Bit[16]
  UINT8   P1pllOffEna[MAX_SOCKET]; //Bit[17]
  UINT8   P2pllOffEna[MAX_SOCKET]; //Bit[18]
  UINT8   Mc0pllOffEna[MAX_SOCKET]; //Bit[22]
  UINT8   Mc1pllOffEna[MAX_SOCKET]; //Bit[23]
  UINT8   Mc0PkgcClkGateDis[MAX_SOCKET]; //Bit[6]
  UINT8   Mc1PkgcClkGateDis[MAX_SOCKET]; //Bit[7]
  UINT8   Kti01pllOffEna[MAX_SOCKET];  //Bit[19]
  UINT8   Kti23pllOffEna[MAX_SOCKET];  //Bit[20]
  UINT8   Kti45pllOffEna[MAX_SOCKET];  //Bit[21]
  UINT8   SetvidDecayDisable[MAX_SOCKET];  //Bit[30];
  UINT8   SapmCtlLock[MAX_SOCKET];  //Bit[31];

  // PERF_P_LIMIT_CONTROL (CSR 1:30:2:0xe4)
  UINT8   PerfPLimitClip;
  UINT8   PerfPLimitEn;

  // PERF_P_LIMIT_CONTROL (CSR 1:30:2:0xe4) >= HSX C stepping
  UINT8   PerfPlimitDifferential;
  UINT8   PerfPLimitClipC;

  // SKX: PKG_CST_ENTRY_CRITERIA_MASK2 (CSR 1:30:2:0x90)
  UINT8   Kti0In[MAX_SOCKET];
  UINT8   Kti1In[MAX_SOCKET];
  UINT8   Kti2In[MAX_SOCKET];

  // SKX: PKG_CST_ENTRY_CRITERIA_MASK (CSR 1:30:2:0x8c)
  UINT8   PcieIio0In[MAX_SOCKET];
  UINT8   PcieIio1In[MAX_SOCKET];
  UINT8   PcieIio2In[MAX_SOCKET];
  UINT8   PcieIio3In[MAX_SOCKET];
  UINT8   PcieIio4In[MAX_SOCKET];
  UINT8   PcieIio5In[MAX_SOCKET];

  // WRITE_PKGC_SA_PS_CRITERIA (B2P)
  UINT8   EnablePkgcCriteria;
  UINT8   PkgCCriteriaLogicalIpType[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaKti[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaRlink[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaFxr[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaMcddr[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaHbm[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaIio[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaHqm[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaNac[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaTip[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaMdfs[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaHcx[MAX_SOCKET];
  UINT8   EnablePkgCCriteriaDino[MAX_SOCKET];
  UINT8   PkgCCriteriaLogicalIpTypeMcddr[MAX_SOCKET];
  UINT8   PkgCCriteriaLogicalIpTypeHbm[MAX_SOCKET];
  UINT8   PkgCCriteriaLogicalIpTypeIio[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoKti[MAX_SOCKET];
  UINT8   EnableLinkInL1Kti[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoRlink[MAX_SOCKET];
  UINT8   EnableLinkInL1Rlink[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoFxr[MAX_SOCKET];
  UINT8   PkgcCriteraPsMaskFxr[MAX_SOCKET];
  UINT8   PkgCCriteriaAllowedPsMaskFxr[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoMcddr[MAX_SOCKET];
  UINT8   PkgcCriteriaPsOptionMcddr[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoHbm[MAX_SOCKET];
  UINT8   PkgcCriteriaPsOptionHbm[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoIio[MAX_SOCKET];
  UINT8   EnableLinkInL1Iio[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoHqm[MAX_SOCKET];
  UINT8   PkgcCriteraPsMaskHqm[MAX_SOCKET];
  UINT8   PkgCCriteriaAllowedPsMaskHqm[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoNac[MAX_SOCKET];
  UINT8   PkgcCriteraPsMaskNac[MAX_SOCKET];
  UINT8   PkgCCriteriaAllowedPsMaskNac[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoTip[MAX_SOCKET];
  UINT8   PkgcCriteraPsMaskTip[MAX_SOCKET];
  UINT8   PkgCCriteriaAllowedPsMaskTip[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoMdfs[MAX_SOCKET];
  UINT8   AllowLpStateMdfs[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoHcx[MAX_SOCKET];
  UINT8   PkgcCriteraPsMaskHcx[MAX_SOCKET];
  UINT8   PkgCCriteriaAllowedPsMaskHcx[MAX_SOCKET];
  UINT8   PkgCCriteriaInstanceNoDino[MAX_SOCKET];
  UINT8   PkgcCriteraPsMaskDino[MAX_SOCKET];
  UINT8   PkgCCriteriaAllowedPsMaskDino[MAX_SOCKET];

  UINT8   FastRaplDutyCycle;
  UINT8   TurboPowerLimitLock;
  UINT8   TurboPowerLimitCsrLock;
  UINT8   PowerLimit1En;
  UINT16  PowerLimit1Power;
  UINT16  PowerLimit1Time;
  UINT8   PowerLimit2En;
  UINT16  PowerLimit2Power;
  UINT16  PowerLimit2Time;

  UINT8   PmaxDetector;
  UINT8   PmaxAutoAdjustment;
  UINT8   PmaxLoadLine;
  UINT8   PmaxSign;
  UINT8   PmaxOffset;
  UINT8   PmaxOffsetNegative;
  UINT8   PmaxTriggerSetup;

  //XTU 3.0

  UINT8   MaxEfficiencyRatio[MAX_SOCKET];
  UINT8   MaxNonTurboRatio[MAX_SOCKET];

  UINT8   VccSAandVccIOdisable;
  // Software LTR Override Control
  UINT8   SwLtrOvrdCtl;
  UINT8   EnhancedPmaxDetector;
  UINT8   PcodeWdogTimerEn;

  UINT8   RunCpuPpmInPei;

  UINT8   UncoreFreqRaplLimit;
} SOCKET_POWERMANAGEMENT_CONFIGURATION;
#pragma pack()

#endif




