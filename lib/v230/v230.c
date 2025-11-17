/**
 * Implementation of the V230 64-Channel Analog Input Module Interface.
 */

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "v230.h"

/***************************************************************************************************
 * DEFINES
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

/** Convert millivolts to volts. */
#define V230_VOLTAGE_SCALE 0.001

#endif

#define V230_RNG1_SCALE_FACTOR (0.1024 / 32768.0)
#define V230_RNG2_SCALE_FACTOR (1.024 / 32768.0)
#define V230_RNG3_SCALE_FACTOR (10.24 / 32768.0)

/***************************************************************************************************
 * TYPES
 **************************************************************************************************/

/** V230 Channel Data Structure. */
typedef struct v230_channel_data_t {
  uint16_t config[V230_NUM_CHANNELS];
  int16_t rdata[V230_NUM_CHANNELS]; 
} v230_channel_data_t;

/***************************************************************************************************
 * VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * IMPLEMENTATION
 **************************************************************************************************/

/**
 * Validates the VME address and addressing mode for the V230 module.
 * 
 * @param  vme_addr  VME base address.
 * @param  addr_mode VME addressing mode.
 * @return 0 if valid, -1 if invalid.
 */
static int v230_validate_address_and_mode(const uint32_t vme_addr, const V120_PD addr_mode) {
  if ((addr_mode != V120_A16) && (addr_mode != V120_A24)) {
    printf("v230_validate_address_and_mode: Invalid address mode\n");
    return -1;
  }
  if ((addr_mode == V120_A16) && (vme_addr > 0xFE00)) {
    printf("v230_validate_address_and_mode: Invalid VME address 0x%08X for A16 mode\n", vme_addr);
    return -1;
  } else if ((addr_mode == V120_A24) && (vme_addr > 0xFFFE00)) {
    printf("v230_validate_address_and_mode: Invalid VME address 0x%08X for A24 mode\n", vme_addr);
    return -1;
  }
  return 0;
}

VME_REGION* v230_add_region(V120_HANDLE* restrict hV120, const uint32_t vme_addr, 
    const V120_PD addr_mode, const char* restrict name) {
  
  if (v230_validate_address_and_mode(vme_addr, addr_mode) < 0) return NULL;

  VME_REGION* v230_region = malloc(sizeof(VME_REGION));
  if (v230_region == NULL) {
    printf("v230_add_region: Failed to allocate memory for VME_REGION\n");
    return NULL;
  }

  v230_channel_data_t* channel_data = malloc(sizeof(v230_channel_data_t));
  if (channel_data == NULL) {
    printf("v230_add_region: Failed to allocate memory for channel data\n");
    free(v230_region);
    return NULL;
  }

  memset(v230_region, 0, sizeof(VME_REGION));
  memset(channel_data, 0, sizeof(v230_channel_data_t));

  v230_region->base = NULL;
  v230_region->start_page = v230_region->end_page = 0;
  v230_region->vme_addr = vme_addr;
  v230_region->len = sizeof(v230_registers);
  v230_region->config = addr_mode | V120_SMAX | V120_EAUTO | V120_RW | V120_D16;
  v230_region->tag = name;
  v230_region->udata = (void *)channel_data;

  VME_REGION* data = v120_add_vme_region(hV120, v230_region);
  if (data == NULL) {
    printf("v230_add_region: Failed to add VME region\n");
    v230_delete_region(v230_region);
    return NULL;
  }

  return v230_region;
}

void v230_delete_region(VME_REGION* restrict v230_region) {
  if (v230_region->udata != NULL) {
    free(v230_region->udata);
    v230_region->udata = NULL;
  }
  if (v230_region != NULL) free(v230_region);
}

/**
 * Gets a pointer to the V230 registers from the VME region.
 * 
 * @param  v230_region VME region of the V230 module.
 * @return Pointer to the V230 registers.
 */
static inline volatile v230_registers* v230_get_registers(VME_REGION* restrict v230_region) {
  return (v230_registers *)v230_region->base;
}

/***************************************************************************************************
 * V230 Test Registers
 **************************************************************************************************/

int v230_get_htest(VME_REGION* restrict v230_region, uint16_t* restrict value) {
  if (v230_region == NULL) return -1;
  *value = v230_get_registers(v230_region)->htest;
  return 0;
}

int v230_set_utest(VME_REGION* restrict v230_region, uint16_t value) {
  if (v230_region == NULL) return -1;
  v230_get_registers(v230_region)->utest = value;
  return 0;
}

int v230_get_utest(VME_REGION* restrict v230_region, uint16_t* restrict value) {
  if (v230_region == NULL) return -1;
  *value = v230_get_registers(v230_region)->utest;
  return 0;
}

/***************************************************************************************************
 * V230 Overhead Information
 **************************************************************************************************/

int v230_get_vxi_mfr(VME_REGION* restrict v230_region, uint16_t* restrict mfr) {
  if (v230_region == NULL) return -1; 
  *mfr = v230_get_registers(v230_region)->vxi_mfr;
  return 0;
}

int v230_get_vxi_type(VME_REGION* restrict v230_region, uint16_t* restrict type) {
  if (v230_region == NULL) return -1; 
  *type = v230_get_registers(v230_region)->vxi_type;
  return 0;
}

int v230_get_serial_number(VME_REGION* restrict v230_region, uint16_t* restrict serial) {
  if (v230_region == NULL) return -1; 
  *serial = v230_get_registers(v230_region)->serial;
  return 0;
}

int v230_get_rom_id(VME_REGION* restrict v230_region, uint16_t* restrict rom_id) {
  if (v230_region == NULL) return -1; 
  *rom_id = v230_get_registers(v230_region)->rom_id;
  return 0;
}

int v230_get_rom_rev(VME_REGION* restrict v230_region, uint16_t* restrict rom_rev) {
  if (v230_region == NULL) return -1; 
  *rom_rev = v230_get_registers(v230_region)->rom_rev;
  return 0;
}

int v230_get_mcount(VME_REGION* restrict v230_region, uint16_t* restrict mcount) {
  if (v230_region == NULL) return -1; 
  *mcount = v230_get_registers(v230_region)->mcount;
  return 0;
}

int v230_get_dash_number(VME_REGION* restrict v230_region, uint16_t* restrict dash) {
  if (v230_region == NULL) return -1; 
  *dash = v230_get_registers(v230_region)->dash;
  return 0;
}

int v230_get_scan_count(VME_REGION* restrict v230_region, uint16_t* restrict scan_count) {
  if (v230_region == NULL) return -1; 
  *scan_count = v230_get_registers(v230_region)->scan;
  return 0;
}

/***************************************************************************************************
 * V230 Calibration Information
 **************************************************************************************************/

int v230_get_cal_id(VME_REGION* restrict v230_region, uint16_t* restrict cal_id) {
  if (v230_region == NULL) return -1; 
  *cal_id = v230_get_registers(v230_region)->calid;
  return 0;
}

int v230_get_ycal(VME_REGION* restrict v230_region, uint16_t* restrict ycal) {
  if (v230_region == NULL) return -1; 
  *ycal = v230_get_registers(v230_region)->ycal;
  return 0;
}

int v230_get_dcal(VME_REGION* restrict v230_region, uint16_t* restrict dcal) {
  if (v230_region == NULL) return -1; 
  *dcal = v230_get_registers(v230_region)->dcal;
  return 0;
}

int v230_get_cal_date(VME_REGION* restrict v230_region, char* restrict date, size_t length) {
  if (v230_region == NULL || date == NULL) return -1; 
  uint16_t year, month_day;
  if (v230_get_ycal(v230_region, &year)) return -1;
  if (v230_get_dcal(v230_region, &month_day)) return -1;
  uint8_t month = (month_day >> 8) & 0xFF;
  uint8_t day = month_day & 0xFF;
  snprintf(date, length, "%d/%d/%d", (int)month, (int)day, (int)year);
  return 0;
}

/***************************************************************************************************
 * V230 User LED 
 **************************************************************************************************/

int v230_set_uled(VME_REGION* restrict v230_region, uint16_t pattern) {
  if (v230_region == NULL) return -1; 
  v230_get_registers(v230_region)->uled = pattern;
  return 0;
}

int v230_get_uled(VME_REGION* restrict v230_region, uint16_t* restrict pattern) {
  if (v230_region == NULL) return -1; 
  *pattern = v230_get_registers(v230_region)->uled;
  return 0;
}

/***************************************************************************************************
 * V230 Channel Information
 **************************************************************************************************/

int v230_set_channel_config(VME_REGION* restrict v230_region, uint16_t channel, 
    v230_channel_config_t config) {
  if (v230_region == NULL || channel >= V230_NUM_CHANNELS) return -1;
  uint16_t ctl_reg = (config.filter & V230_CHANNEL_FILTER_MASK) | 
                     (config.range & V230_CHANNEL_RANGE_MASK);
  if (config.enable) ctl_reg |= V230_BIT_CHANNEL_ENABLE;
  v230_get_registers(v230_region)->ctl[channel] = ctl_reg;
  return 0;
}

int v230_get_channel_config(VME_REGION* restrict v230_region, uint16_t channel, 
    v230_channel_config_t* restrict config) {
  if (v230_region == NULL || channel >= V230_NUM_CHANNELS) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);
  config->range = (v230_channel_range_t)(regs->ctl[channel] & V230_CHANNEL_RANGE_MASK);
  config->filter = (v230_channel_filter_t)(regs->ctl[channel] & V230_CHANNEL_FILTER_MASK);
  config->enable = (regs->ctl[channel] & V230_BIT_CHANNEL_ENABLE) != 0;
  return 0;
}

int v230_get_channel_setup_error_id(VME_REGION* restrict v230_region, uint16_t* restrict channel) {
  if (v230_region == NULL) return -1;
  *channel = v230_get_registers(v230_region)->cher;
  return 0;
}

/***************************************************************************************************
 * V230 Realtime Channel Data
 **************************************************************************************************/

/**
 * Performs a DMA transfer to read channel configuration & raw voltage data from the V230 module.
 * 
 * @param  hV120        Handle to the V120 library.
 * @param  v230_region  VME region of the V230 module.
 * @return 0 on success, -1 on failure.
 */
static int v230_dma_xfr(V120_HANDLE* restrict hV120, VME_REGION* restrict v230_region) {
  struct v120_dma_desc_t desc = {
    .flags = V120_PD_A16 | V120_PD_D16 | V120_PD_ESHORT,
    .ptr = (__u64)v230_region->udata,
    .size = sizeof(v230_channel_data_t),
    .next = 0LL,
    .vme_address = v230_region->vme_addr + offsetof(v230_registers, ctl),
  };
  if (v120_dma_xfr(hV120, &desc) < 0) return -1;
  return 0;
}

int v230_get_all_channel_voltages(V120_HANDLE* restrict hV120, VME_REGION* restrict v230_region, 
  v230_channel_voltage_t* restrict chan_voltages) {
  if (hV120 == NULL || v230_region == NULL) return -1;
  if (v230_dma_xfr(hV120, v230_region) < 0) return -1;

  v230_channel_data_t* v230_udata = (v230_channel_data_t*)v230_region->udata;
  
  for (int ch = 0; ch < V230_NUM_CHANNELS; ch++) {
    switch (v230_udata->config[ch] & V230_CHANNEL_RANGE_MASK) {
      case V230_CHANNEL_RANGE_1:
        chan_voltages->voltage[ch] = (int16_t)(v230_udata->rdata[ch]) * V230_RNG1_SCALE_FACTOR;
        break;
      case V230_CHANNEL_RANGE_2:
        chan_voltages->voltage[ch] = (int16_t)(v230_udata->rdata[ch]) * V230_RNG2_SCALE_FACTOR;
        break;
      case V230_CHANNEL_RANGE_3:
        chan_voltages->voltage[ch] = (int16_t)(v230_udata->rdata[ch]) * V230_RNG3_SCALE_FACTOR;
        break;
      default: return -1;
    }
  }

  return 0;
}

/***************************************************************************************************
 * V230 Macro Control
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

int v230_execute_macro_channel_test(VME_REGION* restrict v230_region, uint16_t channel) {
  if (v230_region == NULL || channel >= V230_NUM_CHANNELS) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);
  if ((regs->macro & V230_BIT_MACRO_BUSY) != 0) return -1;
  regs->mp[0] = channel;
  regs->macro = V230_MACRO_CHANNEL_TEST;
  return 0;
}

/**
 * Executes the full BIST macro on the V230 module.
 * NOTE: This function initiates the macro but does not wait for its completion.
 * 
 * @param  v230_region VME region of the V230 module.
 * @return 0 on success, non-zero on failure.
 */
static int v230_run_full_bist(VME_REGION* restrict v230_region) {
  if (v230_region == NULL) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);
  if ((regs->macro & V230_BIT_MACRO_BUSY) != 0) return -1;
  regs->macro = V230_MACRO_FULL_BIST;
  return 0;
}

#endif

/**
 * Executes the reboot macro on the V230 module.
 * NOTE: This function initiates the macro and waits for its completion.
 * 
 * @param  v230_region VME region of the V230 module.
 * @return 0 on success, non-zero on failure.
 */
static int v230_reboot(VME_REGION* restrict v230_region) {
  if (v230_region == NULL) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);
  if ((regs->macro & V230_BIT_MACRO_BUSY) != 0) return -1;
  regs->macro = V230_MACRO_REBOOT;
  /** Wait for reboot to complete. */
  sleep(6); 
  return 0;
}

/**
 * Executes the power supply test macro on the V230 module.
 * NOTE: This function initiates the macro but does not wait for its completion.
 * 
 * @param  v230_region VME region of the V230 module.
 * @return 0 on success, non-zero on failure.
 */
static int v230_run_ps_test(VME_REGION* restrict v230_region) {
  if (v230_region == NULL) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);
  if ((regs->macro & V230_BIT_MACRO_BUSY) != 0) return -1;
  regs->macro = V230_MACRO_PS_TEST;
  return 0;
}

int v230_execute_macro(VME_REGION* restrict v230_region, v230_macro_code_t macro_code) {
  if (v230_region == NULL) return -1;
  switch (macro_code) {
    case V230_MACRO_NO_OP:        return 0;
    case V230_MACRO_REBOOT:       return v230_reboot(v230_region);
    case V230_MACRO_PS_TEST:      return v230_run_ps_test(v230_region);
#if defined(V230_2) || defined(V230_21)
    case V230_MACRO_FULL_BIST:    return v230_run_full_bist(v230_region);
    case V230_MACRO_CHANNEL_TEST:
#endif
    default:                      return -1;
  }
}

int v230_is_macro_busy(VME_REGION* restrict v230_region, bool* restrict is_busy) {
  if (v230_region == NULL) return -1;
  *is_busy = (v230_get_registers(v230_region)->macro & V230_BIT_MACRO_BUSY) != 0;
  return 0;
}

/***************************************************************************************************
 * V230 Module Control 
 **************************************************************************************************/

int v230_set_scan_speed_slow(VME_REGION* restrict v230_region) {
  if (v230_region == NULL) return -1; 
  v230_get_registers(v230_region)->mode |= V230_BIT_MODE_SLOW;
  return 0;
}

int v230_set_scan_speed_fast(VME_REGION* restrict v230_region) {
  if (v230_region == NULL) return -1; 
  v230_get_registers(v230_region)->mode &= ~V230_BIT_MODE_SLOW;
  return 0;
}

int v230_is_scan_speed_fast(VME_REGION* restrict v230_region, bool *restrict is_fast) {
  if (v230_region == NULL) return -1;
  *is_fast = (v230_get_registers(v230_region)->mode & V230_BIT_MODE_SLOW) == 0;
  return 0;
}

#if defined(V230_2) || defined(V230_21)

int v230_set_mode(VME_REGION* restrict v230_region, v230_mode_t mode) {
  if (v230_region == NULL) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);
  regs->mode &= ~(V230_MODE_MASK);
  regs->mode |= (uint16_t)mode & V230_MODE_MASK;
  return 0;
}

int v230_get_mode(VME_REGION* restrict v230_region, v230_mode_t* restrict mode) {
  if (v230_region == NULL) return -1;
  *mode = (v230_mode_t)(v230_get_registers(v230_region)->mode & V230_MODE_MASK);
  return 0;
}

#endif

/***************************************************************************************************
 * V230 Relay Control
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

int v230_set_relay_config(VME_REGION* restrict v230_region, v230_relay_config_t config) {
  if ((v230_region == NULL) || (config.channel >= V230_NUM_CHANNELS)) return -1;
  uint16_t relay_reg = config.channel;
  if (config.c_relay) relay_reg |= V230_BIT_RELAY_C;
  for (int relay = 0; relay < V230_B_RELAY_NUM; relay++) {
    if (config.b_relays[relay]) relay_reg |= V230_BIT_RELAY_B(relay);
  }
  v230_get_registers(v230_region)->relays = relay_reg;
  return 0;
}

int v230_get_relay_config(VME_REGION* restrict v230_region, v230_relay_config_t* restrict config) {
  if (v230_region == NULL) return -1;
  uint16_t relay_reg = v230_get_registers(v230_region)->relays;
  config->channel = (relay_reg & V230_RELAY_K_MASK);
  config->c_relay = (relay_reg & V230_BIT_RELAY_C) != 0;
  for (int relay = 0; relay < V230_B_RELAY_NUM; relay++) {
    config->b_relays[relay] = (relay_reg & V230_BIT_RELAY_B(relay)) != 0;
  }
  return 0;
}

#endif

/***************************************************************************************************
 * V230 Built-In Self Test (BIST) Information
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

int v230_get_full_bist_channel_results(VME_REGION* restrict v230_region, 
    v230_full_bist_channel_results_t* restrict results) {
  if (v230_region == NULL) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);
  for (int ch = 0; ch < V230_NUM_CHANNELS; ch++) {
    uint16_t bist_reg = regs->bist[ch / 2];
    if ((ch % 2) == 0) bist_reg = (bist_reg >> 8) & 0xFF; 
    results->channel_flags[ch].cer = (bist_reg & V230_BIT_BIST_CER) != 0;
    results->channel_flags[ch].ner = (bist_reg & V230_BIT_BIST_NER) != 0;
    results->channel_flags[ch].per = (bist_reg & V230_BIT_BIST_PER) != 0;
    results->channel_flags[ch].zer = (bist_reg & V230_BIT_BIST_ZER) != 0;
    for (int range = 0; range < V230_BIST_RNG_NUM; range++) {
      results->channel_flags[ch].rng[range] = (bist_reg & V230_BIT_BIST_RNG(range)) != 0;
    }
  }
  return 0;
}

int v230_get_bist_error_count(VME_REGION* restrict v230_region, uint16_t* restrict error_count) {
  if (v230_region == NULL) return -1;
  *error_count = v230_get_registers(v230_region)->bern;
  return 0;
}

int v230_get_single_channel_bist_results(VME_REGION* restrict v230_region, 
    v230_single_channel_bist_results_t* restrict results) {
  if (v230_region == NULL) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);

  uint16_t bist_reg = regs->bist[0];
  results->channel_flags.cer = (bist_reg & V230_BIT_BIST_CER) != 0;
  results->channel_flags.ner = (bist_reg & V230_BIT_BIST_NER) != 0;
  results->channel_flags.per = (bist_reg & V230_BIT_BIST_PER) != 0;
  results->channel_flags.zer = (bist_reg & V230_BIT_BIST_ZER) != 0;
  for (int range = 0; range < V230_BIST_RNG_NUM; range++) {
    results->channel_flags.rng[range] = (bist_reg & V230_BIT_BIST_RNG(range)) != 0;
  }

  for (int mindex = 0; mindex < V230_SINGLE_CHANNEL_BIST_MEASUREMENTS; mindex++) {
    results->measurements[mindex] = (int16_t)regs->bist[mindex + 1];
  }

  return 0;
}

int v230_set_bmux_config(VME_REGION* restrict v230_region, v230_bmux_t config) {
  if (v230_region == NULL) return -1;
  v230_get_registers(v230_region)->bmux = V230_BIT_BMUX_HIGH((uint16_t)config.cal_pos) | 
      V230_BIT_BMUX_LOW((uint16_t)config.cal_neg);
  return 0;
}

int v230_get_bmux_config(VME_REGION* restrict v230_region, v230_bmux_t* restrict config) {
  if (v230_region == NULL) return -1;
  uint16_t bmux_reg = v230_get_registers(v230_region)->bmux;
  config->cal_pos = (v230_bmux_source_t)((bmux_reg & V230_BMUX_MASK_HIGH) >> V230_BMUX_SHIFT_HIGH);
  config->cal_neg = (v230_bmux_source_t)(bmux_reg & V230_BMUX_MASK_LOW);
  return 0;
}

int v230_get_all_power_supply_status(VME_REGION* restrict v230_region, 
    v230_ps_all_status_t* restrict status) {
  if (v230_region == NULL) return -1;
  volatile v230_registers* regs = v230_get_registers(v230_region);

  status->ep1.error = (regs->perr & V230_BIT_PERR_P1) != 0;
  status->ep1.voltage = (int16_t)regs->ep1 * V230_VOLTAGE_SCALE;

  status->ep2.error = (regs->perr & V230_BIT_PERR_P2) != 0;
  status->ep2.voltage = (int16_t)regs->ep2 * V230_VOLTAGE_SCALE;

  status->ep2_5.error = (regs->perr & V230_BIT_PERR_P2_5) != 0;
  status->ep2_5.voltage = (int16_t)regs->ep2_5 * V230_VOLTAGE_SCALE;

  status->ep3.error = (regs->perr & V230_BIT_PERR_P3) != 0;
  status->ep3.voltage = (int16_t)regs->ep3 * V230_VOLTAGE_SCALE;

  status->ep5.error = (regs->perr & V230_BIT_PERR_P5) != 0;
  status->ep5.voltage = (int16_t)regs->ep5 * V230_VOLTAGE_SCALE;

  status->ep15.error = (regs->perr & V230_BIT_PERR_P15) != 0;
  status->ep15.voltage = (int16_t)regs->ep15 * V230_VOLTAGE_SCALE;

  status->em15.error = (regs->perr & V230_BIT_PERR_M15) != 0;
  status->em15.voltage = (int16_t)regs->em15 * V230_VOLTAGE_SCALE;
  return 0;
}

#endif
