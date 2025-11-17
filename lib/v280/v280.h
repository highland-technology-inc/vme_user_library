/**
 * Public API for controlling the V280 48-Channel Digital Input Module.
 */

#pragma once

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdbool.h>

#include <V120.h>

/***************************************************************************************************
 * DEFINES
 **************************************************************************************************/

#define V280_CHANNEL_COUNT 48

/***************************************************************************************************
 * TYPES
 **************************************************************************************************/

/***************************************************************************************************
 * VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * FUNCTIONS
 **************************************************************************************************/

/**
 * Adds a VME region for the V280 module.
 * 
 * @param  hV120     Handle to the V120 library.
 * @param  vme_addr  Base address of the V280 module.
 * @param  addr_mode Addressing mode (A16, A24).
 * @param  name      Name of the VME region.
 * @return Pointer to the VME region, or NULL on failure.
 */
VME_REGION* v280_add_region(
  V120_HANDLE* restrict hV120, 
  const uint32_t vme_addr, 
  const V120_PD addr_mode, 
  const char* restrict name
);

/**
 * Deletes the VME region of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 */
void v280_delete_region(VME_REGION* restrict v280_region);

/***************************************************************************************************
 * V280 Overhead Information
 **************************************************************************************************/

/**
 * Gets the VXI Manufacturer ID of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  mfr         Pointer to store the Manufacturer ID 
 *                     (should be 0xFEEE for Highland Technology)
 * @return 0 on success, non-zero on failure.
 */
int v280_get_vxi_mfr(VME_REGION* restrict v280_region, uint16_t* restrict mfr);

/**
 * Gets the VXI Model Type of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  mtype       Pointer to store the VXI Model Type (should be 22280 for V280).
 * @return 0 on success, non-zero on failure.
 */
int v280_get_vxi_type(VME_REGION* restrict v280_region, uint16_t* restrict mtype);

/**
 * Gets the hardware revision of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param modrev      Pointer to store the Hardware revision.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_modrev(VME_REGION* restrict v280_region, uint16_t* restrict modrev);

/**
 * Gets the serial number of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  serial      Pointer to store the Serial number.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_serial(VME_REGION* restrict v280_region, uint16_t* restrict serial);

/**
 * Gets the Firmware ID of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  rom_id      Pointer to store the Firmware ID.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_rom_id(VME_REGION* restrict v280_region, uint16_t* restrict rom_id);

/**
 * Get the Firmware Revision of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  rom_rev     Pointer to store the Firmware Revision.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_rom_rev(VME_REGION* restrict v280_region, uint16_t* restrict rom_rev);

/**
 * Get the value of the 1 KHz realtime counter of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  mcount      Pointer to store the 1 KHz realtime counter value.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_mcount(VME_REGION* restrict v280_region, uint16_t* restrict mcount);

/**
 * Get the dash (module version) number of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  dash        Pointer to store the Dash (module version) number.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_dash(VME_REGION* restrict v280_region, uint16_t* restrict dash);

/**
 * Get the calibration ID of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  cal_id      Pointer to store the Calibration ID.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_cal_id(VME_REGION* restrict v280_region, uint16_t* restrict cal_id);

/**
 * Get the calibration year of the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  ycal        Pointer to store the Calibration year.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_ycal(VME_REGION* restrict v280_region, uint16_t* restrict ycal);

/**
 * Get the calibration month/day of the V280 module.
 * The month is stored in the high byte and the day in the low byte.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  dcal        Pointer to store the Calibration month/day.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_dcal(VME_REGION* restrict v280_region, uint16_t* restrict dcal);

/**
 * Get the calibration date of the V280 module as a string in the format "MM/DD/YYYY".
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  date        Storage buffer for the calibration date string.
 * @param  len         Length of the date string buffer.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_cal_date(VME_REGION* restrict v280_region, char* restrict date, size_t len);

/***************************************************************************************************
 * V280 User LED 
 **************************************************************************************************/

/**
 * Sets the User LED pattern on the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  pattern     16-bit pattern to set the User LED.
 * @return 0 on success, non-zero on failure.
 */
int v280_set_uled(VME_REGION* restrict v280_region, uint16_t pattern);

/**
 * Gets the User LED pattern from the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  pattern     Pointer to store the 16-bit pattern of the User LED.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_uled(VME_REGION* restrict v280_region, uint16_t* restrict pattern);

/***************************************************************************************************
 * V280 Input State and Debounce Times
 **************************************************************************************************/

/**
 * Get the input states of all channels.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  states      Pointer to store the 48-bit value representing the states of all channels.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_input_states(VME_REGION* restrict v280_region, uint64_t* restrict states);

/**
 * Set the rise time delay for the group of channels corresponding to the specified channel.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  channel     Channel number (0 - 47) to determine the group.
 * @param  delay       Rise time delay in units of 10 microseconds.
 * @return 0 on success, non-zero on failure.
 */
int v280_set_rise_time_delay(VME_REGION* restrict v280_region, uint8_t channel, uint16_t delay);

/**
 * Get the rise time delay for the group of channels corresponding to the specified channel.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  channel     Channel number (0 - 47) to determine the group.
 * @param  delay       Pointer to store the rise time delay in units of 10 microseconds.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_rise_time_delay(
  VME_REGION* restrict v280_region, 
  uint8_t channel, 
  uint16_t* restrict delay
);

/**
 * Set the fall time delay for the group of channels corresponding to the specified channel.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  channel     Channel number (0 - 47) to determine the group.
 * @param  delay       Fall time delay in units of 10 microseconds.
 * @return 0 on success, non-zero on failure.
 */
int v280_set_fall_time_delay(VME_REGION* restrict v280_region, uint8_t channel, uint16_t delay);

/**
 * Get the fall time delay for the group of channels corresponding to the specified channel.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  channel     Channel number (0 - 47) to determine the group.
 * @param  delay       Pointer to store the fall time delay in units of 10 microseconds.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_fall_time_delay(
  VME_REGION* restrict v280_region, 
  uint8_t channel, 
  uint16_t* restrict delay
);

/***************************************************************************************************
 * V280 BIST Error Flags
 **************************************************************************************************/

/**
 * Get the BIST error flags for all channels.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  error_flags Pointer to store the 48-bit value representing the BIST 
 *                     error flags of all channels.
 * @return 0 on success, non-zero on failure.
 */
int v280_get_bist_error_flags(VME_REGION* restrict v280_region, uint64_t* restrict error_flags);

/***************************************************************************************************
 * V280 Buffer Access
 **************************************************************************************************/

/**
 * Writes a value to the specified buffer index.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  index       Buffer index (0 - 127).
 * @param  value       Value to write to the buffer.
 * @return 0 on success, non-zero on failure.
 */
int v280_write_buffer(VME_REGION* restrict v280_region, uint8_t index, uint16_t value);

/**
 * Reads a value from the specified buffer index.
 * 
 * @param  v280_region VME region of the V280 module.
 * @param  index       Buffer index (0 - 127).
 * @param  value       Pointer to store the value read from the buffer.
 * @return 0 on success, non-zero on failure.
 */
int v280_read_buffer(VME_REGION* restrict v280_region, uint8_t index, uint16_t* restrict value);

/***************************************************************************************************
 * V280 Macro Control
 **************************************************************************************************/

/**
 * Runs the Built-In Self Test (BIST) on the V280 module.
 * 
 * @param  v280_region VME region of the V280 module.
 * @return 0 on success, -1 if macro busy, -2 if there was a BIST error.
 */
int v280_run_bist(VME_REGION* restrict v280_region);
