/**
 * Implementation of the V280 48-Channel Digital Input Module Interface.
 */

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "v280.h"
#include "v280_reg.h"

/***************************************************************************************************
 * DEFINES
 **************************************************************************************************/

#define V280_CHANNELS_PER_REGISTER 16

#define V280_BIST_MACRO_CODE 0x8401

/***************************************************************************************************
 * TYPES
 **************************************************************************************************/

/***************************************************************************************************
 * VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * IMPLEMENTATION
 **************************************************************************************************/

/**
 * Validates the VME address and addressing mode for the V280 module.
 * 
 * @param  vme_addr  VME base address.
 * @param  addr_mode VME addressing mode.
 * @return 0 if valid, -1 if invalid.
 */
static int v280_validate_address_and_mode(const uint32_t vme_addr, const V120_PD addr_mode) {
  if ((addr_mode != V120_A16) && (addr_mode != V120_A24)) {
    printf("v280_validate_address_and_mode: Invalid address mode\n");
    return -1;
  }
  if ((addr_mode == V120_A16) && (vme_addr > 0xFE00)) {
    printf("v280_validate_address_and_mode: Invalid VME address 0x%08X for A16 mode\n", vme_addr);
    return -1;
  } else if ((addr_mode == V120_A24) && (vme_addr > 0xFFFE00)) {
    printf("v280_validate_address_and_mode: Invalid VME address 0x%08X for A24 mode\n", vme_addr);
    return -1;
  }
  return 0;
}

VME_REGION* v280_add_region(V120_HANDLE* restrict hV120, const uint32_t vme_addr, 
    const V120_PD addr_mode, const char* restrict name) {

  if (v280_validate_address_and_mode(vme_addr, addr_mode) < 0) return NULL;

  VME_REGION* v280_region = malloc(sizeof(VME_REGION));
  if (v280_region == NULL) {
    printf("v280_add_region: Failed to allocate memory for VME_REGION\n");
    return NULL;
  }

  memset(v280_region, 0, sizeof(VME_REGION));
  v280_region->base = NULL;
  v280_region->start_page = v280_region->end_page = 0;
  v280_region->vme_addr = vme_addr;
  v280_region->len = sizeof(v280_registers);
  v280_region->config = addr_mode | V120_SMAX | V120_EAUTO | V120_RW | V120_D16;
  v280_region->tag = name;
  v280_region->udata = NULL;

  VME_REGION* data = v120_add_vme_region(hV120, v280_region);
  if (data == NULL) {
    printf("v280_add_region: Failed to add VME region\n");
    free(v280_region);
    return NULL;
  }

  return v280_region;
}

void v280_delete_region(VME_REGION* restrict v280_region) {
  if (v280_region != NULL) free(v280_region);
}

/**
 * Gets a pointer to the V280 registers from the VME region.
 * 
 * @param  v280_region VME region of the V280 module.
 * @return Pointer to the V280 registers.
 */
static inline volatile v280_registers* v280_get_registers(VME_REGION* restrict v280_region) {
  return (v280_registers *)v280_region->base;
}

/***************************************************************************************************
 * V280 Overhead Information
 **************************************************************************************************/

int v280_get_vxi_mfr(VME_REGION* restrict v280_region, uint16_t* restrict mfr) {
  if (v280_region == NULL) return -1;
  *mfr = v280_get_registers(v280_region)->vxi_mfr;
  return 0;
}

int v280_get_vxi_type(VME_REGION* restrict v280_region, uint16_t* restrict type) {
  if (v280_region == NULL) return -1;
  *type = v280_get_registers(v280_region)->vxi_type;
  return 0;
}

int v280_get_modrev(VME_REGION* restrict v280_region, uint16_t* restrict modrev) {
  if (v280_region == NULL) return -1;
  *modrev = v280_get_registers(v280_region)->modrev;
  return 0;
}

int v280_get_serial(VME_REGION* restrict v280_region, uint16_t* restrict serial) {
  if (v280_region == NULL) return -1;
  *serial = v280_get_registers(v280_region)->serial;
  return 0;
}

int v280_get_rom_id(VME_REGION* restrict v280_region, uint16_t* restrict rom_id) {
  if (v280_region == NULL) return -1;
  *rom_id = v280_get_registers(v280_region)->rom_id;
  return 0;
}

int v280_get_rom_rev(VME_REGION* restrict v280_region, uint16_t* restrict rom_rev) {
  if (v280_region == NULL) return -1;
  *rom_rev = v280_get_registers(v280_region)->rom_rev;
  return 0;
}

int v280_get_mcount(VME_REGION* restrict v280_region, uint16_t* restrict mcount) {
  if (v280_region == NULL) return -1;
  *mcount = v280_get_registers(v280_region)->mcount;
  return 0;
}

int v280_get_dash(VME_REGION* restrict v280_region, uint16_t* restrict dash) {
  if (v280_region == NULL) return -1;
  *dash = v280_get_registers(v280_region)->dash;
  return 0;
}

int v280_get_cal_id(VME_REGION* restrict v280_region, uint16_t* restrict cal_id) {
  if (v280_region == NULL) return -1;
  *cal_id = v280_get_registers(v280_region)->calid;
  return 0;
}

int v280_get_ycal(VME_REGION* restrict v280_region, uint16_t* restrict ycal) {
  if (v280_region == NULL) return -1;
  *ycal = v280_get_registers(v280_region)->ycal;
  return 0;
}

int v280_get_dcal(VME_REGION* restrict v280_region, uint16_t* restrict dcal) {
  if (v280_region == NULL) return -1;
  *dcal = v280_get_registers(v280_region)->dcal;
  return 0;
}

int v280_get_cal_date(VME_REGION* restrict v280_region, char* restrict date, size_t len) {
  if (v280_region == NULL || date == NULL) return -1;
  uint16_t year, month_day;
  if (v280_get_ycal(v280_region, &year)) return -1;
  if (v280_get_dcal(v280_region, &month_day)) return -1;
  uint8_t month = (month_day >> 8) & 0xFF;
  uint8_t day = (month_day & 0xFF);
  snprintf(date, len, "%d/%d/%d", (int)month, (int)day, (int)year);
  return 0;
}

/***************************************************************************************************
 * V280 User LED 
 **************************************************************************************************/

int v280_set_uled(VME_REGION* restrict v280_region, uint16_t pattern) {
  if (v280_region == NULL) return -1;
  v280_get_registers(v280_region)->uled = pattern;
  return 0;
}

int v280_get_uled(VME_REGION* restrict v280_region, uint16_t* restrict pattern) {
  if (v280_region == NULL) return -1;
  *pattern = v280_get_registers(v280_region)->uled;
  return 0;
}

/***************************************************************************************************
 * V280 Input State and Debounce Times
 **************************************************************************************************/

int v280_get_input_states(VME_REGION* restrict v280_region, uint64_t* restrict states) {
  if (v280_region == NULL) return -1;
  volatile v280_registers* regs = v280_get_registers(v280_region);
  *states = 0;
  for (uint8_t i = 0; i < 3; i++) {
    *states <<= V280_CHANNELS_PER_REGISTER;
    *states |= regs->state[i];
  }
  return 0;
}

int v280_set_rise_time_delay(VME_REGION* restrict v280_region, uint8_t channel, uint16_t delay) {
  if (v280_region == NULL) return -1;
  if (channel >= V280_CHANNEL_COUNT) return -1;
  uint8_t reg = channel / V280_CHANNELS_PER_REGISTER;
  v280_get_registers(v280_region)->rise[reg] = delay;
  return 0;
}

int v280_get_rise_time_delay(VME_REGION* restrict v280_region, uint8_t channel, 
    uint16_t* restrict delay) {
  if (v280_region == NULL) return -1;
  if (channel >= V280_CHANNEL_COUNT) return -1;
  uint8_t reg = channel / V280_CHANNELS_PER_REGISTER;
  *delay = v280_get_registers(v280_region)->rise[reg];
  return 0;
}

int v280_set_fall_time_delay(VME_REGION* restrict v280_region, uint8_t channel, uint16_t delay) {
  if (v280_region == NULL) return -1;
  if (channel >= V280_CHANNEL_COUNT) return -1;
  uint8_t reg = channel / V280_CHANNELS_PER_REGISTER;
  v280_get_registers(v280_region)->fall[reg] = delay;
  return 0;
}

int v280_get_fall_time_delay(VME_REGION* restrict v280_region, uint8_t channel, 
    uint16_t* restrict delay) {
  if (v280_region == NULL) return -1;
  if (channel >= V280_CHANNEL_COUNT) return -1;
  uint8_t reg = channel / V280_CHANNELS_PER_REGISTER;
  *delay = v280_get_registers(v280_region)->fall[reg];
  return 0;
}

/***************************************************************************************************
 * V280 BIST Error Flags
 **************************************************************************************************/

int v280_get_bist_error_flags(VME_REGION* restrict v280_region, 
    uint64_t* restrict error_flags) {
  if (v280_region == NULL) return -1;
  volatile v280_registers* regs = v280_get_registers(v280_region);
  *error_flags = 0;
  for (uint8_t i = 0; i < 3; i++) {
    *error_flags <<= V280_CHANNELS_PER_REGISTER;
    *error_flags |= regs->err[i];
  }
  return 0;
}

/***************************************************************************************************
 * V280 Buffer Access
 **************************************************************************************************/

int v280_write_buffer(VME_REGION* restrict v280_region, uint8_t index, uint16_t value) {
  if (v280_region == NULL) return -1;
  v280_get_registers(v280_region)->buf[index] = value;
  return 0;
}

int v280_read_buffer(VME_REGION* restrict v280_region, uint8_t index, uint16_t* restrict value) {
  if (v280_region == NULL) return -1;
  *value = v280_get_registers(v280_region)->buf[index];
  return 0;
}

/***************************************************************************************************
 * V280 Macro Control
 **************************************************************************************************/

int v280_run_bist(VME_REGION* restrict v280_region) {
  if (v280_region == NULL) return -1;
  volatile v280_registers* regs = v280_get_registers(v280_region);
  if ((regs->macro >> 15) & 0x01) return -1;

  regs->macro = V280_BIST_MACRO_CODE;
  /** This should resolve within 250 useconds. */
  while ((regs->macro >> 15) & 0x1);

  if (((regs->macro >> 8) & 0xFF) != 0) return -2;
  return 0;
}
