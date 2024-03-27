#include "AcpiTables.h"

Device (YSND)
{
    // Name (_ADR, Zero) // _ADR: Address
    Name (_HID, "ESSX8326") // _HID: Hardware ID
    Name (_CID, "ESSX8326") // _CID: Compatible ID
    Name (_DDN, "Everest Codec Controller ES8326") // _DDN: DOS Device Name
    Name (_UID, One) // _UID: Unique ID
    // GDDBG Name (_DEP, Package (){\_SB.PCI0.GPI0, \_SB.PCI0.I2C2})
    //Melody ADD For linux driver>>
    Name (_DSD, Package (0x02)  // _DSD: Device-Specific Data
    {
        ToUUID ("68960506-A9D6-4969-8186-BD52F892D83E") /* Device Properties for _DSD */,
        Package (0x18)
        {
            Package (0x02)
            {
                "everest,mic1-src", // headset mic REG 2A
                0x22
            },

            Package (0x02)
            {
                "everest,mic2-src", // digital mic REG 2A
                0x44
            },

            Package (0x02)
            {
                "everest,jack-pol", //REG 57
                0x1F
            },

            Package (0x02)
            {
                "everest,dac-vol", //REG 50
                0xBF
            },

            Package (0x02)
            {
                "everest,adc-vol", //REG 33/34
                0xEE
            },
            Package (0x02)
            {
                "everest,pga-gain", //REG 23
                0x08
            },
            Package (0x02)
            {
                "everest,interrupt-src",
                0x10
            },
            Package (0x02)
            {
                "everest,interrupt-clk",
                0x45
            },
        },
    })
    Method (_PS3, 0, Serialized) // _PS3: Power State 3
    {
        //Store (Zero, CKC3)
        Store(0, \_SB.GPI2.SPKC)
    }
    Method (_PS0, 0, Serialized) // _PS0: Power State 0
    {
        //Store (One, CKC3)
        Store(1, \_SB.GPI2.SPKC)    
    }
    //Melody ADD For linux driver<<
    Method (_CRS, 0, Serialized) // _CRS: Current Resource Settings
    {
        Name (SBUF, ResourceTemplate () {
            I2cSerialBus (0x0019, ControllerInitiated, 198000000, AddressingMode7Bit, "\\_SB.I2C3",,,I2CD)
            // Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 24 } // 0x18 from x5.dts
        })

        Name (UBUF, ResourceTemplate () {
            GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPI2",,,GRST) {15}
        })             //GPIO_VER1_N_GPP_G6   MCU_TO_AMP_EN

        // Name (GBUF, ResourceTemplate () {
            // GpioInt (Edge, ActiveBoth, ExclusiveAndWake, PullNone, 0x0000, "\\_SB.GPI0", 0x00, ResourceConsumer,INTG) {0}
        // })             //GPIO_VER1_N_GPP_H11   UART1_RTS_N

        Name (CBUF, ResourceTemplate () {
            GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPI0",,,GRWT) {0}
        })             //GPIO_VER1_N_GPP_G0

        CreateWordField(UBUF, GRST._PIN, HDT1)
        // Store(GNUM(GPIO_VER1_N_GPP_G6), HDT1)   // MCU_TO_AMP_EN

        CreateWordField(GBUF,INTG._PIN,HDT2)
        // Store(GNUM(GPIO_VER1_N_GPP_H11),HDT2)  // UART1_RTS_N

        CreateWordField(CBUF,GRWT._PIN,HDT3)
        // Store(GNUM(GPIO_VER1_N_GPP_G0),HDT3)   // AUDIO_SWITCH_ON
        Return (ConcatenateResTemplate (SBUF, ConcatenateResTemplate (UBUF, ConcatenateResTemplate (CBUF, GBUF))))     
    }
    Method(_SUB, 0, Serialized)
    {
    Return("10EC1234") //           Ź ˾ Լ
    }
    Method (_STA, 0, Serialized) // _STA: Status
    {
    Return (0x0F)
    }

    Method (_DIS, 0, Serialized) // _DIS: Disable Device
    {
    }
    #define PLATFORM_MAINMIC_TYPE 0x00
    #define PLATFORM_HPMIC_TYPE 0x01
    #define PLATFORM_SPK_TYPE 0x02
    #define PLATFORM_HPDET_INV 0x03
    #define PLATFORM_MIC_DE_POP    0x06

    #define PLATFORM_CODEC_TYPE 0x0E
    #define PLATFORM_BUS_SLOT 0x0F

    #define HP_CODEC_LINEIN_PGA_GAIN 0x10
    #define MAIN_CODEC_LINEIN_PGA_GAIN 0x20

    #define HP_CODEC_D2SEPGA_GAIN 0x11
    #define MAIN_CODEC_D2SEPGA_GAIN 0x21

    #define HP_CODEC_ADC_VOLUME 0x12
    #define MAIN_CODEC_ADC_VOLUME 0x22

    #define HP_CODEC_ADC_ALC_ENABLE 0x13
    #define MAIN_CODEC_ADC_ALC_ENABLE 0x23

    #define HP_CODEC_ADC_ALC_TARGET_LEVEL 0x14
    #define MAIN_CODEC_ADC_ALC_TARGET_LEVEL 0x24

    #define HP_CODEC_ADC_ALC_MAXGAIN 0x15
    #define MAIN_CODEC_ADC_ALC_MAXGAIN 0x25

    #define HP_CODEC_ADC_ALC_MINGAIN 0x16
    #define MAIN_CODEC_ADC_ALC_MINGAIN 0x26

    #define HP_CODEC_ADC_ALC_HLDTIME 0x17
    #define MAIN_CODEC_ADC_ALC_HLDTIME 0x27

    #define HP_CODEC_ADC_ALC_DCYTIME 0x18
    #define MAIN_CODEC_ADC_ALC_DCYTIME 0x28

    #define HP_CODEC_ADC_ALC_ATKTIME 0x19
    #define MAIN_CODEC_ADC_ALC_ATKTIME 0x29

    #define HP_CODEC_ADC_ALC_NGTYPE 0x1a
    #define MAIN_CODEC_ADC_ALC_NGTYPE 0x2a

    #define HP_CODEC_ADC_ALC_NGTHLD 0x1b
    #define MAIN_CODEC_ADC_ALC_NGTHLD 0x2b

    #define MAIN_CODEC_ADC_GUI_STEP 0x2C
    #define MAIN_CODEC_ADC_GUI_GAIN_RANGE   0x2D


    #define HP_CODEC_DAC_HPMIX_HIGAIN 0x40
    #define SPK_CODEC_DAC_HPMIX_HIGAIN 0x50

    #define HP_CODEC_DAC_HPMIX_VOLUME 0x41
    #define SPK_CODEC_DAC_HPMIX_VOLUME 0x51

    #define HP_CODEC_DAC_HPOUT_VOLUME 0x42
    #define SPK_CODEC_DAC_HPOUT_VOLUME 0x52

    #define HP_CODEC_LDAC_VOLUME 0x44
    #define HP_CODEC_RDAC_VOLUME 0x54

    #define SPK_CODEC_LDAC_VOLUME 0x45
    #define SPK_CODEC_RDAC_VOLUME 0x55

    #define HP_CODEC_DAC_AUTOMUTE 0x46
    #define SPK_CODEC_DAC_AUTOMUTE 0x56

    #define HP_CODEC_DAC_MONO 0x4A
    #define SPK_CODEC_DAC_MONO 0x5A

    #define SPK_CTL_IO_LEVEL 0x5B
    // FOR ES8396
    #define CODEC_GPIO0_FUNC            0x80
    #define CODEC_GPIO1_FUNC            0x81
    #define CODEC_GPIO2_FUNC            0x82
    #define CODEC_GPIO3_FUNC            0x83
    #define CODEC_GPIO4_FUNC            0x84
            /*
    *TO DECLARE BCLK AND MCLK FREQ
    *BIT[7:4] MCLK FREQ
    *0 - 19.2MHz
    *1 - 24MHz
    *2 - 12.288MHz
    *F - Default for 19.2MHz
    *
    *BIT[3:0] BCLK FREQ
    *0 - 4.8MHz
    *1 - 2.4MHz
    *2 - 2.304MHz
    *3 - 3.072MHz
    *4 - 4.096MHz
    *F - Default for 4.8MHz
    */
    #define PLATFORM_MCLK_LRCK_FREQ     0x85

    #define HEADPHONE_DUMMY_REMOVE_ENABLE   0x2e
    #define PLATFORM_PCM_TYPE    0x04

    /*
    * How to enable/disable DRC
    * 0: disable DRC
    * 1: enable DRC
    */
    #define HP_CODEC_DRC_ENABLE          0x4C
    #define SPK_CODEC_DRC_ENABLE         0x5C

    /*
    * Set DRC limit
    * 0: normal mode
    * 1: limit mode
    */
    #define HP_CODEC_DRC_LIMIT            0x4D
    #define SPK_CODEC_DRC_LIMIT           0x5D

    /*
    * DRC fast gain recovery control
    * 0 - 0.125db
    * 1 - 0.375db
    * 2 - 0.625db
    * 3 - 0.875db
    */
    #define HP_CODEC_DRC_RECOVERY         0x4E
    #define SPK_CODEC_DRC_RECOVERY        0x5E

    /*
    * DRC winsize
    * 0 - 2 LRCK
    * 1 - 4 LRCK
    * ...
    * 15 - 65536 LRCK
    */
    #define HP_CODEC_DRC_WINSIZE          0x4F
    #define SPK_CODEC_DRC_WINSIZE         0x5F

    /*
    * DRC_LEVEL[3:0] DRC target level, Used to adjust volume of HP and SPK
    * 0 - '-30.0dB'(dfault)
    * 1 - '-27.0dB'
    * 2 - '-24.0dB'
    * 3 - '-22.0dB'
    * 4 - '-19.0dB'
    * 5 - '-17.0dB'
    * 6 - '-15.0dB'
    * 7 - '-13.5dB'
    * 8 - '-12.0dB'
    * 9 - '-10.5dB'
    * 10 - '-9.0dB'
    * 11 - '-7.5dB'
    * 12 - '-6.0dB'
    * 13 - '-4.5dB'
    * 14 - '-3.0dB'
    * 15 - '-1.5dB'
    */

    #define HP_CODEC_DRC_LEVEL            0x60
    #define SPK_CODEC_DRC_LEVEL           0x61

    Method (_DSM, 4, Serialized) // _DSM: Device-Specific Method
    {
    If (LEqual (Arg0, ToUUID ("68960506-A9D6-4969-8186-BD52F892D83E")))
    {
    if (LEqual (Arg2, PLATFORM_MCLK_LRCK_FREQ))
    {
      Return (0x00)
    }
    //For DRC control >>>
    If (LEqual (Arg2, HP_CODEC_DRC_ENABLE))
    {
        Return (0x00)
    }
    If (LEqual (Arg2, SPK_CODEC_DRC_ENABLE))
    {
        Return (0x00)
    }
    If (LEqual (Arg2, HP_CODEC_DRC_LIMIT))
    {
        Return (0x00)
    }
    If (LEqual (Arg2, SPK_CODEC_DRC_LIMIT))
    {
        Return (0x00)
    }
    If (LEqual (Arg2, HP_CODEC_DRC_RECOVERY))
    {
        Return (0x00)
    }
    If (LEqual (Arg2, SPK_CODEC_DRC_RECOVERY))
    {
        Return (0x00)
    }
    If (LEqual (Arg2, HP_CODEC_DRC_WINSIZE))
    {
        Return (0x08)
    }
    If (LEqual (Arg2, SPK_CODEC_DRC_WINSIZE))
    {
        Return (0x08)
    }
    //For DRC control <<<

    //For adjusting volume of HP and SPK >>>
    If (LEqual (Arg2, HP_CODEC_DRC_LEVEL))
    {
        Return (0x0F)
    }

    If (LEqual (Arg2, SPK_CODEC_DRC_LEVEL))
    {
        Return (0x0A)
    }
    //For adjusting volume of HP and SPK <<<

    If (LEqual (Arg2, PLATFORM_PCM_TYPE))
    {
    Return(0x03) //Changed from 0x0 to 0x3, for 24MHz clock
    }
    If (LEqual (Arg2, HEADPHONE_DUMMY_REMOVE_ENABLE))
    {
    Return(0x00)
    }
    If (LEqual (Arg2, PLATFORM_MAINMIC_TYPE)) //MAIN MIC TYPE
    {
    //Return (0xAA) // DMIC AT HIGH LEVEL
    Return (0x55) // DMIC AT LOW LEVEL
    //Return (0xBB) // AMIC at lin1rin1
    //Return (0xcc) // AMIC at LIN2RIN2
    }

    If (LEqual (Arg2, PLATFORM_HPMIC_TYPE)) //HP MIC TYPE
    {
    //Return (0xAA) // DMIC AT HIGH LEVEL
    //Return (0x55) // DMIC AT LOW LEVEL
    //Return (0xBB) // AMIC at lin1rin1
    return (0xcc) // AMIC at LIN2RIN2
    }

    If (LEqual (Arg2, PLATFORM_SPK_TYPE)) //SPEAKER TYPE
    {
    Return (0x02) //stereo speaker type
    //Return (0x01) //mono speaker type
    //Return (0x00) //no speaker
    }

    If (LEqual (Arg2, PLATFORM_HPDET_INV))
    {
    //Return (0x00) // Normal
    Return (0x01) // Inverted
    }

    If (LEqual (Arg2, PLATFORM_MIC_DE_POP))
    {
    Return (0x00) // no depop
    //Return (0x01) // de pop
    }


    If (LEqual (Arg2,  PLATFORM_BUS_SLOT))
    {
    return (0x00) // BIT[3:0] FOR BUS NUMBER, BIT[7:4] FOR SLOT NUMBER
    }

    If (LEqual (Arg2,  PLATFORM_CODEC_TYPE))
    {
      Return (0x1A) //Declare codec type: 16/36: 8136; 26: 8326; 96:8396; 95: 8395
    }

    If (LEqual (Arg2, HP_CODEC_LINEIN_PGA_GAIN))
    {
    //Return (0x00) //gain =0db
    //Return (0x01) //gain = +3db
    //Return (0x02) //gain = +6db
    //Return (0x03) //gain = +9db
    //Return (0x04) //gain = +12db
    //Return (0x05) //gain = +15db
    return (0x06) //gain = +18db
    //Return (0x07) //gain = +21db
    //Return (0x08) //gain = +24db
    //Return (0x09) //gain = +27db
    //Return (0x0a) //gain = +30db
    }
    If (LEqual (Arg2, MAIN_CODEC_LINEIN_PGA_GAIN))
    {
    //Return (0x00) //gain =0db
    //Return (0x01) //gain = +3db
    //Return (0x02) //gain = +6db
    //Return (0x03) //gain = +9db
    //Return (0x04) //gain = +12db
    //Return (0x05) //gain = +15db
    //return (0x06) //gain = +18db
    //Return (0x07) //gain = +21db
    Return (0x08) //gain = +24db
    //Return (0x09) //gain = +27db
    //Return (0x0a) //gain = +30db
    }

    If (LEqual (Arg2, MAIN_CODEC_ADC_GUI_STEP))
    {
    Return (0x03) //3db / step for mic boost gain
    //Return (0x06) //6db /step for mic boost gain
    //Return(0x0a)  //10db  /step for mic boost gain
    }

    If (LEqual (Arg2, MAIN_CODEC_ADC_GUI_GAIN_RANGE))
    {
    //definition for mic boost gain range, maximum gain
    Return(0x1B)   // 27DB GAIN RANGE
    }

    If (LEqual (Arg2, HP_CODEC_D2SEPGA_GAIN))
    {
    Return (0x01) //gain = +6db
    //Return (0x00) //gain = 0db
    }
    If (LEqual (Arg2, MAIN_CODEC_D2SEPGA_GAIN))
    {
    Return (0x01) //gain = +6db
    //Return (0x00) //gain = 0db
    }
    If (LEqual (Arg2, HP_CODEC_ADC_VOLUME))
    {
    Return (0xbf) // volume 0xbf= 0db, -0.5db/setp, 0x00= -95.5db0xff= +32db
    }
    If (LEqual (Arg2, MAIN_CODEC_ADC_VOLUME))
    {
    Return (0xbf) // volume 0xbf= 0db, 0.5db/setp, 0x00= -95.5db0xff= +32db
    }
    If (LEqual (Arg2, HP_CODEC_ADC_ALC_ENABLE))
    {
    Return (0x01) //enable ALC
    //Return (0x00) //Disable ALC
    }
    If (LEqual (Arg2, MAIN_CODEC_ADC_ALC_ENABLE))
    {
    Return (0x01) //enable ALC
    //Return (0x00) //Disable ALC
    }

    if (LEqual (Arg2, HP_CODEC_ADC_ALC_TARGET_LEVEL))
    {
    //Return (0x00) //gain = -16.5db
    //Return (0x01) //gain = -15db
    //Return (0x02) //gain = -13.5db
    //Return (0x03) //gain = -12db
    //Return (0x04) //gain = -10.5db
    //Return (0x05) //gain = -9db
    //return (0x06) //gain = -7.5db
    //Return (0x07) //gain = -6db
    //Return (0x08) //gain = -4.5db
    //Return (0x09) //gain = -3db
    return (0x0e) //gain = -1.5db
    }
    if (LEqual (Arg2, MAIN_CODEC_ADC_ALC_TARGET_LEVEL))
    {
    //Return (0x00) //gain = -16.5db
    //Return (0x01) //gain = -15db
    //Return (0x02) //gain = -13.5db
    //Return (0x03) //gain = -12db
    //Return (0x04) //gain = -10.5db
    //Return (0x05) //gain = -9db
    //return (0x06) //gain = -7.5db
    //Return (0x07) //gain = -6db
    //Return (0x08) //gain = -4.5db
    //Return (0x09) //gain = -3db
    return (0x0e) //gain = -1.5db
    }

    if (LEqual (Arg2, HP_CODEC_ADC_ALC_MAXGAIN))
    {

    Return (0xc6) //digtal gain 0xbf= 0db, -0.5db/setp, 0x00= -95.5db0xff= +32db
    }
    if (LEqual (Arg2, MAIN_CODEC_ADC_ALC_MAXGAIN))
    {
    return (0xf5) //digtal gain 0xbf= 0db, -0.5db/setp, 0x00= -95.5db0xff= +32db
    }

    if (LEqual (Arg2, HP_CODEC_ADC_ALC_HLDTIME))
    {
    return (0x00) //time = 0ms
    //Return (0x01) //time = 2.67ms
    //Return (0x02) //time = 5.33ms
    //Return (0x03) //time = 10.66ms
    //Return (0x04) //time = 21.32ms
    //Return (0x05) //time = 42.64ms
    //return (0x06) //time = 85.38ms
    //Return (0x07) //time = 170.76ms
    //Return (0x08) //time = 341.52ms
    //Return (0x09) //time = 0.68s
    //return (0x0a) //time = 1.36s
    }
    if (LEqual (Arg2, MAIN_CODEC_ADC_ALC_HLDTIME))
    {
    return (0x00) //time = 0ms
    //Return (0x01) //time = 2.67ms
    //Return (0x02) //time = 5.33ms
    //Return (0x03) //time = 10.66ms
    //Return (0x04) //time = 21.32ms
    //Return (0x05) //time = 42.64ms
    //return (0x06) //time = 85.38ms
    //Return (0x07) //time = 170.76ms
    //Return (0x08) //time = 341.52ms
    //Return (0x09) //time = 0.68s
    //return (0x0a) //time = 1.36s
    }
    if (LEqual (Arg2, HP_CODEC_ADC_ALC_ATKTIME))
    {
    //return (0x00) //time = 104us
    //Return (0x01) //time = 208us
    return (0x02) //time = 416ms
    //Return (0x03) //time = 832ms
    //Return (0x04) //time = 1.664ms
    //Return (0x05) //time = 3.328ms
    //return (0x06) //time = 6.656ms
    //Return (0x07) //time = 13.312ms
    //Return (0x08) //time = 26.624ms
    //Return (0x09) //time = 53.248ms
    //return (0x0a) //time = 106.496ms
    }
    if (LEqual (Arg2, MAIN_CODEC_ADC_ALC_ATKTIME))
    {
    //return (0x00) //time = 104us
    //Return (0x01) //time = 208us
    return (0x02) //time = 416ms
    //Return (0x03) //time = 832ms
    //Return (0x04) //time = 1.664ms
    //Return (0x05) //time = 3.328ms
    //return (0x06) //time = 6.656ms
    //Return (0x07) //time = 13.312ms
    //Return (0x08) //time = 26.624ms
    //Return (0x09) //time = 53.248ms
    //return (0x0a) //time = 106.496ms
    }
    if (LEqual (Arg2, HP_CODEC_LDAC_VOLUME))
    {
      Return (0xbf) //volume 0xbf= 0db, -0.5db/setp, 0x00= -95.5db0xff= +32db
    }

    if (LEqual (Arg2, SPK_CODEC_LDAC_VOLUME))
    {
      Return (0xB3) //volume 0xbf= 0db, -0.5db/setp, 0x00= -95.5db0xff= +32db
    }

    if (LEqual (Arg2, SPK_CTL_IO_LEVEL))
    {
    //Return (0x00)   //low level enable speaker
    Return (0x01) // high level enable speaker
    }
    If (LEqual(Arg2 , 0x8E))  //redirected path
    {
        Return (0)
    }

    If (LEqual(Arg2 , 0x8F))  //audio path number, one path for rendor, one path for capture
    {
        Return (0x11)
    }

    If (LEqual(Arg2 , 0x90))  //path 0, render path=out path, attached and with 2 jack
    {
        Return (0x22)
    }

    If (LEqual(Arg2 , 0x91))  //path 1, render path=c path, not attached and with 1 jack
    {
        Return (0x01)
    }

    If (LEqual(Arg2 , 0x92))  //path 2, render path=out path, not attached and with 1 jack
    {
        Return (0x01)
    }

    If (LEqual(Arg2 , 0x93))  //path 3, render path=out path, not attached and with 1 jack
    {
        Return (0x01)
    }

    If (LEqual(Arg2 , 0x94))  //path 0, capture path=input path, attached and with 1 jack
    {
        Return (0x62)    //OnBoard MIC: 0x62, USB MIC: 0x61
    }

     If (LEqual(Arg2 , 0x95))  //path 1, capture path=input path, not attached and with 1 jack
     {
         Return (0x41)
     }

     If (LEqual(Arg2 , 0x96))  //path 2, capture path=input path, not attached and with 1 jack
     {
         Return (0x41)
     }

     If (LEqual(Arg2 , 0x97))  //path 3, capture path=input path, not attached and with 1 jack
     {
         Return (0x41)
     }

     If (LEqual(Arg2 , 0x98))  //render path 0 time slot number = 0, no jack detection capability
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0x99))  //render path 1 time slot number = 0, with jack detection capability
     {
         Return (0x80)
     }

     If (LEqual(Arg2 , 0x9a))  //render path 2 time slot number = 0, no jack detection capability
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0x9b))  //render path 3 time slot number = 0, no jack detection capability
     {
         Return (0x00)
     }

    If (LEqual(Arg2 , 0x9c))  //capture path 0 time slot number = 0, with jack detection capability
    {
        Return (0x00)    //OnBoard MIC: 0x00, USB MIC: 0x80
    }

    If (LEqual(Arg2 , 0x9d))  //capture path 1 time slot number = 0, no jack detection capability
    {
        Return (0x80)    //OnBoard MIC: 0x80, USB MIC: 0x00
    }

     If (LEqual(Arg2 , 0x9e))  //capture path 2 time slot number = 0, no jack detection capability
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0x9f))  //capture path 3 time slot number = 0, no jack detection capability
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xa0))  //render path 0 connectype =  eConnTypeOtherAnalog, portConnection = ePortConnIntegratedDevice
     {
         Return (0x71)
     }
     If (LEqual(Arg2 , 0xa1))  //render path 1 connectype =  eConnType3Point5mm, portConnection = ePortConnJack
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xa2))  //render path 2 connectype =  eConnType3Point5mm, portConnection = ePortConnJack
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xa3))  //render path 3 connectype =  eConnType3Point5mm, portConnection = ePortConnJack
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xa4))  //Capture path 0 connectype =  eConnTypeOtherAnalog, portConnection = ePortConnIntegratedDevice
     {
         Return (0x71)
     }
     If (LEqual(Arg2 , 0xa5))  //Capture path 1 connectype =  eConnType3Point5mm, portConnection = ePortConnJack
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xa6))  //Capture path 2 connectype =  eConnType3Point5mm, portConnection = ePortConnJack
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xa7))  //Capture path 3 connectype =  eConnType3Point5mm, portConnection = ePortConnJack
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xa8))  //Render path 0 geoLocation =  eGeoLocFront, genLocation = eGenLocPrimaryBox
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xa9))  //Render path 1 geoLocation =  eGeoLocRight, genLocation = eGenLocPrimaryBox
     {
         Return (0x30)
     }

     If (LEqual(Arg2 , 0xaa))  //Render path 2 geoLocation =  eGeoLocRight, genLocation = eGenLocPrimaryBox
     {
         Return (0x30)
     }

     If (LEqual(Arg2 , 0xab))  //Render path 3 geoLocation =  eGeoLocRight, genLocation = eGenLocPrimaryBox
     {
         Return (0x30)
     }

     If (LEqual(Arg2 , 0xac))  //Capture path 0 geoLocation =  eGeoLocFront, genLocation = eGenLocPrimaryBox
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xad))  //Capture path 1 geoLocation =  eGeoLocRight, genLocation = eGenLocPrimaryBox
     {
         Return (0x30)
     }

     If (LEqual(Arg2 , 0xae))  //Capture path 2 geoLocation =  eGeoLocRight, genLocation = eGenLocPrimaryBox
     {
         Return (0x30)
     }

     If (LEqual(Arg2 , 0xaf))  //Capture path 3 geoLocation =  eGeoLocRight, genLocation = eGenLocPrimaryBox
     {
         Return (0x30)
     }

     If (LEqual(Arg2 , 0xb0))  //Render path 0 defDevice = eDefaultDeviceSpeaker, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0x10)
     }

     If (LEqual(Arg2 , 0xb1))  //Render path 1 defDevice = eDefaultDeviceHPOut, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0x20)
     }

     If (LEqual(Arg2 , 0xb2))  //Render path 2 defDevice = eDefaultDeviceHPOut, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0x20)
     }

     If (LEqual(Arg2 , 0xb3))  //Render path 3 defDevice = eDefaultDeviceHPOut, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0x20)
     }

     If (LEqual(Arg2 , 0xb4))  //Capture path 0 defDevice = eDefaultDeviceMicIn, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0xa0)
     }

     If (LEqual(Arg2 , 0xb5))  //Capture path 1 defDevice = eDefaultDeviceTelephony, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0xb0)
     }

     If (LEqual(Arg2 , 0xb6))  //Capture path 2 defDevice = eDefaultDeviceTelephony, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0xb0)
     }

     If (LEqual(Arg2 , 0xb7))  //Capture path 3 defDevice = eDefaultDeviceTelephony, ChannelMapping = KSAUDIO_SPEAKER_DIRECTOUT
     {
         Return (0xb0)
     }

     If (LEqual(Arg2 , 0xb8))  //render path 0 KSCATEGORY = KSNODETYPE_SPEAKER
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xB9))  //render path 1 KSCATEGORY = KSNODETYPE_HEADPHONES
     {
         Return (0x01)
     }

     If (LEqual(Arg2 , 0xBA))  //render path 2 KSCATEGORY = KSNODETYPE_HEADPHONES
     {
         Return (0x01)
     }

     If (LEqual(Arg2 , 0xBB))  //render path 3 KSCATEGORY = KSNODETYPE_HEADPHONES
     {
         Return (0x01)
     }

     If (LEqual(Arg2 , 0xBC))  //Capture path 0 KSCATEGORY = KSNODETYPE_MICROPHONE
     {
         Return (0x03)
     }

     If (LEqual(Arg2 , 0xBD))  //Capture path 1 KSCATEGORY = KSNODETYPE_HEADSET_MICROPHONE
     {
         Return (0x04)
     }

     If (LEqual(Arg2 , 0xBE))  //Capture path 2 KSCATEGORY = KSNODETYPE_HEADSET_MICROPHONE
     {
         Return (0x04)
     }

     If (LEqual(Arg2 , 0xBF))  //Capture path 3 KSCATEGORY = KSNODETYPE_HEADSET_MICROPHONE
     {
         Return (0x04)
     }

     If (LEqual(Arg2 , 0xC0))  //render path 0 color-BYTE 3
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xC1))  //render path 0 color-BYTE 2
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xC2))  //render path 0 color-BYTE 1
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xC3))  //render path 0 color-BYTE 0
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xC4))  //render path 1 color-BYTE 3
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xC5))  //render path 1 color-BYTE 2
     {
         Return (0xB3)
     }

     If (LEqual(Arg2 , 0xC6))  //render path 1 color-BYTE 1
     {
         Return (0xC9)
     }

     If (LEqual(Arg2 , 0xC7))  //render path 1 color-BYTE 0
     {
         Return (0x8C)
     }

     If (LEqual(Arg2 , 0xC8))  //render path 2 color-BYTE 3
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xC9))  //render path 2 color-BYTE 2
     {
         Return (0xB3)
     }

     If (LEqual(Arg2 , 0xCA))  //render path 2 color-BYTE 1
     {
         Return (0xC9)
     }

     If (LEqual(Arg2 , 0xCB))  //render path 2 color-BYTE 0
     {
         Return (0x8C)
     }

     If (LEqual(Arg2 , 0xCC))  //render path 3 color-BYTE 3
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xCD))  //render path 3 color-BYTE 2
     {
         Return (0xB3)
     }

     If (LEqual(Arg2 , 0xCE))  //render path 3 color-BYTE 1
     {
         Return (0xC9)
     }

     If (LEqual(Arg2 , 0xCF))  //render path 3 color-BYTE 0
     {
         Return (0x8C)
     }

     If (LEqual(Arg2 , 0xD0))  //Capture path 0 color-BYTE 3
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xD1))  //Capture path 0  color-BYTE 2
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xD2))  //Capture path 0  color-BYTE 1
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xD3))  //Capture path 0  color-BYTE 0
     {
         Return (0xFF)
     }

     If (LEqual(Arg2 , 0xD4))  //Capture path 1 color-BYTE 3
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xD5))  //Capture path 1  color-BYTE 2
     {
         Return (0xC6)
     }

     If (LEqual(Arg2 , 0xD6))  //Capture path 1  color-BYTE 1
     {
         Return (0x73)
     }

     If (LEqual(Arg2 , 0xD7))  //Capture path 1  color-BYTE 0
     {
         Return (0x86)
     }

     If (LEqual(Arg2 , 0xD8))  //Capture path 2 color-BYTE 3
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xD9))  //Capture path 2  color-BYTE 2
     {
         Return (0xC6)
     }

     If (LEqual(Arg2 , 0xDA))  //Capture path 2  color-BYTE 1
     {
         Return (0x73)
     }

     If (LEqual(Arg2 , 0xDB))  //Capture path 2  color-BYTE 0
     {
         Return (0x86)
     }

     If (LEqual(Arg2 , 0xDC))  //Capture path 3 color-BYTE 3
     {
         Return (0x00)
     }

     If (LEqual(Arg2 , 0xDD))  //Capture path 3  color-BYTE 2
     {
         Return (0xC6)
     }

     If (LEqual(Arg2 , 0xDE))  //Capture path 3  color-BYTE 1
     {
         Return (0x73)
     }

     If (LEqual(Arg2 , 0xDF))  //Capture path 3  color-BYTE 0
     {
         Return (0x86)
     }
    }

    Return (0xFF)
    }
}
// I2S bus i2s0
Device (I2S0)
{
    // Name (_ADR, Zero) // _ADR: Address
    Name (_HID, "GEDU0600") // _HID: Hardware ID
    Name (_CID, "GEDU0600") // _CID: Compatible ID
    Name (_UID, Zero) // _UID: Unique ID
    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xFE470000, 0x10000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 212 }
        })
        Return (RBUF)
    }    
    Name (_DSD, Package () {
        ToUUID("05ABF9AE-C144-4B39-922C-7F494C8A737F"),
        Package () {
            //dma-names = "tx\0rx"
          Package (2) { "rockchip,dma", "GdDMACtrl0" },
          Package (2) { "rockchip,tplg", "i2s-jack" },
          Package (2) { "rockchip,tx", 0 }, 
          Package (2) { "rockchip,rx", 1 },
        }
    })    
    Method (_STA)
    {
      Return(0xf)
    }    
}
// I2S bus i2s1
Device (I2S1)
{
    // Name (_ADR, Zero) // _ADR: Address
    Name (_HID, "GEDU0600") // _HID: Hardware ID
    Name (_CID, "GEDU0600") // _CID: Compatible ID
    Name (_UID, One) // _UID: Unique ID
    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xFE480000, 0x10000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 213 }
        })
        Return (RBUF)
    }    
    Method (_STA)
    {
      Return(0)// disabled
    }    
}
// I2S bus i2s2
Device (I2S2)
{
    // Name (_ADR, Zero) // _ADR: Address
    Name (_HID, "GEDU0600") // _HID: Hardware ID
    Name (_CID, "GEDU0600") // _CID: Compatible ID
    Name (_UID, 0x02) // _UID: Unique ID
    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xFE490000, 0x10000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 214 }
        })
        Return (RBUF)
    }    
    Method (_STA)
    {
        Return(0)// disabled
    }    
}
// I2S bus i2s3
Device (I2S3)
{
    // Name (_ADR, Zero) // _ADR: Address
    Name (_HID, "GEDU0600") // _HID: Hardware ID
    Name (_CID, "GEDU0600") // _CID: Compatible ID
    Name (_UID, 0x03) // _UID: Unique ID
    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xFE4A0000, 0x10000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 215 }
        })
        Return (RBUF)
    }    
    Method (_STA)
    {
        Return(0)// disabled
    }    
}
// I2S bus i2s4
Device (I2S4)
{
    // Name (_ADR, Zero) // _ADR: Address
    Name (_HID, "GEDU0600") // _HID: Hardware ID
    Name (_CID, "GEDU0600") // _CID: Compatible ID
    Name (_UID, 0x04) // _UID: Unique ID
    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xFDDC0000, 0x8000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 216 }
        })
        Return (RBUF)
    }    
    Method (_STA)
    {
        Return(0)// disabled
    }    
}
// I2S5_8CH
Device (I2S5)
{
    // Name (_ADR, Zero) // _ADR: Address
    Name (_HID, "GEDU0600") // _HID: Hardware ID
    Name (_CID, "GEDU0600") // _CID: Compatible ID
    Name (_UID, 0x04) // _UID: Unique ID
    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xFDDF0000, 0x4000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 217 }
        })
        Return (RBUF)
    }
    Name (_DSD, Package () {
        ToUUID("05ABF9AE-C144-4B39-922C-7F494C8A737F"),
        Package () {
          Package (2) { "rockchip,dma", "GdDMACtrl1" },
          Package (2) { "rockchip,tplg", "hdmi" },
          Package (2) { "rockchip,tx", 2 }, // dma index for tx
        }
    })      
    Method (_STA)
    {
        Return(0xf)
    }    
}