/**
 * V280 Register Map
 */

#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/** V280 Register Map. */
typedef struct __attribute__((packed)) v280_registers {
  /** Registers 0 - 11 */
  uint16_t vxi_mfr;       /** 0x000 - VXI Manufacturer ID - 65262 (0xFEEE) */
  uint16_t vxi_type;      /** 0x002 - VXI Model Type - 22280 (0x5708) */
  uint16_t modrev;        /** 0x004 - Hardware Revision */
  uint16_t serial;        /** 0x006 - Unit Serial Number */
  uint16_t rom_id;        /** 0x008 - Firmware ID */
  uint16_t rom_rev;       /** 0x00A - Firmware Revision */
  uint16_t mcount;        /** 0x00C - 1 KHz realtime counter */
  uint16_t dash;          /** 0x00E - Module version (DASH) number */
  uint16_t calid;         /** 0x010 - Calibration table status */
  uint16_t ycal;          /** 0x012 - Calibration date: year */
  uint16_t dcal;          /** 0x014 - Calibration date: month/day */
  uint16_t dummy016;      /** 0x016 - Unused */

  /** Register 12 - 15 */
  uint16_t uled;          /** 0x018 - User LED control */
  uint16_t dummy01A[3];   /** 0x01A - 0x01E - Unused */

  /** Registers 16 - 23 */
  uint16_t macro;         /** 0x020 - Macro control register */
  uint16_t mp[4];         /** 0x022 - 0x028 - Macro parameters 0 - 3 */
  uint16_t dummy02A[3];   /** 0x02A - 0x02E - Unused */

  /** Registers 24 - 26 */
  uint16_t state[3];      /** 0x030 - 0x034 - Input states 0 - 47 */
  uint16_t dummy036;      /** 0x036 - Unused */

  /** Registers 28 - 31 */
  uint16_t rise[3];       /** 0x038 - 0x03C - Input rise debounce times 0 - 47 */
  uint16_t dummy03E;      /** 0x03E - Unused */

  /** Registers 32 - 35 */
  uint16_t fall[3];       /** 0x040 - 0x044 - Input fall debounce times 0 - 47 */
  uint16_t dummy046;      /** 0x046 - Unused */

  /** Registers 36 - 127 */
  uint16_t err[3];        /** 0x048 - 0x04C - BIST error flags 0 - 47 */
  uint16_t dummy04E[89];  /** 0x04E - 0x0FE - Unused */

  /** Registers 128 - 255 */
  uint16_t buf[128];      /** 0x100 - 0x1FE - Read/Write buffer 0 - 127 */
} v280_registers;

/** Register Map Sanity Checks. */
static_assert(offsetof(v280_registers, uled) == 0x18);
static_assert(offsetof(v280_registers, macro) == 0x20);
static_assert(offsetof(v280_registers, state) == 0x30);
static_assert(offsetof(v280_registers, rise) == 0x38);
static_assert(offsetof(v280_registers, fall) == 0x40);
static_assert(offsetof(v280_registers, err) == 0x48);
static_assert(offsetof(v280_registers, buf) == 0x100);
static_assert(sizeof(v280_registers) == 0x200);
