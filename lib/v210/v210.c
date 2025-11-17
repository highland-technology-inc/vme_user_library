/**
 * Implementation of the V210 64-Channel SPDT Relay Module Interface.
 */

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "v210.h"
#include "v210_reg.h"

/***************************************************************************************************
 * DEFINES
 **************************************************************************************************/

#define V210_CHANNELS_PER_REGISTER 16

/***************************************************************************************************
 * TYPES
 **************************************************************************************************/

/***************************************************************************************************
 * VARIABLES
 **************************************************************************************************/

/** Relay Settling Time */
struct timespec relay_settling_time = {0, 10 * 1000000L};

/***************************************************************************************************
 * IMPLEMENTATION
 **************************************************************************************************/

/**
 * Validates the VME address and addressing mode for the V210 module.
 * 
 * @param  addr_mode VME addressing mode.
 * @param  vme_addr  VME base address.
 * @return 0 if valid, -1 if invalid.
 */
static int v210_validate_address_and_mode(const V120_PD addr_mode, const uint32_t vme_addr) {
  if ((addr_mode != V120_A16) && (addr_mode != V120_A24)) {
    printf("v210_validate_address_and_mode: Invalid address mode\n");
    return -1;
  }
  if ((addr_mode == V120_A16) && (vme_addr > 0xFFE0)) {
    printf("v210_validate_address_and_mode: Invalid VME address 0x%08X for A16 mode\n", vme_addr);
    return -1;
  } else if ((addr_mode == V120_A24) && (vme_addr > 0xFFFFE0)) {
    printf("v210_validate_address_and_mode: Invalid VME address 0x%08X for A24 mode\n", vme_addr);
    return -1;
  }
  return 0;
}

VME_REGION* v210_add_region(V120_HANDLE* restrict hV120, const uint32_t vme_addr, 
    const V120_PD addr_mode, const char* restrict name) {

  if (v210_validate_address_and_mode(addr_mode, vme_addr) < 0) return NULL;
  
  VME_REGION* v210_region = malloc(sizeof(VME_REGION));
  if (v210_region == NULL) {
    printf("v210_add_region: Failed to allocate memory for VME_REGION\n");
    return NULL;
  }

  memset(v210_region, 0, sizeof(VME_REGION));
  v210_region->base = NULL;
  v210_region->start_page = v210_region->end_page = 0;
  v210_region->vme_addr = vme_addr;
  v210_region->len = sizeof(v210_registers);
  v210_region->config = addr_mode | V120_SMAX | V120_EAUTO | V120_RW | V120_D16;
  v210_region->tag = name;
  v210_region->udata = NULL;

  VME_REGION* data = v120_add_vme_region(hV120, v210_region);
  if (data == NULL) {
    printf("v210_add_region: Failed to add VME region\n");
    free(v210_region);
    return NULL;
  }

  return v210_region;
}

int v210_print_region_info(VME_REGION* v210_region) {
  if (v210_region == NULL) return -1;
	printf("v210_region.next = %p\n", v210_region->next);
	printf("v210_region.base = %p\n", v210_region->base);
	printf("v210_region.start_page = %u\n", v210_region->start_page);
	printf("v210_region.end_page = %u\n", v210_region->end_page);
	printf("v210_region.vme_addr = 0x%08lX\n", v210_region->vme_addr);
	printf("v210_region.len = %zu\n", v210_region->len);
	printf("v210_region.config = 0x%08lX\n", v210_region->config);
	printf("v210_region.tag = %s\n", v210_region->tag);
	printf("v210_region.udata = %p\n", v210_region->udata);
  return 0;
}

void v210_delete_region(VME_REGION* restrict v210_region) {
  if (v210_region != NULL) free(v210_region);
}

/**
 * Gets a pointer to the V210 registers from the VME region.
 * 
 * @param  v210_region VME region of the V210 module.
 * @return Pointer to the V210 registers.
 */
static inline volatile v210_registers* v210_get_registers(VME_REGION* restrict v210_region) {
  return (v210_registers *)v210_region->base;
}

/***************************************************************************************************
 * V210 Overhead Information
 **************************************************************************************************/

int v210_get_board_id(VME_REGION* restrict v210_region, uint16_t* id) {
  if (v210_region == NULL) return -1;
  *id = v210_get_registers(v210_region)->bdid;
  return 0;
}

int v210_get_vxi_mfr(VME_REGION* restrict v210_region, uint16_t* mfr) {
  if (v210_region == NULL) return -1;
  *mfr = v210_get_registers(v210_region)->vxi_mfr;
  return 0;
}

int v210_get_vxi_type(VME_REGION* restrict v210_region, uint16_t* type) {
  if (v210_region == NULL) return -1;
  *type = v210_get_registers(v210_region)->vxi_type;
  return 0;
}

int v210_get_fpga_rev(VME_REGION* restrict v210_region, uint16_t* rev) {
  if (v210_region == NULL) return -1;
  *rev = v210_get_registers(v210_region)->fpga_rev;
  return 0;
}

/***************************************************************************************************
 * V210 Error LED 
 **************************************************************************************************/

int v210_turn_off_error_led(VME_REGION* restrict v210_region) {
  if (v210_region == NULL) return -1;
  v210_get_registers(v210_region)->csr |= V210_CSR_ERR_LED;
  return 0;
}

int v210_turn_on_error_led(VME_REGION* restrict v210_region) {
  if (v210_region == NULL) return -1;
  v210_get_registers(v210_region)->csr &= ~V210_CSR_ERR_LED;
  return 0;
}

int v210_is_error_led_on(VME_REGION* restrict v210_region, bool* is_on) {
  if (v210_region == NULL) return -1;
  *is_on = (v210_get_registers(v210_region)->csr & V210_CSR_ERR_LED) == 0;
  return 0;
}

/***************************************************************************************************
 * V210 Relay Drivers 
 **************************************************************************************************/

int v210_enable_relay_drivers(VME_REGION* restrict v210_region) {
  if (v210_region == NULL) return -1;
  v210_get_registers(v210_region)->csr |= (V210_CSR_P4TM | V210_CSR_P3TM);
  return 0;
}

int v210_disable_relay_drivers(VME_REGION* restrict v210_region) {
  if (v210_region == NULL) return -1;
  v210_get_registers(v210_region)->csr &= ~(V210_CSR_P4TM | V210_CSR_P3TM);
  return 0;
}

int v210_is_p4tm_driver_enabled(VME_REGION* restrict v210_region, bool* is_enabled) {
  if (v210_region == NULL) return -1;
  *is_enabled = (v210_get_registers(v210_region)->csr & V210_CSR_P4TM) != 0;
  return 0;
}

int v210_is_p3tm_driver_enabled(VME_REGION* restrict v210_region, bool* is_enabled) {
  if (v210_region == NULL) return -1;
  *is_enabled = (v210_get_registers(v210_region)->csr & V210_CSR_P3TM) != 0;
  return 0;
}

/***************************************************************************************************
 * V210 Relay Control 
 **************************************************************************************************/

int v210_set_relays(VME_REGION* restrict v210_region, uint64_t channel_mask) {
  if (v210_region == NULL) return -1;
  volatile v210_registers* regs = v210_get_registers(v210_region);
  for (int8_t i = 0; i < 4; i++) {
    regs->ctl[3 - i] = (uint16_t)(channel_mask & 0xFFFF);
    channel_mask >>= 16;
  }
  return 0;
}

int v210_get_relays(VME_REGION* restrict v210_region, bool let_relays_settle, uint64_t* mask) {
  if (v210_region == NULL) return -1;
  if (let_relays_settle) nanosleep(&relay_settling_time, NULL);
  uint64_t channel_mask = 0;
  volatile v210_registers* regs = v210_get_registers(v210_region);
  for (int8_t i = 0; i < 4; i++) {
    channel_mask <<= 16;
    channel_mask |= (regs->ctl[i] & 0xFFFF);
  }
  *mask = channel_mask;
  return 0;
}
