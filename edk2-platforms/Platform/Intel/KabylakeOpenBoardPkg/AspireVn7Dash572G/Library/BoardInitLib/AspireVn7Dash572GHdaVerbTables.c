/** @file
  HDA Verb table for Acer Aspire VN7-572G

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ASPIRE_VN7_572G_HDA_VERB_TABLES_H_
#define _ASPIRE_VN7_572G_HDA_VERB_TABLES_H_

#include <Ppi/SiPolicy.h>

HDAUDIO_VERB_TABLE HdaVerbTableAlc255AspireVn7Dash572G = HDAUDIO_VERB_TABLE_INIT (
  //
  //  VerbTable: (Realtek ALC255) for Aspire VN7-572G
  //  Revision ID = 0xff
  //  Codec Verb Table for SKL PCH boards
  //  Codec Address: CAd value (0/1/2)
  //  Codec Vendor: 0x10EC0255
  //
  0x10EC, 0x0255,
  0xFF, 0xFF,

  // The number of verb command block : 20
  //    NID 0x12 : 0x411111C0
  //    NID 0x14 : 0x90172120
  //    NID 0x17 : 0x40000000
  //    NID 0x18 : 0x411111F0
  //    NID 0x19 : 0x411111F0
  //    NID 0x1A : 0x411111F0
  //    NID 0x1B : 0x411111F0
  //    NID 0x1D : 0x40700001
  //    NID 0x1E : 0x411111F0
  //    NID 0x21 : 0x02211030

  // Codec Address: Bits 31:28
  // Node ID: Bits 27:20
  // Verb ID: Bits 19:8 / Bits 19:16
  // Payload: Bits 7:0  / Bits 15:0

  //Widget node 0x01 : Reset Codec
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,

  //===== HDA Codec Subsystem ID Verb-table =====
  //HDA Codec Subsystem ID  : 0x10251037
  0x00172037,
  0x00172110,
  0x00172225,
  0x00172310,

  //===== Pin Widget Verb-table =====
  //Pin widget 0x12
  0x01271CC0,
  0x01271D11,
  0x01271E11,
  0x01271F41,
  //Pin widget 0x14 - Speaker
  0x01471C20,
  0x01471D21,
  0x01471E17,
  0x01471F90,
  //Pin widget 0x17
  0x01771C00,
  0x01771D00,
  0x01771E00,
  0x01771F40,
  //Pin widget 0x18 - NC
  0x01871CF0,
  0x01871D11,
  0x01871E11,
  0x01871F41,
  //Pin widget 0x19 - NC
  0x01971CF0,
  0x01971D11,
  0x01971E11,
  0x01971F41,
  //Pin widget 0x1A - NC
  0x01A71CF0,
  0x01A71D11,
  0x01A71E11,
  0x01A71F41,
  //Pin widget 0x1B - NC
  0x01B71CF0,
  0x01B71D11,
  0x01B71E11,
  0x01B71F41,
  //Pin widget 0x1D
  0x01D71C01,
  0x01D71D00,
  0x01D71E70,
  0x01D71F40,
  //Pin widget 0x1E - NC
  0x01E71CF0,
  0x01E71D11,
  0x01E71E11,
  0x01E71F41,
  //Pin widget 0x21 - Headphone
  0x02171C30,
  0x02171D10,
  0x02171E21,
  0x02171F02,

  /* See data blob in "InstallPchHdaVerbTablePei" of vendor firmware
   * (some appear in https://github.com/torvalds/linux/blob/master/sound/pci/hda/patch_realtek.c).
   * - Largely coefficient programming (undocumented): Select coeff; write data
   * - Also programs speaker amplifier gain
   * - Sets speaker output
   * NOTE: NID 0x20 holds the "Realtek Defined Hidden registers" */
  0x02050038,  /* Set coeff idx: 0x38 */
  0x02048981,  /* Set processing coeff: 0x8981 */
  0x02050045,  /* Set coeff idx: 0x45 */
  0x0204c489,  /* Set processing coeff: 0xc489 */

  0x02050037,  /* Set coeff idx: 0x37 */
  0x02044a05,  /* Set processing coeff: 0x4a05 */
  0x05750003,  /* Set coeff idx on NID 0x57?: 0x3 */
  0x057486a6,  /* Set processing coeff on NID 0x57?: 0x86a6 */

  0x02050046,  /* Set coeff idx: 0x46 */
  0x02040004,  /* Set processing coeff: 0x4 */
  0x0205001b,  /* Set coeff idx: 0x1b */
  0x02040a0b,  /* Set processing coeff: 0xa0b */

  0x02050008,  /* Set coeff idx: 0x8 */
  0x02046a0c,  /* Set processing coeff: 0x6a0c */
  0x02050009,  /* Set coeff idx: 0x9 */
  0x0204e003,  /* Set processing coeff: 0xe003 */

  0x0205000a,  /* Set coeff idx: 0xa */
  0x02047770,  /* Set processing coeff: 0x7770 */
  0x02050040,  /* Set coeff idx: 0x40 */
  0x02049800,  /* Set processing coeff: 0x9800 */

  0x02050010,  /* Set coeff idx: 0x10 */
  0x02040e20,  /* Set processing coeff: 0xe20 */
  0x0205000d,  /* Set coeff idx: 0xd */
  0x02042801,  /* Set processing coeff: 0x2801 */

  0x0143b000,  /* Set amplifier gain on speaker: Set output, L+R amp; Unmuted; No gain */
  0x0143b000,  /* Repeated for units? */
  0x01470740,  /* Set widget control on speaker: Out enabled; VrefEn: Hi-Z (disabled) */
  0x01470740,  /* Repeated for units? */

  0x01470740,  /* Repeated for units? */
  0x01470740,  /* Repeated for units? */
  0x02050010,  /* Set coeff idx: 0x10 */
  0x02040f20   /* Set processing coeff: 0xf20 */
);

HDAUDIO_VERB_TABLE HdaVerbTableDisplayAudio = HDAUDIO_VERB_TABLE_INIT (
  //
  //  VerbTable: Intel Skylake HDMI
  //  Revision ID = 0xFF
  //  Codec Vendor: 0x80862809
  //  Subsystem ID: 0x80860101
  //
  0x8086, 0x2809,
  0xFF, 0xFF,

  // Codec Address: Bits 31:28
  // Node ID: Bits 27:20
  // Verb ID: Bits 19:8 / Bits 19:16
  // Payload: Bits 7:0  / Bits 15:0

  // NOTE: Corrected the table in vendor FW, codec address 0x2, not 0x0

  //
  // Display Audio Verb Table
  //
  // For GEN9, the Vendor Node ID is 08h
  // Enable the third converter and pin first
  0x20878101,
  0x20878101,
  0x20878101,
  0x20878101,
  // Pin Widget 5 - PORT B - Configuration Default: 0x18560010
  0x20571C10,
  0x20571D00,
  0x20571E56,
  0x20571F18,
  // Pin Widget 6 - PORT C - Configuration Default: 0x18560020
  0x20671C20,
  0x20671D00,
  0x20671E56,
  0x20671F18,
  // Pin Widget 7 - PORT D - Configuration Default: 0x18560030
  0x20771C30,
  0x20771D00,
  0x20771E56,
  0x20771F18,
  // Disable the third converter and third pin
  0x20878100,
  0x20878100,
  0x20878100,
  0x20878100
);

#endif // _ASPIRE_VN7_572G_HDA_VERB_TABLES_H_
