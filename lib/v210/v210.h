/**
 * Public API for controlling the V210 64-Channel SPDT Relay Module. 
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

#define V210_CHANNEL_COUNT 64

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
 * Adds a VME region for the V210 module.
 * 
 * @param  hV120     Handle to the V120 library.
 * @param  vme_addr  Base address of the V210 module.
 * @param  addr_mode Addressing mode (A16, A24).
 * @param  name      Name of the VME region.
 * @return Pointer to the VME region, or NULL on failure.
 */
VME_REGION* v210_add_region(
  V120_HANDLE* restrict hV120, 
  const uint32_t vme_addr, 
  const V120_PD addr_mode, 
  const char* restrict name
);

/**
 * Prints the information of the VME region for the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @return 0 on success, non-zero on failure.
 */
int v210_print_region_info(VME_REGION* v210_region);

/**
 * Deletes the VME region of the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 */
void v210_delete_region(VME_REGION* restrict v210_region);

/***************************************************************************************************
 * V210 Overhead Information
 **************************************************************************************************/

/**
 * Gets the board ID of the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param id           Storage for the board ID (should be 0x1B00).
 * @return 0 on success, non-zero on failure.
 */
int v210_get_board_id(VME_REGION* restrict v210_region, uint16_t* id);

/**
 * Gets the VXI manufacturer ID of the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param mfr          Storage for the manufacturer ID (should be 0xFEEE for Highland Technology.
 * @return 0 on success, non-zero on failure.
 */
int v210_get_vxi_mfr(VME_REGION* restrict v210_region, uint16_t* mfr);

/**
 * Gets the VXI model type of the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param mtype        Storage for the model type (should be 22210 for V210).
 * @return 0 on success, non-zero on failure.
 */
int v210_get_vxi_type(VME_REGION* restrict v210_region, uint16_t* mtype);

/**
 * Gets the FPGA revision of the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param rev          Storage for the FPGA revision (typically 'A').
 * @return 0 on success, non-zero on failure.
 */
int v210_get_fpga_rev(VME_REGION* restrict v210_region, uint16_t* rev);

/***************************************************************************************************
 * V210 Error LED Control
 **************************************************************************************************/

/**
 * Turns off the error LED on the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @return 0 on success, non-zero on failure.
 */
int v210_turn_off_error_led(VME_REGION* restrict v210_region);

/**
 * Turns on the error LED on the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @return 0 on success, non-zero on failure.
 */
int v210_turn_on_error_led(VME_REGION* restrict v210_region);

/**
 * Checks if the error LED on the V210 module is on.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param  is_on       Storage for the LED status (true if on, false if off).
 * @return 0 on success, non-zero on failure.
 */
int v210_is_error_led_on(VME_REGION* restrict v210_region, bool* is_on);

/***************************************************************************************************
 * V210 Relay Drivers
 **************************************************************************************************/

/**
 * Enables the P4TM and P3TM relay drivers on the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @return 0 on success, non-zero on failure.
 */
int v210_enable_relay_drivers(VME_REGION* restrict v210_region);

/**
 * Disables the P4TM and P3TM relay drivers on the V210 module.
 * 
 * @param  v210_region VME region of the V210 module.
 * @return 0 on success, non-zero on failure.
 */
int v210_disable_relay_drivers(VME_REGION* restrict v210_region);

/**
 * Checks if the P4TM relay driver on the V210 module is enabled.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param  is_enabled  Storage for the driver status (true if enabled, false if disabled).
 * @return 0 on success, non-zero on failure.
 */
int v210_is_p4tm_driver_enabled(VME_REGION* restrict v210_region, bool* is_enabled);

/**
 * Checks if the P3TM relay driver on the V210 module is enabled.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param  is_enabled  Storage for the driver status (true if enabled, false if disabled).
 * @return 0 on success, non-zero on failure.
 */
int v210_is_p3tm_driver_enabled(VME_REGION* restrict v210_region, bool* is_enabled);

/***************************************************************************************************
 * V210 Relay Control
 **************************************************************************************************/

/**
 * Sets multiple channel relays on the V210 module based on the provided channel mask.
 * 
 * @param  v210_region VME region of the V210 module.
 * @param  channel_mask 64-bit mask where each bit represents a channel (0 - 63).
 * @return 0 on success, non-zero on failure.
 */
int v210_set_relays(VME_REGION* restrict v210_region, uint64_t channel_mask);

/**
 * Gets the status of all channel relays on the V210 module as a 64-bit mask.
 * 
 * @param  v210_region       VME region of the V210 module.
 * @param  let_relays_settle If true, waits for 10 ms to allow relays to settle before reading.
 * @param  mask              Storage for the 64-bit mask representing relay statuses.
 * @return 0 on success, non-zero on failure.
 */
int v210_get_relays(VME_REGION* restrict v210_region, bool let_relays_settle, uint64_t* mask);
