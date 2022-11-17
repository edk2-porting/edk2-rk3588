/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MEMTYPELIB_H_
#define _MEMTYPELIB_H_

typedef enum {
  MemTypeNone = 0,
  MemType1lmDdr,
  MemType1lmAppDirect,
  MemType1lmAppDirectReserved,
  MemType1lmCtrl,
  MemType1lmHbm,
  MemTypeNxm,
  MemType2lmDdrCacheMemoryMode,
  MemType2lmDdrWbCacheAppDirect,
  MemType2lmHbmCacheDdr,
  MemType2lmHbmCacheMemoryMode,
  MemTypeCxlAccVolatileMem,
  MemTypeCxlAccPersistentMem,
  MemTypeFpga,
  MemTypeCxlExpVolatileMem,
  MemTypeCxlExpPersistentMem,
  MemTypeMax
} MEM_TYPE;

#endif // _MEMTYPELIB_H_
