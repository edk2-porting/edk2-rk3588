#ifndef _UBOOT_ENV_H
#define _UBOOT_ENV_H

#include "errno.h"

#define __maybe_unused

typedef UINT8 u8;
typedef UINT32 u32;
typedef UINT64 u64;
typedef unsigned int uint;
typedef UINTN ulong;
typedef BOOLEAN bool;

#define true TRUE
#define false FALSE

#define udelay(x)	MicroSecondDelay (x)

#define dev_err(dev, args...)  	DEBUG ((DEBUG_ERROR, args))
#define dev_info(dev, args...)  DEBUG ((DEBUG_INFO, args))
#define dev_dbg(dev, args...) 	DEBUG ((DEBUG_INFO, args))
#define printf(args...)		DEBUG ((DEBUG_INFO, args))

#define BITS_PER_LONG 	(sizeof(UINTN) * 8)
#define GENMASK(h, l) \
  (((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define BIT(x)	(1 << (x))

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

#define FIELD_PREP(_mask, _val)						\
	({								\
		((typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask);	\
	})

#define FIELD_GET(_mask, _reg)						\
	({								\
		(typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask));	\
	})

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

#endif // _UBOOT_ENV_H
