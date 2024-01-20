#ifndef _UBOOT_ENV_H
#define _UBOOT_ENV_H

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Soc.h>
#include "errno.h"

#define __maybe_unused

typedef UINT8 u8;
typedef UINT16 u16;
typedef UINT32 u32;
typedef UINT64 u64;
typedef unsigned int uint;
typedef UINTN ulong;
typedef INTN ssize_t;
typedef UINTN size_t;
typedef BOOLEAN bool;

#define true TRUE
#define false FALSE

#define udelay(x)	MicroSecondDelay (x)
#define mdelay(x) 	MicroSecondDelay (x * 1000)

#define dev_err(dev, args...)  	DEBUG ((DEBUG_ERROR, args))
#define dev_info(dev, args...)  DEBUG ((DEBUG_INFO, args))
#define dev_dbg(dev, args...) 	DEBUG ((DEBUG_INFO, args))
#define printf(args...)		DEBUG ((DEBUG_INFO, args))
#define debug(args...)		DEBUG ((DEBUG_INFO, args))
#define WARN(x, args...)	DEBUG ((DEBUG_WARN, args))

#define BITS_PER_LONG 	(sizeof(UINTN) * 8)
#define GENMASK(h, l) \
  (((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

#define FIELD_PREP(_mask, _val)						\
	({								\
		((typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask);	\
	})

#define FIELD_GET(_mask, _reg)						\
	({								\
		(typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask));	\
	})

#define min(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);	\
	_x < _y ? _x : _y; })

#define max(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);	\
	_x > _y ? _x : _y; })

#define min_t(type, a, b) min(((type) a), ((type) b))
#define max_t(type, a, b) max(((type) a), ((type) b))

#define regmap_read_poll_timeout(map, addr, val, cond, sleep_us, \
				      timeout_ms) \
({ \
	int __ret; \
	for (;;) { \
		__ret = regmap_read((map), (addr), &(val)); \
		if (__ret) \
			break; \
		if (cond) \
			break; \
		if ((sleep_us)) \
			udelay((sleep_us)); \
	} \
	__ret ?: ((cond) ? 0 : -ETIMEDOUT); \
})

static inline int regmap_write(UINTN map, uint offset, uint val)
{
	MmioWrite32 (map + offset, val);
	return 0;
}

static inline int regmap_read(UINTN map, uint offset, uint *valp)
{
	*valp = MmioRead32 (map + offset);
	return 0;
}

static inline int regmap_update_bits(UINTN map, uint offset, uint mask, uint val)
{
	uint reg;
	int ret;

	ret = regmap_read(map, offset, &reg);
	if (ret)
		return ret;

	reg &= ~mask;

	return regmap_write(map, offset, reg | (val & mask));
}

struct reset_ctl {
	unsigned int	offset;
	unsigned int	bit;
};

static inline void reset(const struct reset_ctl *ctl, bool en)
{
	u32 val;
	val = (en << ctl->bit) | ((1 << ctl->bit) << 16);
	regmap_write(CRU_BASE, ctl->offset, val);
}

#define reset_assert(ctl)  reset(ctl, true)
#define reset_deassert(ctl)  reset(ctl, false)

static inline u32 __get_unaligned_le32(const u8 *p)
{
	return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}

static inline u32 get_unaligned_le32(const void *p)
{
	return __get_unaligned_le32((const u8 *)p);
}

#define memcpy(dest, source, count)         CopyMem(dest,source, (UINTN)(count))
#define memset(dest, ch, count)             SetMem(dest, (UINTN)(count),(UINT8)(ch))
#define memchr(buf, ch, count)              ScanMem8(buf, (UINTN)(count),(UINT8)ch)
#define memcmp(buf1, buf2, count)           (int)(CompareMem(buf1, buf2, (UINTN)(count)))
#define memmove(dest, source, count)        CopyMem(dest, source, (UINTN)(count))
#define strlen(str)                         (size_t)(AsciiStrLen(str))
#define strnlen(str, count)                 (size_t)(AsciiStrnLenS(str, count))
#define strncpy(strDest, strSource, count)  AsciiStrnCpyS(strDest, MAX_STRING_SIZE, strSource, (UINTN)count)
#define strcat(strDest, strSource)          AsciiStrCatS(strDest, MAX_STRING_SIZE, strSource)
#define strcmp(string1, string2, count)     (int)(AsciiStrCmp(string1, string2))
#define strncmp(string1, string2, count)    (int)(AsciiStrnCmp(string1, string2, (UINTN)(count)))

#endif // _UBOOT_ENV_H
