/**
 * Example program to demonstrate the usage of the V230 library.
 */

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <V120.h>

#include "v230.h"

/***************************************************************************************************
 * DEFINES
 **************************************************************************************************/

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
 * Prints the usage information for the program.
 * 
 * @param  progname Name of the program.
 */
static void print_usage(const char* progname) {
  printf("Usage: %s [OPTIONS]\n", progname);
  printf("\nOptions:\n");
  printf("  --v120_id <id>          V120 controller ID\n");
  printf("  --v230_address <addr>   V230 base address (e.g. 0xC000)\n");
  printf("  --v230_addr_mode <mode> Addressing mode (a16, a24, a32)\n");
  printf("  --v230_name <name>      Logical name for V230 module\n");
  printf("  --help                  Show this help message\n");
}

#if defined(V230_2) || defined(V230_21)

/**
 * Prints the power supply status results.
 * 
 * @param  status Power supply status structure.
 */
static void print_ps_status(v230_ps_all_status_t status) {
  printf("Power Supply Status:\n");
  printf("  EP1: Voltage = %.2f V, Error = %s\n", status.ep1.voltage, 
      status.ep1.error ? "YES" : "NO");
  printf("  EP2: Voltage = %.2f V, Error = %s\n", status.ep2.voltage,
      status.ep2.error ? "YES" : "NO");
  printf("  EP2.5: Voltage = %.2f V, Error = %s\n", status.ep2_5.voltage,
      status.ep2_5.error ? "YES" : "NO");
  printf("  EP3: Voltage = %.2f V, Error = %s\n", status.ep3.voltage,
      status.ep3.error ? "YES" : "NO");
  printf("  EP5: Voltage = %.2f V, Error = %s\n", status.ep5.voltage,
      status.ep5.error ? "YES" : "NO");
  printf("  EP15: Voltage = %.2f V, Error = %s\n", status.ep15.voltage,
      status.ep15.error ? "YES" : "NO");
  printf("  EM15: Voltage = %.2f V, Error = %s\n", status.em15.voltage,
      status.em15.error ? "YES" : "NO");
}

/**
 * Prints the BIST flags for a single channel.
 * 
 * @param  flags BIST flags structure.
 */
static void print_single_channel_flags(v230_bist_flags_t flags) {
  printf("  CER=%s, NER=%s, PER=%s, ZER=%s, RNG=[", 
      flags.cer ? "YES" : "NO",
      flags.ner ? "YES" : "NO",
      flags.per ? "YES" : "NO",
      flags.zer ? "YES" : "NO");
  for (int range = 0; range < V230_BIST_RNG_NUM; range++) {
    printf("%s", flags.rng[range] ? "1" : "0");
    if (range < V230_BIST_RNG_NUM - 1) printf(", ");
  }
  printf("]\n");
}

/**
 * Prints the full BIST channel results.
 * 
 * @param  results Full BIST channel results structure.
 */
static void print_full_bist_channel_results(v230_full_bist_channel_results_t results) {
  printf("Full BIST Channel Results:\n");
  for (int channel = 0; channel < V230_NUM_CHANNELS; channel++) {
    v230_bist_flags_t flags = results.channel_flags[channel];
    printf("  Channel %2d: ", channel);
    print_single_channel_flags(flags);
  }
}

/**
 * Prints the single channel BIST results.
 * 
 * @param results Single channel BIST results structure.
 */
static void print_single_channel_bist_results(v230_single_channel_bist_results_t results) {
  printf("Single Channel BIST Results:\n");
  v230_bist_flags_t flags = results.channel_flags;
  print_single_channel_flags(flags);

  printf("  Measurements:\n");
  for (int mindex = 0; mindex < V230_SINGLE_CHANNEL_BIST_MEASUREMENTS; mindex++) {
    printf("    Measurement %2d: %d\n", mindex, results.measurements[mindex]);
  }
}

/**
 * Prints the V230 mode.
 * 
 * @param  mode V230 mode.
 */
static void print_mode(v230_mode_t mode) {
  switch (mode) {
    case V230_MODE_OFF: printf("V230 mode is OFF\n"); break;
    case V230_MODE_CHAN_TEST: printf("V230 mode is CHANNEL TEST\n"); break;
    case V230_MODE_BIST: printf("V230 mode is BIST\n"); break;
    case V230_MODE_BOTH: printf("V230 mode is BOTH CHANNEL TEST and BIST\n"); break;
    default: printf("V230 mode is UNKNOWN\n"); break;
  }
}

/**
 * Prints the relay configuration.
 * 
 * @param  config Relay configuration structure.
 */
static void print_relay_config(v230_relay_config_t config) {
  printf("Relay Configuration:\n");
  printf("  K Relay Channel: %u\n", config.channel);
  printf("  C Relay: %s\n", config.c_relay ? "ON" : "OFF");
  for (int i = 0; i < V230_B_RELAY_NUM; i++) {
    printf("  B Relay %d: %s\n", i, config.b_relays[i] ? "ON" : "OFF");
  }
}

/**
 * Prints a single BMUX source.
 * 
 * @param  source BMUX source.
 */
static void print_bmux_single_source(v230_bmux_source_t source) {
  switch (source) {
    case V230_BMUX_SRC_0: printf("+10.00 V\n"); break;
    case V230_BMUX_SRC_1: printf("+911 mV\n"); break;
    case V230_BMUX_SRC_2: printf("+83.1 mV\n"); break;
    case V230_BMUX_SRC_3: printf("+8.25 mV\n"); break;
    case V230_BMUX_SRC_4: printf("-10.00 V \n"); break;
    case V230_BMUX_SRC_5: printf("-90.5 mV\n"); break;
    case V230_BMUX_SRC_6: printf("+10.00 V through 1 M ohms\n"); break;
    case V230_BMUX_SRC_7: printf("GND\n"); break;
    default: printf("Unknown Source\n"); break;
  }
}

/**
 * Prints the BMUX configuration.
 * 
 * @param  config BMUX configuration structure.
 */
static void print_bmux_config(v230_bmux_t config) {
  printf("BMUX Configuration:\n");
  printf("  Cal Pos: ");
  print_bmux_single_source(config.cal_pos);
  printf("  Cal Neg: ");
  print_bmux_single_source(config.cal_neg);
}

#endif

/**
 * Prints the channel configuration.
 * 
 * @param  channel Channel number (0-63).
 * @param  config  Channel configuration structure.
 */
static void print_channel_config(uint16_t channel, v230_channel_config_t config) {
  printf("Channel %u Configuration:\n", channel);
  printf("  Range: ");
  switch (config.range) {
    case V230_CHANNEL_RANGE_1: printf("± 102.4 mV\n"); break;
    case V230_CHANNEL_RANGE_2: printf("± 1.024 V\n"); break;
    case V230_CHANNEL_RANGE_3: printf("± 10.24 V\n"); break;
    default: printf("Unknown Range\n"); break;
  }
  printf("  Filter: ");
  switch (config.filter) {
    case V230_CHANNEL_FILTER_NONE: printf("No Filter\n"); break;
    case V230_CHANNEL_FILTER_200HZ: printf("200 Hz Filter\n"); break;
    case V230_CHANNEL_FILTER_17HZ: printf("17 Hz Filter\n"); break;
    default: printf("Unknown Filter\n"); break;
  }
  printf("  Enabled: %s\n", config.enable ? "YES" : "NO");
}

int main(int argc, char* argv[]) {
  int v120_id = -1;
  uint32_t v230_address = 0;
  const char* v230_addr_mode = NULL;
  const char* v230_name = NULL;

  bool parsed_id = false, parsed_address = false, parsed_mode = false, parsed_name = false;
  static struct option long_opts[] = {
    {"v120_id",        required_argument, 0, 'i'},
    {"v230_address",   required_argument, 0, 'a'},
    {"v230_addr_mode", required_argument, 0, 'm'},
    {"v230_name",      required_argument, 0, 'n'},
    {"help",           no_argument,       0, 'h'},
    {0, 0, 0, 0},
  };

  int opt;
  while ((opt = getopt_long(argc, argv, "i:a:m:n:h", long_opts, NULL)) != -1) {
    switch (opt) {
      case 'i':
        v120_id = atoi(optarg);
        parsed_id = true;
        break;

      case 'a':
        v230_address = strtoul(optarg, NULL, 0);
        parsed_address = true;
        break;

      case 'm':
        v230_addr_mode = optarg;
        parsed_mode = true;
        break;

      case 'n':
        v230_name = optarg;
        parsed_name = true;
        break;

      case 'h':
      default:
        print_usage(argv[0]);
        return 0;
    }
  }

  if (!parsed_id || !parsed_address || !parsed_mode || !parsed_name) {
    print_usage(argv[0]);
    return 1;
  }

  V120_HANDLE* hV120 = v120_open(v120_id);
  if (hV120 == NULL) {
    printf("Error: Failed to open V120 handle\n");
    return -1;
  }

  /** V230 operates in the 16-bit or 24-bit address space. */
  V120_PD addr_mode;
  if (strcasecmp(v230_addr_mode, "a16") == 0) {
    addr_mode = V120_A16;
  } else if (strcasecmp(v230_addr_mode, "a24") == 0) {
    addr_mode = V120_A24;
  } else {
    printf("Error: Invalid V230 addressing mode '%s'\n", v230_addr_mode);
    return 1;
  }

  VME_REGION* v230_region = v230_add_region(hV120, v230_address, addr_mode, v230_name);
  if (v230_region == NULL) {
    printf("Error: Failed to add V230 region\n");
    v120_close(hV120);
    return -1;
  }

  if (v120_allocate_vme(hV120, 0) < 0) {
    printf("Error: Failed to allocate VME region\n");
    v120_close(hV120);
    v230_delete_region(v230_region);
    return -1;
  }

  /*************************************************************************************************
   * Getting Hardware Test Register value.
   ************************************************************************************************/
  printf("\n--- V230 Test Registers ---\n");
  uint16_t htest_value;
  if (v230_get_htest(v230_region, &htest_value) == 0) {
    printf("Hardware Test Register value is 0x%04X\n", htest_value);
  } else {
    printf("Error: Failed to get Hardware Test Register value\n");
  }

  /*************************************************************************************************
   * Setting and getting User Test Register value.
   ************************************************************************************************/
  uint16_t utest_value = 0x55AA;
  if (v230_set_utest(v230_region, utest_value) == 0) {
    printf("Set User Test Register to 0x%04X\n", utest_value);
  } else {
    printf("Error: Failed to set User Test Register\n");
  }

  if (v230_get_utest(v230_region, &utest_value) == 0) {
    printf("User Test Register value is 0x%04X\n", utest_value);
  } else {
    printf("Error: Failed to get User Test Register value\n");
  }

  uint16_t mfr, mtype, serial, rom_id, rom_rev, mcount, dash, scan_count, cal_id, ycal, dcal;
  /*************************************************************************************************
   * Getting V230 overhead information.
   ************************************************************************************************/
  printf("\n--- V230 Overhead Information ---\n");
  if (v230_get_vxi_mfr(v230_region, &mfr) == 0) printf("VXI Manufacturer ID: 0x%04X\n", mfr);
  else printf("Error: Failed to get VXI Manufacturer ID\n");

  if (v230_get_vxi_type(v230_region, &mtype) == 0) printf("VXI Model Type: %u\n", mtype);
  else printf("Error: Failed to get VXI Model Type\n");

  if (v230_get_serial_number(v230_region, &serial) == 0) printf("Serial Number: %u\n", serial);
  else printf("Error: Failed to get Serial Number\n");

  if (v230_get_rom_id(v230_region, &rom_id) == 0) printf("Firmware ID: %u\n", rom_id);
  else printf("Error: Failed to get Firmware ID\n");

  if (v230_get_rom_rev(v230_region, &rom_rev) == 0) printf("Firmware Revision: %c\n", (char)rom_rev);
  else printf("Error: Failed to get Firmware Revision\n");

  if (v230_get_mcount(v230_region, &mcount) == 0) printf("IRQ Update Counter: %u\n", mcount);
  else printf("Error: Failed to get Microprocessor IRQ Update Counter\n");

  if (v230_get_dash_number(v230_region, &dash) == 0) printf("Dash Number: %u\n", dash);
  else printf("Error: Failed to get Dash Number\n");

  if (v230_get_scan_count(v230_region, &scan_count) == 0) {
    printf("ADC Scan Counter: %u\n", scan_count);
  } else {
    printf("Error: Failed to get ADC Scan Counter\n");
  }

  /*************************************************************************************************
   * Getting V230 calibration information.
   ************************************************************************************************/
  printf("\n--- V230 Calibration Information ---\n");
  if (v230_get_cal_id(v230_region, &cal_id) == 0) printf("Calibration ID: 0x%04X\n", cal_id);
  else printf("Error: Failed to get Calibration ID\n");

  if (v230_get_ycal(v230_region, &ycal) == 0) printf("Calibration Year: %u\n", ycal);
  else printf("Error: Failed to get Calibration Year\n");

  if (v230_get_dcal(v230_region, &dcal) == 0) {
    uint8_t month = (dcal >> 8) & 0xFF;
    uint8_t day = dcal & 0xFF;
    printf("Calibration Date: %u/%u\n", month, day);
  } else {
    printf("Error: Failed to get Calibration Month/Day\n");
  }

  char cal_date[16];
  if (v230_get_cal_date(v230_region, cal_date, sizeof(cal_date)) == 0) {
    printf("Calibration Date: %s\n", cal_date);
  } else {
    printf("Error: Failed to get Calibration Date\n");
  }

  /*************************************************************************************************
   * Setting and getting V230 User LED pattern.
   ************************************************************************************************/
  printf("\n--- V230 User LED Pattern ---\n");
  uint16_t led_pattern = 0xAAAA;
  if (v230_set_uled(v230_region, led_pattern) == 0) {
    printf("Set User LED pattern to 0x%04X\n", led_pattern);
  } else {
    printf("Error: Failed to set User LED pattern\n");
  }

  if (v230_get_uled(v230_region, &led_pattern) == 0) {
    printf("User LED pattern is 0x%04X\n", led_pattern);
  } else {
    printf("Error: Failed to get User LED pattern\n");
  }

  /*************************************************************************************************
   * Executing Macro Commands
   ************************************************************************************************/
  printf("\n--- V230 Macro Execution ---\n");

  bool is_macro_busy = true;
  while (is_macro_busy) {
    if (v230_is_macro_busy(v230_region, &is_macro_busy) != 0) {
      printf("Error: Failed to get V230 macro busy status\n");
      break;
    }
  }

  /**********************************************
   * Executing NO_OP Macro. 
   *********************************************/
  printf("\n--- Executing NO-OP Macro ---\n");
  if (v230_execute_macro(v230_region, V230_MACRO_NO_OP) == 0) printf("Executed NO_OP macro\n");
  else printf("Error: Failed to execute NO_OP macro\n");
  
  /**********************************************
   * Executing REBOOT Macro. 
   *********************************************/
  printf("\n--- Executing REBOOT Macro ---\n");
  if (v230_execute_macro(v230_region, V230_MACRO_REBOOT) == 0) printf("Executed REBOOT macro\n");
  else printf("Error: Failed to execute REBOOT macro\n");

  /**********************************************
   * Executing PS_TEST Macro.
   *********************************************/
  printf("\n--- Executing PS TEST Macro ---\n");
  if (v230_execute_macro(v230_region, V230_MACRO_PS_TEST) == 0) printf("Executed PS_TEST macro\n");
  else printf("Error: Failed to execute PS_TEST macro\n");

  /** Wait for PS Macro to complete. */
  is_macro_busy = true;
  while (is_macro_busy) {
    if (v230_is_macro_busy(v230_region, &is_macro_busy) != 0) {
      printf("Error: Failed to get V230 macro busy status\n");
      break;
    }
  }

  uint16_t channel = 7;
#if defined(V230_2) || defined(V230_21)

  /*********************
   * Getting power supply status.
   ********************/
  v230_ps_all_status_t ps_status;
  if (v230_get_all_power_supply_status(v230_region, &ps_status) == 0) print_ps_status(ps_status);
  else printf("Error: Failed to get power supply status\n");

  /*********************
   * Getting BIST error count.
   ********************/
  uint16_t bist_error_count;
  if (v230_get_bist_error_count(v230_region, &bist_error_count) == 0) {
    printf("BIST Error Count: %u\n", bist_error_count);
  } else {
    printf("Error: Failed to get BIST Error Count\n");
  }

  /**********************************************
   * Executing FULL_BIST Macro.
   *********************************************/
  printf("\n--- Executing FULL BIST Macro ---\n");
  if (v230_execute_macro(v230_region, V230_MACRO_FULL_BIST) == 0) {
    printf("Executed FULL BIST macro\n");
  } else {
    printf("Error: Failed to execute FULL BIST macro\n");
  }

  is_macro_busy = true;
  while (is_macro_busy) {
    if (v230_is_macro_busy(v230_region, &is_macro_busy) != 0) {
      printf("Error: Failed to get V230 macro busy status\n");
      break;
    }
  }
  
  /*********************
   * Getting Full BIST Channel Results.
   ********************/
  v230_full_bist_channel_results_t full_bist_results;
  if (v230_get_full_bist_channel_results(v230_region, &full_bist_results) == 0) {
    print_full_bist_channel_results(full_bist_results);
  } else {
    printf("Error: Failed to get full BIST channel results\n");
  }

  /*********************
   * Getting BIST Error Count.
   ********************/
  if (v230_get_bist_error_count(v230_region, &bist_error_count) == 0) {
    printf("BIST Error Count: %u\n", bist_error_count);
  } else {
    printf("Error: Failed to get BIST Error Count\n");
  }

  /**********************************************
   * Executing CHANNEL TEST Macro on channel 7.
   *********************************************/
  printf("\n--- Executing CHANNEL TEST Macro on channel %u ---\n", channel);
  if (v230_execute_macro_channel_test(v230_region, channel) == 0) {
    printf("Executed CHANNEL TEST macro on channel %u\n", channel);
  } else {
    printf("Error: Failed to execute CHANNEL TEST macro on channel %u\n", channel);
  }

  is_macro_busy = true;
  while (is_macro_busy) {
    if (v230_is_macro_busy(v230_region, &is_macro_busy) != 0) {
      printf("Error: Failed to get V230 macro busy status\n");
      break;
    }
  }

  /*********************
   * Getting Single Channel BIST Results.
   ********************/
  v230_single_channel_bist_results_t single_bist_results;
  if (v230_get_single_channel_bist_results(v230_region, &single_bist_results) == 0) {
    print_single_channel_bist_results(single_bist_results);
  } else {
    printf("Error: Failed to get single channel BIST results\n");
  }

  /*********************
   * Getting BIST Error Count.
   ********************/
  if (v230_get_bist_error_count(v230_region, &bist_error_count) == 0) {
    printf("BIST Error Count: %u\n", bist_error_count);
  } else {
    printf("Error: Failed to get BIST Error Count\n");
  }

#endif

#if defined(V230_2) || defined(V230_21)

  /*************************************************************************************************
   * Putting BIST Voltage on Channel 12.
   ************************************************************************************************/
  channel = 12;
  printf("\n--- Putting BIST Voltage on Channel %u ---\n", channel);

  /**********************************************
   * Setting and getting V230 Operating Mode.
   *********************************************/
  printf("\n--- V230 Operating Mode ---\n");
  v230_mode_t mode = V230_MODE_BOTH;
  if (v230_set_mode(v230_region, mode) == 0) printf("Set V230 mode to BOTH\n");
  else printf("Error: Failed to set V230 mode to BOTH\n");

  if (v230_get_mode(v230_region, &mode) == 0) print_mode(mode);
  else printf("Error: Failed to get V230 mode\n");

  /**********************************************
   * Setting and getting Relay Configuration.
   *********************************************/
  printf("\n--- V230 Relay Configuration ---\n");
  v230_relay_config_t relay_config = {
    .channel = channel,
    .c_relay = false,
    .b_relays = {false, false, false, false, false, false, false, false}
  };
  if (v230_set_relay_config(v230_region, relay_config) == 0) printf("Set relay configuration\n");
  else printf("Error: Failed to set relay configuration\n");

  if (v230_get_relay_config(v230_region, &relay_config) == 0) print_relay_config(relay_config);
  else printf("Error: Failed to get relay configuration\n");

  /**********************************************
   * Setting and getting BMUX Configuration.
   *********************************************/
  printf("\n--- V230 BMUX Configuration ---\n");
  v230_bmux_t bmux_config = {
    .cal_pos = V230_BMUX_SRC_1,
    .cal_neg = V230_BMUX_SRC_7,
  };
  if (v230_set_bmux_config(v230_region, bmux_config) == 0) printf("Set BMUX configuration\n");
  else printf("Error: Failed to set BMUX configuration\n");

  if (v230_get_bmux_config(v230_region, &bmux_config) == 0) print_bmux_config(bmux_config);
  else printf("Error: Failed to get BMUX configuration\n");

#endif

  /*************************************************************************************************
   * Setting and getting Channel Configuration for channel 12.
   ************************************************************************************************/
  channel = 12;
  printf("\n--- V230 Channel Configuration ---\n");
  v230_channel_config_t chan_config = {
    .range = V230_CHANNEL_RANGE_3, 
    .filter = V230_CHANNEL_FILTER_NONE,
    .enable = true,
  };
  if (v230_set_channel_config(v230_region, channel, chan_config) == 0) {
    printf("Set channel %u configuration\n", channel);
  } else {
    printf("Error: Failed to set channel %u configuration\n", channel);
  }

  if (v230_get_channel_config(v230_region, channel, &chan_config) == 0) {
    print_channel_config(channel, chan_config);
  } else {
    printf("Error: Failed to get channel %u configuration\n", channel);
  }

  /** Let relays settle. Manual says to wait atleast 25 ms.*/
  struct timespec relay_settle_delay = {0, 50000000}; // 50 ms
  nanosleep(&relay_settle_delay, NULL);

  /*************************************************************************************************
   * Getting first channel with setup error.
   ************************************************************************************************/
  printf("\n--- V230 Channel Setup Error ID ---\n");
  if (v230_get_channel_setup_error_id(v230_region, &channel) == 0) {
    if (channel != 0xFFFF) printf("First channel with setup error: %u\n", channel);
    else printf("No channels have setup errors\n");
  } else {
    printf("Error: Failed to get channel setup error ID\n");
  }

  /*************************************************************************************************
   * Getting all V230 Channel Voltages.
   ************************************************************************************************/
  printf("\n--- V230 Channel Voltages ---\n");
  v230_channel_voltage_t chan_voltage;
  if (v230_get_all_channel_voltages(hV120, v230_region, &chan_voltage) == 0) {
    for (int ch = 0; ch < V230_NUM_CHANNELS; ch++) {
      printf("Channel %d voltage: %.05f V\n", ch, chan_voltage.voltage[ch]);
    }
  } else {
    printf("Error: Failed to get all channel voltages\n");
  }

  /*************************************************************************************************
   * Setting and getting V230 Scan Speed.
   ************************************************************************************************/
  printf("\n--- V230 Scan Speed ---\n");
  bool is_scan_fast;
  if (v230_is_scan_speed_fast(v230_region, &is_scan_fast) == 0) {
    printf("Current scan speed is %s\n", is_scan_fast ? "fast" : "slow");
  } else {
    printf("Error: Failed to get scan speed\n");
  }

  if (v230_set_scan_speed_slow(v230_region) == 0)  printf("Set scan speed to slow\n");
  else printf("Error: Failed to set scan speed to slow\n");

  if (v230_is_scan_speed_fast(v230_region, &is_scan_fast) == 0) {
    printf("Current scan speed is %s\n", is_scan_fast ? "fast" : "slow");
  } else {
    printf("Error: Failed to get scan speed\n");
  }

  if (v230_set_scan_speed_fast(v230_region) == 0) printf("Set scan speed to fast\n");
  else printf("Error: Failed to set scan speed to fast\n");

  if (v230_is_scan_speed_fast(v230_region, &is_scan_fast) == 0) {
    printf("Current scan speed is %s\n", is_scan_fast ? "fast" : "slow");
  } else {
    printf("Error: Failed to get scan speed\n");
  }

  v120_close(hV120);
  v230_delete_region(v230_region);

  return 0;
}
