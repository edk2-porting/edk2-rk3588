/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(_SB)
{
  Device (MDIO)
  {
    OperationRegion(CLKR, SystemMemory, 0x60000338, 8)
    Field(CLKR, DWordAcc, NoLock, Preserve) {
      CLKE, 1,  // clock enable
      , 31,
      CLKD, 1,  // clode disable
      , 31,
    }
    OperationRegion(RSTR, SystemMemory, 0x60000A38, 8)
    Field(RSTR, DWordAcc, NoLock, Preserve) {
      RSTE, 1,  // reset
      , 31,
      RSTD, 1,  // de-reset
      , 31,
    }

    Name(_HID, "HISI0141")
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0x603c0000 , 0x10000)
            })

    Method(_RST, 0, Serialized) {
      Store (0x1, RSTE)
      Sleep (10)
      Store (0x1, CLKD)
      Sleep (10)
      Store (0x1, RSTD)
      Sleep (10)
      Store (0x1, CLKE)
      Sleep (10)
    }
  }

  Device (DSF0)
  {
    OperationRegion(H3SR, SystemMemory, 0xC0000184, 4)
    Field(H3SR, DWordAcc, NoLock, Preserve) {
          H3ST, 1,
          , 31,  //RESERVED
        }
    OperationRegion(H4SR, SystemMemory, 0xC0000194, 4)
    Field(H4SR, DWordAcc, NoLock, Preserve) {
          H4ST, 1,
          , 31,  //RESERVED
        }
    // DSAF RESET
    OperationRegion(DRER, SystemMemory, 0xC0000A00, 8)
    Field(DRER, DWordAcc, NoLock, Preserve) {
          DRTE, 1,
          , 31,  //RESERVED
          DRTD, 1,
          , 31,  //RESERVED
        }
    // NT RESET
    OperationRegion(NRER, SystemMemory, 0xC0000A08, 8)
    Field(NRER, DWordAcc, NoLock, Preserve) {
          NRTE, 1,
          , 31,  //RESERVED
          NRTD, 1,
          , 31,  //RESERVED
        }
    // XGE RESET
    OperationRegion(XRER, SystemMemory, 0xC0000A10, 8)
    Field(XRER, DWordAcc, NoLock, Preserve) {
          XRTE, 31,
          , 1,    //RESERVED
          XRTD, 31,
          , 1,    //RESERVED
        }

    // GE RESET
    OperationRegion(GRTR, SystemMemory, 0xC0000A18, 16)
    Field(GRTR, DWordAcc, NoLock, Preserve) {
          GR0E, 30,
          , 2,    //RESERVED
          GR0D, 30,
          , 2,    //RESERVED
          GR1E, 18,
          , 14,  //RESERVED
          GR1D, 18,
          , 14,  //RESERVED
        }
    // PPE RESET
    OperationRegion(PRTR, SystemMemory, 0xC0000A48, 8)
    Field(PRTR, DWordAcc, NoLock, Preserve) {
          PRTE, 10,
          , 22,  //RESERVED
          PRTD, 10,
          , 22,  //RESERVED
        }

    // RCB PPE COM RESET
    OperationRegion(RRTR, SystemMemory, 0xC0000A88, 8)
    Field(RRTR, DWordAcc, NoLock, Preserve) {
          RRTE, 1,
          , 31,  //RESERVED
          RRTD, 1,
          , 31,  //RESERVED
        }

    // DSAF Channel RESET
    OperationRegion(DCRR, SystemMemory, 0xC0000AA8, 8)
    Field(DCRR, DWordAcc, NoLock, Preserve) {
          DCRE, 1,
          , 31,  //RESERVED
          DCRD, 1,
          , 31,  //RESERVED
        }

    // RoCE RESET
    OperationRegion(RKRR, SystemMemory, 0xC0000A50, 8)
    Field(RKRR, DWordAcc, NoLock, Preserve) {
          RKRE, 1,
          , 31,  //RESERVED
          RKRD, 1,
          , 31,  //RESERVED
        }

    // RoCE Clock enable/disable
    OperationRegion(RKCR, SystemMemory, 0xC0000328, 8)
    Field(RKCR, DWordAcc, NoLock, Preserve) {
          RCLE, 1,
          , 31,  //RESERVED
          RCLD, 1,
          , 31,  //RESERVED
        }

    // Hilink access sel cfg reg
    OperationRegion(HSER, SystemMemory, 0xC2240008, 0x4)
    Field(HSER, DWordAcc, NoLock, Preserve) {
          HSEL, 2,  // hilink_access_sel & hilink_access_wr_pul
          , 30,    // RESERVED
        }

    // Serdes
    OperationRegion(H4LR, SystemMemory, 0xC2208100, 0x1000)
    Field(H4LR, DWordAcc, NoLock, Preserve) {
          H4L0, 16,    // port0
          , 16,    //RESERVED
          Offset (0x400),
          H4L1, 16,    // port1
          , 16,    //RESERVED
          Offset (0x800),
          H4L2, 16,    // port2
          , 16,    //RESERVED
          Offset (0xc00),
          H4L3, 16,    // port3
          , 16,    //RESERVED
        }
    OperationRegion(H3LR, SystemMemory, 0xC2208900, 0x800)
    Field(H3LR, DWordAcc, NoLock, Preserve) {
          H3L2, 16,    // port4
          , 16,    //RESERVED
          Offset (0x400),
          H3L3, 16,    // port5
          , 16,    //RESERVED
        }
                Name (_HID, "HISI00B2")
                Name (_CCA, 1) // Cache-coherent controller
                Name (_CRS, ResourceTemplate (){
                        Memory32Fixed (ReadWrite, 0xc5000000 , 0x890000)
                        Memory32Fixed (ReadWrite, 0xc7000000 , 0x60000)
      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI3")
        {
          576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588,
          589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600,
        }
      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI3")
        {
          960, 961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975,
          976, 977, 978, 979, 980, 981, 982, 983, 984, 985, 986, 987, 988, 989, 990, 991,
          992, 993, 994, 995, 996, 997, 998, 999, 1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007,
          1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023,
          1024, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039,
          1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048, 1049, 1050, 1051, 1052, 1053, 1054, 1055,
          1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063, 1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071,
          1072, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087,
          1088, 1089, 1090, 1091, 1092, 1093, 1094, 1095, 1096, 1097, 1098, 1099, 1100, 1101, 1102, 1103,
          1104, 1105, 1106, 1107, 1108, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119,
          1120, 1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134, 1135,
          1136, 1137, 1138, 1139, 1140, 1141, 1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1150, 1151,
        }
      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI3")
        {
          1152, 1153, 1154, 1155, 1156, 1157, 1158, 1159, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167,
          1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183,
          1184, 1185, 1186, 1187, 1188, 1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199,
          1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215,
          1216, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231,
          1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247,
          1248, 1249, 1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260, 1261, 1262, 1263,
          1264, 1265, 1266, 1267, 1268, 1269, 1270, 1271, 1272, 1273, 1274, 1275, 1276, 1277, 1278, 1279,
          1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295,
          1296, 1297, 1298, 1299, 1300, 1301, 1302, 1303, 1304, 1305, 1306, 1307, 1308, 1309, 1310, 1311,
          1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319, 1320, 1321, 1322, 1323, 1324, 1325, 1326, 1327,
          1328, 1329, 1330, 1331, 1332, 1333, 1334, 1335, 1336, 1337, 1338, 1339, 1340, 1341, 1342, 1343,
        }
                })
                Name (_DSD, Package () {
                        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"mode", "6port-16rss"},
        Package () {"buf-size", 4096},
        Package () {"desc-num", 1024},
        Package () {"interrupt-parent", Package() {\_SB.MBI3}},
                        }
                })

    //reset XGE port
    //Arg0 : XGE port index in dsaf
    //Arg1 : 0 reset, 1 cancle reset
    Method(XRST, 2, Serialized) {
      ShiftLeft (0x2082082, Arg0, Local0)
      Or (Local0, 0x1, Local0)

      If (LEqual (Arg1, 0)) {
        Store(Local0, XRTE)
      } Else  {
        Store(Local0, XRTD)
      }
    }

    //reset XGE core
    //Arg0 : XGE port index in dsaf
    //Arg1 : 0 reset, 1 cancle reset
    Method(XCRT, 2, Serialized) {
      ShiftLeft (0x2080, Arg0, Local0)

      If (LEqual (Arg1, 0)) {
        Store(Local0, XRTE)
      } Else  {
        Store(Local0, XRTD)
      }
    }

    //reset GE port
    //Arg0 : GE port index in dsaf
    //Arg1 : 0 reset, 1 cancle reset
    Method(GRST, 2, Serialized) {
      If (LLessEqual (Arg0, 5)) {
        //Service port
        ShiftLeft (0x2082082, Arg0, Local0)
        ShiftLeft (0x1, Arg0, Local1)

        If (LEqual (Arg1, 0)) {
          Store(Local1, GR1E)
          Store(Local0, GR0E)
        } Else  {
          Store(Local0, GR0D)
          Store(Local1, GR1D)
        }
      }
    }

    //reset PPE port
    //Arg0 : PPE port index in dsaf
    //Arg1 : 0 reset, 1 cancle reset
    Method(PRST, 2, Serialized) {
      ShiftLeft (0x1, Arg0, Local0)
      If (LEqual (Arg1, 0)) {
        Store(Local0, PRTE)
      } Else  {
        Store(Local0, PRTD)
      }
    }

    //reset DSAF channels
    //Arg0 : mask
    //Arg1 : 0 reset, 1 de-reset
    Method(DCRT, 2, Serialized) {
      If (LEqual (Arg1, 0)) {
        Store(Arg0, DCRE)
      } Else  {
        Store(Arg0, DCRD)
      }
    }

    //reset RoCE
    //Arg0 : 0 reset, 1 de-reset
    Method(RRST, 1, Serialized) {
      If (LEqual (Arg0, 0)) {
        Store(0x1, RKRE)
      } Else  {
        Store(0x1, RCLD)
        Store(0x1, RKRD)
        sleep(20)
        Store(0x1, RCLE)
      }
    }

    // Set Serdes Loopback
    //Arg0 : port
    //Arg1 : 0 disable, 1 enable
    Method(SRLP, 2, Serialized) {
      ShiftLeft (Arg1, 10, Local0)
      Switch (ToInteger(Arg0))
      {
        case (0x0){
          Store (0, HSEL)
          Store (H4L0, Local1)
          And (Local1, 0xfffffbff, Local1)
          Or (Local0, Local1, Local0)
          Store (Local0, H4L0)
        }
        case (0x1){
          Store (0, HSEL)
          Store (H4L1, Local1)
          And (Local1, 0xfffffbff, Local1)
          Or (Local0, Local1, Local0)
          Store (Local0, H4L1)
        }
        case (0x2){
          Store (0, HSEL)
          Store (H4L2, Local1)
          And (Local1, 0xfffffbff, Local1)
          Or (Local0, Local1, Local0)
          Store (Local0, H4L2)
        }
        case (0x3){
          Store (0, HSEL)
          Store (H4L3, Local1)
          And (Local1, 0xfffffbff, Local1)
          Or (Local0, Local1, Local0)
          Store (Local0, H4L3)
        }
        case (0x4){
          Store (3, HSEL)
          Store (H3L2, Local1)
          And (Local1, 0xfffffbff, Local1)
          Or (Local0, Local1, Local0)
          Store (Local0, H3L2)
        }
        case (0x5){
          Store (3, HSEL)
          Store (H3L3, Local1)
          And (Local1, 0xfffffbff, Local1)
          Or (Local0, Local1, Local0)
          Store (Local0, H3L3)
        }
      }
    }

    //Reset
    //Arg0 : reset type (1: dsaf; 2: ppe; 3:xge core; 4:xge; 5:ge; 6:dchan; 7:roce)
    //Arg1 : port
    //Arg2 : 0 disable, 1 enable
    Method(DRST, 3, Serialized)
    {
      Switch (ToInteger(Arg0))
      {
        //DSAF reset
        case (0x1)
        {
          Store (Arg2, Local0)
          If (LEqual (Local0, 0))
          {
            Store (0x1, DRTE)
            Store (0x1, NRTE)
            Sleep (10)
            Store (0x1, RRTE)
          }
          Else
          {
            Store (0x1, DRTD)
            Store (0x1, NRTD)
            Sleep (10)
            Store (0x1, RRTD)
          }
        }
        //Reset PPE port
        case (0x2)
        {
          Store (Arg1, Local0)
          Store (Arg2, Local1)
          PRST (Local0, Local1)
        }

        //Reset XGE core
        case (0x3)
        {
          Store (Arg1, Local0)
          Store (Arg2, Local1)
          XCRT (Local0, Local1)
        }
        //Reset XGE port
        case (0x4)
        {
          Store (Arg1, Local0)
          Store (Arg2, Local1)
          XRST (Local0, Local1)
        }

        //Reset GE port
        case (0x5)
        {
          Store (Arg1, Local0)
          Store (Arg2, Local1)
          GRST (Local0, Local1)
        }

        //Reset DSAF Channels
        case (0x6)
        {
          Store (Arg1, Local0)
          Store (Arg2, Local1)
          DCRT (Local0, Local1)
        }

        //Reset RoCE
        case (0x7)
        {
          // Discarding Arg1 as it is always 0
          Store (Arg2, Local0)
          RRST (Local0)
        }
      }
    }

    // _DSM Device Specific Method
    //
    // Arg0: UUID Unique function identifier
    // Arg1: Integer Revision Level
    // Arg2: Integer Function Index
    //   0 : Return Supported Functions bit mask
    //   1 : Reset Sequence
    //    Arg3[0] : reset type (1:dsaf; 2:ppe; 3:xge core; 4:xge; 5:ge; 6:dchan; 7:roce)
    //    Arg3[1] : port index in dsaf
    //    Arg3[2] : 0 reset, 1 cancle reset
    //   2 : Set Serdes Loopback
    //    Arg3[0] : port
    //    Arg3[1] : 0 disable, 1 enable
    //   3 : LED op set
    //    Arg3[0] : op type
    //    Arg3[1] : port
    //    Arg3[2] : para
    //   4 : Get port type (GE or XGE)
    //    Arg3[0] : port index in dsaf
    //    Return : 0 GE, 1 XGE
    //   5 : Get sfp status
    //    Arg3[0] : port index in dsaf
    //    Return : 0 no sfp, 1 have sfp
    // Arg3: Package Parameters
    Method (_DSM, 4, Serialized)
    {
      If (LEqual(Arg0,ToUUID("1A85AA1A-E293-415E-8E28-8D690A0F820A")))
      {
        If (LEqual (Arg1, 0x00))
        {
          Switch (ToInteger(Arg2))
          {
            case (0x0)
            {
              Return (Buffer () {0x3F})
            }

            //Reset Sequence
            case (0x1)
            {
              Store (DeRefOf (Index (Arg3, 0)), Local0)
              Store (DeRefOf (Index (Arg3, 1)), Local1)
              Store (DeRefOf (Index (Arg3, 2)), Local2)
              DRST (Local0, Local1, Local2)
            }

            //Set Serdes Loopback
            case (0x2)
            {
              Store (DeRefOf (Index (Arg3, 0)), Local0)
              Store (DeRefOf (Index (Arg3, 1)), Local1)
              SRLP (Local0, Local1)
            }

            //LED op set
            case (0x3)
            {

            }

            // Get port type (GE or XGE)
            case (0x4)
            {
              Store (0, Local1)
              Store (DeRefOf (Index (Arg3, 0)), Local0)
              If (LLessEqual (Local0, 3))
              {
                // mac0: Hilink4 Lane0
                // mac1: Hilink4 Lane1
                // mac2: Hilink4 Lane2
                // mac3: Hilink4 Lane3
                Store (H4ST, Local1)
              }
              ElseIf (LLessEqual (Local0, 5))
              {
                // mac4: Hilink3 Lane2
                // mac5: Hilink3 Lane3
                Store (H3ST, Local1)
              }

              Return (Local1)
            }

            //Get sfp status
            case (0x5)
            {

            }
          }
        }
      }
      Return (Buffer() {0x00})
    }
    Device (PRT0)
    {
      Name (_ADR, 0x0)
      Name (_DSD, Package () {
                        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
            Package () {
                Package () {"reg", 0},
                Package () {"media-type", "fiber"},
                        }
                })
    }
    Device (PRT1)
    {
      Name (_ADR, 0x1)
      Name (_DSD, Package () {
                        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
            Package () {
                Package () {"reg", 1},
                Package () {"media-type", "fiber"},
                        }
                })
    }
    Device (PRT4)
    {
      Name (_ADR, 0x4)
      Name (_DSD, Package () {
                        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
            Package () {
                Package () {"reg", 4},
                                Package () {"phy-mode", "sgmii"},
                Package () {"phy-addr", 0},
                Package () {"mdio-node", Package (){\_SB.MDIO}},
                Package () {"media-type", "copper"},
                        }
                })
    }
    Device (PRT5)
    {
      Name (_ADR, 0x5)
      Name (_DSD, Package () {
                        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
            Package () {
                Package () {"reg", 5},
                                Package () {"phy-mode", "sgmii"},
                Package () {"phy-addr", 1},
                Package () {"mdio-node", Package (){\_SB.MDIO}},
                Package () {"media-type", "copper"},
                        }
                })
    }
  }
  Device (ETH4) {
    Name(_HID, "HISI00C2")
    Name (_CCA, 1) // Cache-coherent controller
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"local-mac-address", Package () { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}, // 6-bytes
        Package () {"ae-handle", Package (){\_SB.DSF0}},
        Package () {"port-idx-in-ae", 4},
      }
    })
  }
  Device (ETH5) {
    Name(_HID, "HISI00C2")
    Name (_CCA, 1) // Cache-coherent controller
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"local-mac-address", Package () { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}, // 6-bytes
        Package () {"ae-handle", Package (){\_SB.DSF0}},
        Package () {"port-idx-in-ae", 5},
      }
    })
  }
  Device (ETH0) {
    Name(_HID, "HISI00C2")
    Name (_CCA, 1) // Cache-coherent controller
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"local-mac-address", Package () { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}, // 6-bytes
        Package () {"ae-handle", Package (){\_SB.DSF0}},
        Package () {"port-idx-in-ae", 0},
      }
    })
  }
  Device (ETH1) {
    Name(_HID, "HISI00C2")
    Name (_CCA, 1) // Cache-coherent controller
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"local-mac-address", Package () { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}, // 6-bytes
        Package () {"ae-handle", Package (){\_SB.DSF0}},
        Package () {"port-idx-in-ae", 1},
      }
    })
  }
  Device (ROCE) {
    Name(_HID, "HISI00D1")
    Name (_CCA, 1) // Cache-coherent controller
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"eth-handle", Package () {\_SB.ETH0, \_SB.ETH1, 0, 0, \_SB.ETH4, \_SB.ETH5}},
        Package () {"dsaf-handle", Package (){\_SB.DSF0}},
        Package () {"node-guid", Package () { 0x00, 0x9A, 0xCD, 0x00, 0x00, 0x01, 0x02, 0x03 }}, // 8-bytes
        Package () {"interrupt-names", Package() {"hns-roce-comp-0",
                                                  "hns-roce-comp-1",
                                                  "hns-roce-comp-2",
                                                  "hns-roce-comp-3",
                                                  "hns-roce-comp-4",
                                                  "hns-roce-comp-5",
                                                  "hns-roce-comp-6",
                                                  "hns-roce-comp-7",
                                                  "hns-roce-comp-8",
                                                  "hns-roce-comp-9",
                                                  "hns-roce-comp-10",
                                                  "hns-roce-comp-11",
                                                  "hns-roce-comp-12",
                                                  "hns-roce-comp-13",
                                                  "hns-roce-comp-14",
                                                  "hns-roce-comp-15",
                                                  "hns-roce-comp-16",
                                                  "hns-roce-comp-17",
                                                  "hns-roce-comp-18",
                                                  "hns-roce-comp-19",
                                                  "hns-roce-comp-20",
                                                  "hns-roce-comp-21",
                                                  "hns-roce-comp-22",
                                                  "hns-roce-comp-23",
                                                  "hns-roce-comp-24",
                                                  "hns-roce-comp-25",
                                                  "hns-roce-comp-26",
                                                  "hns-roce-comp-27",
                                                  "hns-roce-comp-28",
                                                  "hns-roce-comp-29",
                                                  "hns-roce-comp-30",
                                                  "hns-roce-comp-31",
                                                  "hns-roce-async",
                                                  "hns-roce-common"}},
      }
    })
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xc4000000 , 0x100000)
      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI7")
        {
          722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733,
          734, 735, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745,
          746, 747, 748, 749, 750, 751, 752, 753, 785, 754,
        }
   })
    Name (_PRS, ResourceTemplate (){
      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive,,,)
        {
          722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733,
          734, 735, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745,
          746, 747, 748, 749, 750, 751, 752, 753, 785, 754,
        }
   })
 }
}
