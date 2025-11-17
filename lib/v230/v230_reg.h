/** V230 Register Map and Bit Definitions. */

#pragma once

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

/***************************************************************************************************
 * TYPES
 **************************************************************************************************/

/** V230 Register Map. */
typedef struct __attribute__((packed)) v230_registers {
  /** Registers 0 - 2 */
  uint16_t vxi_mfr;       /** 0x000 - VXI Manufacturer ID - 65262 (0xFEEE) */
  uint16_t vxi_type;      /** 0x002 - VXI Model Type - 22230 (0x56D6) */
  uint16_t dummy004;      /** 0x004 - Unused */

  /** Registers 3 - 10 */
  uint16_t serial;        /** 0x006 - Unit Serial Number */
  uint16_t rom_id;        /** 0x008 - Firmware ID */
  uint16_t rom_rev;       /** 0x00A - Firmware Revision */
  uint16_t mcount;        /** 0x00C - Microprocessor IRQ update counter */
  uint16_t dash;          /** 0x00E - Module version (DASH) number */
  uint16_t scan;          /** 0x010 - ADC scan counter # */
  uint16_t dummy012[2];   /** 0x012 - 0x014 - Unused */

  /** Registers 11 - 15 */
  uint16_t relays;        /** 0x016 - Control for optional cal-bus relays */
  uint16_t uled;          /** 0x018 - User LED control */
  uint16_t mode;          /** 0x01A - Module operating mode */
  uint16_t calid;         /** 0x01C - Calibration table status */
  uint16_t cher;          /** 0x01E - Channel configuration error ID */
  
  /** Registers 16 - 19 */
  uint16_t macro;         /** 0x020 - Macro command register */
  uint16_t mp[3];         /** 0x022 - 0x026 - Macro parameters 0 - 2 */

  /** Registers 20 - 63 */
  uint16_t ycal;          /** 0x028 - Calibration date: year */
  uint16_t dcal;          /** 0x02A - Calibration date: month/day */
  uint16_t bern;          /** 0x02C - Optional BIST error count */
  uint16_t bmux;          /** 0x02E - Optional BIST mux control */
  uint16_t dummy030[40];  /** 0x030 - 0x07E - Unused */

  /** Registers 64 - 127 */
  uint16_t ctl[64];       /** 0x080 - 0x0FE - Channel control 0 - 63 */

  /** Registers 128 - 191 */
  uint16_t rdat[64];      /** 0x100 - 0x17E - Channel 0 - 63 realtime data */

  /** Registers 192 - 239 */
  uint16_t bist[32];      /** 0x180 - 0x1BE - BIST reporting region */
  uint16_t dummy1C0[16];  /** 0x1C0 - 0x1DE - Unused */

  /** Registers 240 - 253 */
  uint16_t perr;         /** 0x1E0 - Power supply error flags */
  uint16_t ep1;          /** 0x1E2 - +1.2V power supply voltage */
  uint16_t ep2;          /** 0x1E4 - +2.048V power supply voltage */
  uint16_t ep2_5;        /** 0x1E6 - +2.5V power supply voltage */
  uint16_t ep3;          /** 0x1E8 - +3.3V power supply voltage */
  uint16_t ep5;          /** 0x1EA - +5V power supply voltage */
  uint16_t ep15;         /** 0x1EC - +15V power supply voltage */
  uint16_t em15;         /** 0x1EE - -15V power supply voltage */
  uint16_t dummy1F0[6];  /** 0x1F0 - 0x1FA - Unused */

  /** Registers 254 - 255 */
  uint16_t utest;        /** 0x1FC - User read/write test register */
  uint16_t htest;        /** 0x1FE - Hardware test register (reads 0xABCD) */
} v230_registers;

/** Register Map Sanity Checks. */
static_assert(offsetof(v230_registers, serial) == 0x006);
static_assert(offsetof(v230_registers, relays) == 0x016);
static_assert(offsetof(v230_registers, macro) == 0x020);
static_assert(offsetof(v230_registers, ycal) == 0x028);
static_assert(offsetof(v230_registers, ctl) == 0x080);
static_assert(offsetof(v230_registers, rdat) == 0x100);
static_assert(offsetof(v230_registers, bist) == 0x180);
static_assert(offsetof(v230_registers, perr) == 0x1E0);
static_assert(offsetof(v230_registers, utest) == 0x1FC);
static_assert(sizeof(v230_registers) == 0x200);

/***************************************************************************************************
 * V230 RELAYS Register Bits
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

#define V230_RELAY_K_MASK (0x3F)
#define V230_BIT_RELAY_C (1 << 7)
#define V230_BIT_RELAY_B(n) (1 << (8 + (n)))

#endif

/***************************************************************************************************
 * V230 Module Control (MODE) Register Bits
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

#define V230_MODE_MASK (0x3)
#define V230_BIT_MODE(code) ((code) & V230_MODE_MASK)

#endif

#define V230_BIT_MODE_SLOW (1 << 8)

/***************************************************************************************************
 * V230 MACRO Register Bits
 **************************************************************************************************/

#define V230_BIT_MACRO_BUSY (1 << 15)

/***************************************************************************************************
 * V230 BIST Mux Control Register Bits
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

#define V230_BMUX_MASK_LOW (0x7)
#define V230_BIT_BMUX_LOW(code) ((code) & V230_BMUX_MASK_LOW)

#define V230_BMUX_SHIFT_HIGH 4
#define V230_BMUX_MASK_HIGH (0x7 << V230_BMUX_SHIFT_HIGH)
#define V230_BIT_BMUX_HIGH(code) (((code) << V230_BMUX_SHIFT_HIGH) & V230_BMUX_MASK_HIGH)

#endif

/*************************************************************************************************** 
 * V230 Channel Control (CTL) Register Bits
 **************************************************************************************************/

#define V230_CHANNEL_RANGE_MASK (0x3)
#define V230_BIT_CHANNEL_RANGE(code) ((code) & V230_CHANNEL_RANGE_MASK)

#define V230_CHANNEL_FILTER_MASK (0x3 << 4)
#define V230_BIT_CHANNEL_FILTER(code) ((code << 4) & V230_CHANNEL_FILTER_MASK)

#define V230_BIT_CHANNEL_ENABLE (1 << 8)

/***************************************************************************************************
 * V230 BIST Reporting Register Bits
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

#define V230_BIT_BIST_RNG(range) (1 << (range))

#define V230_BIT_BIST_ZER (1 << 4)
#define V230_BIT_BIST_PER (1 << 5)
#define V230_BIT_BIST_NER (1 << 6)
#define V230_BIT_BIST_CER (1 << 7)

#endif

/***************************************************************************************************
 * V230 Power Supply Error (PERR) Register Bits
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

#define V230_BIT_PERR_P1 (1 << 0)
#define V230_BIT_PERR_P2 (1 << 1)
#define V230_BIT_PERR_P2_5 (1 << 2)
#define V230_BIT_PERR_P3 (1 << 3)
#define V230_BIT_PERR_P5 (1 << 4)
#define V230_BIT_PERR_P15 (1 << 5)
#define V230_BIT_PERR_M15 (1 << 6)

#endif
