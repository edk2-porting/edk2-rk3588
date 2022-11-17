/** @file
  Intel CPU PPM policy common structures and macros for both PPM policy PPI and
  policy protocol.

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PPM_POLICY_PEI_DXE_COMMON_HEADER__
#define __PPM_POLICY_PEI_DXE_COMMON_HEADER__


/*===============================================================

  !!! Keep this file common for both PEI and DXE use !!!

===============================================================*/


#include <CpuDataStruct.h>


#define NUM_CST_LAT_MSR       3
#define NUM_TURBO_RATIO_GROUP 8

//
// Hardware P States Modes
//
typedef enum {
  HWP_MODE_DISABLED         = 0, // also known as legacy P states
  HWP_MODE_NATIVE           = 1, // native with legacy support
  HWP_MODE_OOB              = 2, // out of band
  HWP_MODE_NATIVE_NO_LEGACY = 3  // native w/o legacy support
} HWP_MODES;

//
// Power Perf Tuning options
//
// HWP is disabled         : OS, BIOS and PECI control all available. OS control is the default
// HWP native mode         : OS, BIOS and PECI control all available. OS control is the default
// HWP is OOB              : BIOS and PECI control available. PECI control is the default
// HWP is native w/o legacy: BIOS and PECI control available. BIOS control is the default
//
// On CPX, PECI is not available
//
typedef enum {
  PWR_PERF_TUNING_OS_CONTROL    = 0,
  PWR_PERF_TUNING_BIOS_CONTROL  = 1,
  PWR_PERF_TUNING_PECI_CONTROL  = 2
} POWER_PERF_TUNING_CONTROL;

#pragma pack(1)


typedef struct {
  UINT8            PkgCstEntryValCtl;
  UINT8            SapmctlValCtl;
  UINT8            SkipPkgCstEntry;
  UINT8            SwLtrOvrdCtl;
  UINT8            PriPlnCurrCfgCtl;
  UINT8            CurrentConfig;
  UINT8            MsrLock;
  UINT8            MsrPkgCstConfigControlLock;
  UINT8            MpllOffEnaAuto;
  UINT8            DynamicL1Disable;
  UINT8            VccsaVccioDisable;
  UINT8            PcodeWdogTimerEn;
  UINT8            DramRaplPwrLimitLockCsr;
  UINT8            EnableLowerLatencyMode;
} ADV_PWR_CTL;

typedef struct {
  UINT8            BidirProchotEnable;
  UINT8            C1eEnable;
  UINT8            EeTurboDisable;
  UINT8            ProchotOutputDisable;
  UINT8            SapmControl;
  UINT8            PwrPerfSwitch;
  POWER_PERF_TUNING_CONTROL PwrPerfTuning;
  UINT8            ProchotLock;
  UINT8            LtrSwInput;
  UINT8            PkgCLatNeg;
  UINT8            SetvidDecayDisable;
} POWER_CTL;

typedef struct {
  UINT16           PowerLimit1Power;
  UINT8            PowerLimit1En;
  UINT16           PowerLimit1Time;
  UINT8            PkgClmpLim1;
  UINT16           PowerLimit2Power;
  UINT8            PkgClmpLim2;
  UINT8            PowerLimit2En;
  UINT16           PowerLimit2Time;
  UINT8            TurboPowerLimitLock;
  UINT8            TurboLimitCsrLock;
} TURBO_POWRER_LIMIT;

typedef struct {
  UINT16           CurrentLimit;
  UINT8            PpcccLock;
} PPO_CURRENT_CFG;

typedef struct {
  UINT8            WorkLdConfig;
  UINT8            AltEngPerfBIAS;
  UINT8            P0TtlTimeHigh1;
  UINT8            P0TtlTimeLow1;
  UINT16           EngAvgTimeWdw1;
} PERF_BIAS_CONFIG;

typedef struct {
  UINT8            PmaxDetector;
  UINT8            PmaxAutoAdjustment;
  UINT8            PmaxLoadLine;
  UINT8            PmaxSign;
  UINT8            PmaxOffset;
  UINT8            PmaxOffsetNegative;
  UINT8            PmaxTriggerSetup;
  UINT16           BasePackageTdp[MAX_SOCKET];
  UINT8            EnhancedPmaxDetector;
} PMAX_CONFIG;

typedef struct {
  UINT8            Iio0PkgcClkGateDis;
  UINT8            Iio1PkgcClkGateDis;
  UINT8            Iio2PkgcClkGateDis;
  UINT8            Kti01PkgcClkGateDis;
  UINT8            Kti23PkgcClkGateDis;
  UINT8            Kti45PkgcClkGateDis;
  UINT8            Mc0PkgcClkGateDis;
  UINT8            Mc1PkgcClkGateDis;
  UINT8            P0pllOffEna;
  UINT8            P1pllOffEna;
  UINT8            P2pllOffEna;
  UINT8            Kti01pllOffEna;
  UINT8            Kti23pllOffEna;
  UINT8            Kti45pllOffEna;
  UINT8            Mc0pllOffEna;
  UINT8            Mc1pllOffEna;
  UINT8            Mc0PkgcIoVolRedDis;
  UINT8            Mc1PkgcIoVolRedDis;
  UINT8            Mc0PkgcDigVolRedDis;
  UINT8            Mc1PkgcDigVolRedDis;
  UINT8            SetvidDecayDisable;
  UINT8            SapmCtlLock;
} SAPM_CTL;

typedef struct {
  UINT8            PerfPLimitEn;
  UINT8            PerfPLmtThshld;
  UINT8            PerfPLimitClipC;
  UINT8            PerfPlimitDifferential;
} PERF_PLIMIT_CTL;

typedef struct {
  UINT8            KtiApmOvrdEn;
  UINT8            IomApmOvrdEn;
  UINT8            IoBwPlmtOvrdEn;
  UINT8            EetOverrideEn;
  UINT8            UncrPerfPlmtOvrdEn;
} DYNAMIC_PER_POWER_CTL;

typedef struct {
  UINT16           NonSnpLatVal;
  UINT8            NonSnpLatMult;
  UINT8            NonSnpLatOvrd;
  UINT16           NonSnpLatVld;
  UINT16           SnpLatVal;
  UINT8            SnpLatMult;
  UINT8            SnpLatOvrd;
  UINT8            SnpLatVld;
} PCIE_ILTR_OVRD;

typedef struct {
  UINT16           Value;
  UINT8            Multiplier;
  UINT8            Valid;
} CST_LATENCY_CTL;


typedef struct {
  BOOLEAN          C1e;

  UINT32           PkgCstEntryCriteriaMaskKti[MAX_SOCKET];
  UINT32           PkgCstEntryCriteriaMaskPcie[MAX_SOCKET];
  CST_LATENCY_CTL  LatencyCtrl[NUM_CST_LAT_MSR];
} PPM_CSTATE_STRUCT;

typedef struct {
  BOOLEAN          Enable;
  UINT32           Voltage;
  UINT16           RatioLimit[MAX_CORE];
} PPM_XE_STRUCT;

typedef struct {
  UINT8            RatioLimitRatio[NUM_TURBO_RATIO_GROUP];
  UINT8            RatioLimitRatioMask[NUM_TURBO_RATIO_GROUP];
  UINT8            RatioLimitCores[NUM_TURBO_RATIO_GROUP];
  UINT8            RatioLimitCoresMask[NUM_TURBO_RATIO_GROUP];
} TURBO_RATIO_LIMIT;

typedef struct {
  HWP_MODES        HWPMEnable;
  UINT8            HWPMNative;
  UINT8            HWPMOOB;
  UINT8            HWPMInterrupt;
  UINT8            EPPEnable;
  UINT8            EPPProfile;
  UINT8            APSrocketing;
  UINT8            Scalability;
  UINT8            PPOTarget;
  UINT8            RaplPrioritization;
  UINT32           SstCpSystemStatus;
  UINT8            OutofBandAlternateEPB;
  UINT8            ConfigurePbf;
  UINT64           PbfHighPriCoreMap[MAX_SOCKET];    // PBF High Priority Cores Bitmap
  UINT8            PbfP1HighRatio[MAX_SOCKET];       // PBF P1_High Ratio
  UINT8            PbfP1LowRatio[MAX_SOCKET];        // PBF P1_Low Ratio
} PPM_HWPM_STRUCT;

typedef struct {
  UINT8            EnablePkgcCriteria;
  UINT8            EnablePkgCCriteriaKti[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaRlink[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaFxr[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaMcddr[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaHbm[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaIio[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaHqm[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaNac[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaTip[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaMdfs[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaHcx[MAX_SOCKET];
  UINT8            EnablePkgCCriteriaDino[MAX_SOCKET];
  UINT8            PkgCCriteriaLogicalIpType[MAX_SOCKET];
  UINT8            PkgCCriteriaLogicalIpTypeMcddr[MAX_SOCKET];
  UINT8            PkgCCriteriaLogicalIpTypeHbm[MAX_SOCKET];
  UINT8            PkgCCriteriaLogicalIpTypeIio[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoKti[MAX_SOCKET];
  UINT8            EnableLinkInL1Kti[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoRlink[MAX_SOCKET];
  UINT8            EnableLinkInL1Rlink[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoFxr[MAX_SOCKET];
  UINT8            PkgcCriteraPsMaskFxr[MAX_SOCKET];
  UINT8            PkgCCriteriaAllowedPsMaskFxr[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoMcddr[MAX_SOCKET];
  UINT8            PkgcCriteriaPsOptionMcddr[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoHbm[MAX_SOCKET];
  UINT8            PkgcCriteriaPsOptionHbm[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoIio[MAX_SOCKET];
  UINT8            EnableLinkInL1Iio[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoHqm[MAX_SOCKET];
  UINT8            PkgcCriteraPsMaskHqm[MAX_SOCKET];
  UINT8            PkgCCriteriaAllowedPsMaskHqm[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoNac[MAX_SOCKET];
  UINT8            PkgcCriteraPsMaskNac[MAX_SOCKET];
  UINT8            PkgCCriteriaAllowedPsMaskNac[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoTip[MAX_SOCKET];
  UINT8            PkgcCriteraPsMaskTip[MAX_SOCKET];
  UINT8            PkgCCriteriaAllowedPsMaskTip[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoMdfs[MAX_SOCKET];
  UINT8            AllowLpStateMdfs[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoHcx[MAX_SOCKET];
  UINT8            PkgcCriteraPsMaskHcx[MAX_SOCKET];
  UINT8            PkgCCriteriaAllowedPsMaskHcx[MAX_SOCKET];
  UINT8            PkgCCriteriaInstanceNoDino[MAX_SOCKET];
  UINT8            PkgcCriteraPsMaskDino[MAX_SOCKET];
  UINT8            PkgCCriteriaAllowedPsMaskDino[MAX_SOCKET];
} PKGC_SA_PS_CRITERIA_STRUCT;

typedef struct {
  UINT8            ThermalMonitorStatusFilter;
  UINT8            ThermalMonitorStatusFilterTimeWindow;
} TM_STATUS_Filter;

typedef struct {
  UINT8                  IssCapableSystem;
  UINT8                  DynamicIss;
  UINT8                  ConfigTDPLevel;
  UINT8                  ConfigTDPLock;
  UINT16                 CurrentPackageTdp[MAX_SOCKET];
  UINT8                  FastRaplDutyCycle;
  UINT32                 ProchotRatio;
  UINT8                  OverclockingLock;
  UINT32                 C2C3TT;
  UINT8                  AvxSupport;
  UINT8                  AvxLicensePreGrant;
  UINT8                  AvxIccpLevel;
  UINT8                  GpssTimer;

  ADV_PWR_CTL            AdvPwrMgtCtl;
  POWER_CTL              PowerCtl;
  TURBO_POWRER_LIMIT     TurboPowerLimit;
  PPO_CURRENT_CFG        PpoCurrentCfg;
  PERF_BIAS_CONFIG       PerfBiasConfig;
  PMAX_CONFIG            PmaxConfig;
  TM_STATUS_Filter       ThermalReport;
  SAPM_CTL               SapmCtl[MAX_SOCKET];
  PERF_PLIMIT_CTL        PerPLimitCtl;
  DYNAMIC_PER_POWER_CTL  DynamicPerPowerCtl;
  PCIE_ILTR_OVRD         PcieIltrOvrd;

  PPM_CSTATE_STRUCT      PpmCst;
  PPM_XE_STRUCT          PpmXe;
  PPM_HWPM_STRUCT        Hwpm;
  TURBO_RATIO_LIMIT      TurboRatioLimit;

  PKGC_SA_PS_CRITERIA_STRUCT  PkgcCriteria;

  UINT8                  CpuThermalManagement;
  UINT8                  RunCpuPpmInPei;
} PPM_POLICY_CONFIGURATION;


#pragma pack()

#endif
