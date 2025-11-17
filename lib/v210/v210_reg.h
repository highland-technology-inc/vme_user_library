/** V210 Register Map and Bit Definitions. */

#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/** V210 Register Map. */
typedef struct __attribute__((packed)) v210_registers {
  /** Registers 0 - 1 */
  uint16_t bdid;          /** 0x00 - Board ID - 6192 (0x1B00) */
  uint16_t csr;           /** 0x02 - Relay Driver & Front Panel LED Control */

  /** Registers 2 - 7 */
  uint16_t vxi_mfr;       /** 0x04 - VXI Manufacturer ID - 65262 (0xFEEE) */
  uint16_t vxi_type;      /** 0x06 - VXI Model Type - 22210 (0x56C2) */
  uint16_t fpga_rev;      /** 0x08 - FPGA Revision - 0x0041 ('A') */
  uint16_t _dummy0A[3];   /** 0x0A - 0x00E - Unused */

  /** Registers 8 - 11 */
  uint16_t ctl[4];        /** 0x10 - Control Relays 63 - 0 */

  /** Registers 12-15 */
  uint16_t rcon[4];       /** 0x18 - Relay Contacts of Relays 63 - 0  */
} v210_registers;

/** Register Map Sanity Checks. */
static_assert(offsetof(v210_registers, vxi_mfr) == 0x04);
static_assert(offsetof(v210_registers, ctl) == 0x10);
static_assert(offsetof(v210_registers, rcon) == 0x18);
static_assert(sizeof(v210_registers) == 0x20);

/***************************************************************************************************
 * Control & Status Register (CSR) Bits
 **************************************************************************************************/

#define V210_CSR_ERR_LED (1 << 15)
#define V210_CSR_P4TM (1 << 14)
#define V210_CSR_P3TM (1 << 13)
