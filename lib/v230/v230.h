/**
 * Public API for controlling the V230 64-Channel Analog Input Module.
 */

#pragma once

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdbool.h>

#include <V120.h>

#include "v230_reg.h"

/***************************************************************************************************
 * DEFINES
 **************************************************************************************************/

#define V230_NUM_CHANNELS 64

#if defined(V230_2) || defined(V230_21)

#define V230_B_RELAY_NUM 8

#define V230_BIST_RNG_NUM 3

#define V230_SINGLE_CHANNEL_BIST_MEASUREMENTS 15

#endif

/***************************************************************************************************
 * TYPES
 **************************************************************************************************/

/***************************************************************************************************
 * V230 Channel Information
 **************************************************************************************************/

/** V230 Channel Input Range Enumeration. */
typedef enum v230_channel_range_t {
  V230_CHANNEL_RANGE_1 = V230_BIT_CHANNEL_RANGE(1),
  V230_CHANNEL_RANGE_2 = V230_BIT_CHANNEL_RANGE(2),
  V230_CHANNEL_RANGE_3 = V230_BIT_CHANNEL_RANGE(3),
} v230_channel_range_t;


/** V230 Channel Filter Enumeration. */
typedef enum v230_channel_filter_t {
  V230_CHANNEL_FILTER_NONE  = V230_BIT_CHANNEL_FILTER(0),
  V230_CHANNEL_FILTER_200HZ = V230_BIT_CHANNEL_FILTER(1),
  V230_CHANNEL_FILTER_17HZ  = V230_BIT_CHANNEL_FILTER(2),
} v230_channel_filter_t;

/** V230 Channel Configuration. */
typedef struct v230_channel_config_t {
  v230_channel_range_t range;
  v230_channel_filter_t filter;
  bool enable;
} v230_channel_config_t;

/** V230 Channel Voltages For All Channels. */
typedef struct v230_channel_voltage_t {
  float voltage[V230_NUM_CHANNELS];
} v230_channel_voltage_t;

/***************************************************************************************************
 * V230 Realtime Channel Data
 **************************************************************************************************/

/***************************************************************************************************
 * V230 Macro Control
 **************************************************************************************************/

/** V230 Macro Codes Enumeration. */
typedef enum v230_macro_code_t {
  V230_MACRO_NO_OP        = 0x8400,
  V230_MACRO_REBOOT       = 0x8407,
  V230_MACRO_PS_TEST      = 0x8409,
#if defined(V230_2) || defined(V230_21)
  V230_MACRO_FULL_BIST    = 0x8401,
  V230_MACRO_CHANNEL_TEST = 0x8408,
#endif
} v230_macro_code_t;

/***************************************************************************************************
 * V230 Module Control 
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

/** V230 Modes Enumeration. */
typedef enum v230_mode_t {
  V230_MODE_OFF       = V230_BIT_MODE(0),
  V230_MODE_CHAN_TEST = V230_BIT_MODE(1),
  V230_MODE_BIST      = V230_BIT_MODE(2),
  V230_MODE_BOTH      = V230_BIT_MODE(3),
} v230_mode_t;

#endif

/***************************************************************************************************
 * V230 Relay Control
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

/** V230 Relay Configuration. */
typedef struct v230_relay_config_t {
  uint16_t channel;  /** Sets the k relay based on channel number 0-63. */
  bool c_relay;
  bool b_relays[V230_B_RELAY_NUM];
} v230_relay_config_t;

#endif

/***************************************************************************************************
 * V230 Built-In Self Test (BIST) Information
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

/** V230 BIST Flags. */
typedef struct v230_bist_flags_t {
  bool cer;
  bool ner;
  bool per;
  bool zer;
  bool rng[V230_BIST_RNG_NUM];
} v230_bist_flags_t;

/** V230 Full BIST Channel Results For All Channels. */
typedef struct v230_full_bist_channel_results_t {
  v230_bist_flags_t channel_flags[V230_NUM_CHANNELS];
} v230_full_bist_channel_results_t;

/** V230 Single Channel BIST Results.*/
typedef struct v230_single_channel_bist_results_t {
  v230_bist_flags_t channel_flags;
  int16_t measurements[V230_SINGLE_CHANNEL_BIST_MEASUREMENTS];
} v230_single_channel_bist_results_t;

/** V230 BMUX Source Enumeration. */
typedef enum v230_bmux_source_t {
  V230_BMUX_SRC_0 = 0,
  V230_BMUX_SRC_1 = 1,
  V230_BMUX_SRC_2 = 2,
  V230_BMUX_SRC_3 = 3,
  V230_BMUX_SRC_4 = 4,
  V230_BMUX_SRC_5 = 5,
  V230_BMUX_SRC_6 = 6,
  V230_BMUX_SRC_7 = 7,
} v230_bmux_source_t;

/** V230 BMUX Configuration. */
typedef struct v230_bmux_t {
  v230_bmux_source_t cal_pos;
  v230_bmux_source_t cal_neg;
} v230_bmux_t;

/** V230 Power Supply Status. */
typedef struct v230_ps_status_t {
  bool error;
  float voltage;
} v230_ps_status_t;

/** V230 Power Supply Statuses For All Power Supplies. */
typedef struct v230_ps_all_status_t {
  v230_ps_status_t ep1;
  v230_ps_status_t ep2;
  v230_ps_status_t ep2_5;
  v230_ps_status_t ep3;
  v230_ps_status_t ep5;
  v230_ps_status_t ep15;
  v230_ps_status_t em15;
} v230_ps_all_status_t;

#endif

/***************************************************************************************************
 * VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * FUNCTIONS
 **************************************************************************************************/

/**
 * Adds a VME region for the V230 module.
 * 
 * @param  hV120     Handle to the V120 library.
 * @param  vme_addr  Base address of the V230 module.
 * @param  addr_mode Addressing mode (A16, A24).
 * @param  name      Name of the VME region.
 * @return Pointer to the VME region, or NULL on failure.
 */
VME_REGION* v230_add_region(
  V120_HANDLE* restrict hV120, 
  const uint32_t vme_addr, 
  const V120_PD addr_mode, 
  const char* restrict name
);

/**
 * Deletes the VME region of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 */
void v230_delete_region(VME_REGION* restrict v230_region);

/***************************************************************************************************
 * V230 Test Registers
 **************************************************************************************************/

/**
 * Gets the value of the Hardware test register on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  value       Pointer to store the value of the Hardware test register.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_htest(VME_REGION* restrict v230_region, uint16_t* restrict value);

/**
 * Sets the User test register on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  value       Value to set the User test register.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_utest(VME_REGION* restrict v230_region, uint16_t value);

/**
 * Gets the value of the User test register on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  value       Pointer to store the value of the User test register.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_utest(VME_REGION* restrict v230_region, uint16_t* restrict value);

/***************************************************************************************************
 * V230 Overhead Information
 **************************************************************************************************/

/**
 * Get the VXI Manufacturer ID of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  mfr         Pointer to store the Manufacturer ID (0xFEEE for Highland Technology).
 * @return 0 on success, non-zero on failure.
 */
int v230_get_vxi_mfr(VME_REGION* restrict v230_region, uint16_t* restrict mfr);

/**
 * Gets the VXI Model Type of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  type        Pointer to store the Model Type.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_vxi_type(VME_REGION* restrict v230_region, uint16_t* restrict type);

/**
 * Gets the serial number of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  serial      Pointer to store the serial number.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_serial_number(VME_REGION* restrict v230_region, uint16_t* restrict serial);

/**
 * Gets the Firmware ID of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  rom_id      Pointer to store the Firmware ID.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_rom_id(VME_REGION* restrict v230_region, uint16_t* restrict rom_id);

/**
 * Gets the Firmware Revision of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  rom_rev     Pointer to store the Firmware Revision.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_rom_rev(VME_REGION* restrict v230_region, uint16_t* restrict rom_rev);

/**
 * Gets the value of the Microprocessor IRQ update counter.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  mcount      Pointer to store the Microprocessor IRQ update counter value.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_mcount(VME_REGION* restrict v230_region, uint16_t* restrict mcount);

/**
 * Gets the dash (module version) number of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  dash        Pointer to store the Dash number.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_dash_number(VME_REGION* restrict v230_region, uint16_t* restrict dash);

/**
 * Gets the value of the ADC scan counter.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  scan_count  Pointer to store the ADC scan counter value.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_scan_count(VME_REGION* restrict v230_region, uint16_t* restrict scan_count);

/***************************************************************************************************
 * V230 Calibration Information
 **************************************************************************************************/

/**
 * Gets the calibration ID of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  cal_id      Pointer to store the Calibration ID.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_cal_id(VME_REGION* restrict v230_region, uint16_t* restrict cal_id);

/**
 * Gets the calibration year of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  ycal        Pointer to store the Calibration year.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_ycal(VME_REGION* restrict v230_region, uint16_t* restrict ycal);

/**
 * Gets the calibration month/day of the V230 module.
 * The month is stored in the high byte and the day in the low byte.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  dcal        Pointer to store the Calibration month/day.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_dcal(VME_REGION* restrict v230_region, uint16_t* restrict dcal);

/**
 * Get the calibration date of the V230 module as a string in the format "MM/DD/YYYY".
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  date        Pointer to store the calibration date string.
 * @param  length      Length of the date string buffer.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_cal_date(VME_REGION* restrict v230_region, char* restrict date, size_t length);

/***************************************************************************************************
 * V280 User LED 
 **************************************************************************************************/

/**
 * Sets the User LED pattern on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  pattern     LED pattern to set.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_uled(VME_REGION* restrict v230_region, uint16_t pattern);

/**
 * Gets the User LED pattern from the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  pattern     Pointer to store the LED pattern.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_uled(VME_REGION* restrict v230_region, uint16_t* restrict pattern);

/***************************************************************************************************
 * V230 Channel Information
 **************************************************************************************************/

/**
 * Sets the configuration of a specific channel on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  channel     Channel number to configure (0 to 63).
 * @param  config      Configuration settings for the channel.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_channel_config(
  VME_REGION* restrict v230_region, 
  uint16_t channel, 
  v230_channel_config_t config
);

/**
 * Gets the configuration of a specific channel on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  channel     Channel number to query (0 to 63).
 * @param  config      Pointer to store the configuration settings of the channel.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_channel_config(
  VME_REGION* restrict v230_region, 
  uint16_t channel, 
  v230_channel_config_t* restrict config
);

/**
 * Gets the channel number of the first channel with a setup error.
 * If no channels have setup errors, the channel number is set to 0xFFFF.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  channel     Pointer to store the channel number.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_channel_setup_error_id(VME_REGION* restrict v230_region, uint16_t* restrict channel);

/***************************************************************************************************
 * V230 Realtime Channel Data
 **************************************************************************************************/

/**
 * Get the voltages of all channels on the V230 module.
 * 
 * @param  hV120       Handle to the V120 library.
 * @param  v230_region VME region of the V230 module.
 * @param  voltages    Pointer to store the voltages of all channels.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_all_channel_voltages(
  V120_HANDLE* restrict hV120, 
  VME_REGION* restrict v230_region, 
  v230_channel_voltage_t* restrict voltages
);

/***************************************************************************************************
 * V230 Macro Control
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

/**
 * Executes the single channel test macro on the V230 module for the specified channel.
 * NOTE: This function initiates the macro but does not wait for its completion.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  channel     Channel number to test (0 to 63).
 * @return 0 on success, non-zero on failure.
 */
int v230_execute_macro_channel_test(VME_REGION* restrict v230_region, uint16_t channel);

#endif

/**
 * Executes the specified macro on the V230 module.
 * NOTE: Attemping to execute V230_MACRO_CHANNEL_TEST will result in an error, use 
 *       v230_execute_macro_channel_test() instead.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  macro_code  Macro code to execute.
 * @return 0 on success, non-zero on failure.
 */
int v230_execute_macro(VME_REGION* restrict v230_region, v230_macro_code_t macro_code);

/**
 * Checks if the macro on the V230 module is busy.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  is_busy     Pointer to store whether the macro is busy (true) or not (false).
 * @return 0 on success, non-zero on failure.
 */
int v230_is_macro_busy(VME_REGION* restrict v230_region, bool* restrict is_busy);

/***************************************************************************************************
 * V230 Module Control 
 **************************************************************************************************/

/**
 * Sets the scan speed of the V230 module to slow.
 * 
 * @param  v230_region VME region of the V230 module.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_scan_speed_slow(VME_REGION* restrict v230_region);

/**
 * Sets the scan speed of the V230 module to fast.
 * 
 * @param  v230_region VME region of the V230 module.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_scan_speed_fast(VME_REGION* restrict v230_region);

/**
 * Checks if the scan speed of the V230 module is set to fast.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  is_fast     Pointer to store whether the scan speed is fast (true) or slow (false).
 */
int v230_is_scan_speed_fast(VME_REGION* restrict v230_region, bool *restrict is_fast);

#if defined(V230_2) || defined(V230_21)

/**
 * Sets the operating mode of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  mode        Operating mode to set.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_mode(VME_REGION* restrict v230_region, v230_mode_t mode);

/**
 * Gets the operating mode of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  mode        Pointer to store the operating mode.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_mode(VME_REGION* restrict v230_region, v230_mode_t* restrict mode);

#endif

/***************************************************************************************************
 * V230 Relay Control
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

/**
 * Sets the relay configuration on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  config      Relay configuration to set.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_relay_config(VME_REGION* restrict v230_region, v230_relay_config_t config);

/**
 * Gets the relay configuration from the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  config      Pointer to store the relay configuration.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_relay_config(VME_REGION* restrict v230_region, v230_relay_config_t* restrict config);

#endif

/***************************************************************************************************
 * V230 Built-In Self Test (BIST) Information
 **************************************************************************************************/

#if defined(V230_2) || defined(V230_21)

/**
 * Gets the full BIST channel results for all channels on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  results     Pointer to store the full BIST channel results.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_full_bist_channel_results(
  VME_REGION* restrict v230_region, 
  v230_full_bist_channel_results_t* restrict results
);

/**
 * Gets the BIST error count of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  error_count Pointer to store the BIST error count.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_bist_error_count(VME_REGION* restrict v230_region, uint16_t* restrict error_count);

/**
 * Gets the single channel BIST results of the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  results     Pointer to store the single channel BIST results.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_single_channel_bist_results(
  VME_REGION* restrict v230_region, 
  v230_single_channel_bist_results_t* restrict results
);

/**
 * Sets the BMUX configuration on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  config      BMUX configuration to set.
 * @return 0 on success, non-zero on failure.
 */
int v230_set_bmux_config(VME_REGION* restrict v230_region, v230_bmux_t config);

/**
 * Gets the BMUX configuration from the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  config      Pointer to store the BMUX configuration.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_bmux_config(VME_REGION* restrict v230_region, v230_bmux_t* restrict config);

/** 
 * Gets the power supply status of all power supplies on the V230 module.
 * 
 * @param  v230_region VME region of the V230 module.
 * @param  status      Pointer to store the power supply statuses.
 * @return 0 on success, non-zero on failure.
 */
int v230_get_all_power_supply_status(
  VME_REGION* restrict v230_region, 
  v230_ps_all_status_t* restrict status
);

#endif
