/**
 * Example program to demonstrate the usage of the V280 library.
 */

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <V120.h>

#include "v280.h"

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

static void print_usage(const char* progname) {
  printf("Usage: %s [OPTIONS]\n", progname);
  printf("\nOptions:\n");
  printf("  --v120_id <id>          V120 controller ID\n");
  printf("  --v280_address <addr>   V280 base address (e.g. 0xC000)\n");
  printf("  --v280_addr_mode <mode> Addressing mode (a16, a24, a32)\n");
  printf("  --v280_name <name>      Logical name for V280 module\n");
  printf("  --help                  Show this help message\n");
}

int main(int argc, char* argv[]) {
  int v120_id = -1;
  uint32_t v280_address = 0;
  const char* v280_addr_mode = NULL;
  const char* v280_name = NULL;

  bool parsed_id = false, parsed_address = false, parsed_mode = false, parsed_name = false;

  static struct option long_opts[] = {
    {"v120_id",        required_argument, 0, 'i'},
    {"v280_address",   required_argument, 0, 'a'},
    {"v280_addr_mode", required_argument, 0, 'm'},
    {"v280_name",      required_argument, 0, 'n'},
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
        v280_address = strtoul(optarg, NULL, 0);
        parsed_address = true;
        break;
      
      case 'm':
        v280_addr_mode = optarg;
        parsed_mode = true;
        break;

      case 'n':
        v280_name = optarg;
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

  /** V280 operates in the 16-bit or 24-bit address space. */
  V120_PD addr_mode;
  if (strcasecmp(v280_addr_mode, "a16") == 0) {
    addr_mode = V120_A16;
  } else if (strcasecmp(v280_addr_mode, "a24") == 0) {
    addr_mode = V120_A24;
  } else {
    printf("Error: Invalid V280 addressing mode '%s'\n", v280_addr_mode);
    return 1;
  }

  V120_HANDLE* hV120 = v120_open(v120_id);
  if (hV120 == NULL) {
    printf("Error: Failed to open V120 handle\n");
    return -1;
  }

  VME_REGION* v280_region = v280_add_region(hV120, v280_address, addr_mode, v280_name);
  if (v280_region == NULL) {
    printf("Error: Failed to add V280 region\n");
    v120_close(hV120);
    return -1;
  }

  if (v120_allocate_vme(hV120, 0) < 0) {
    printf("Error: Failed to allocate VME region\n");
    v120_close(hV120);
    v280_delete_region(v280_region);
    return -1;
  }

  uint16_t vxi_mfr, vxi_type, modrev, serial, rom_id, rom_rev, mcount, dash, cal_id, ycal, dcal;
  /** Getting V280 overhead information. */
  printf("\n--- V280 Overhead Information ---\n");
  if (v280_get_vxi_mfr(v280_region, &vxi_mfr)) printf("Error: Failed to get VXI Manufacturer ID\n");
  else printf("VXI Manufacturer ID: 0x%04X\n", vxi_mfr);

  if (v280_get_vxi_type(v280_region, &vxi_type)) printf("Error: Failed to get VXI Model Type\n");
  else printf("VXI Model Type: %u\n", vxi_type);

  if (v280_get_modrev(v280_region, &modrev)) printf("Error: Failed to get Hardware Revision\n");
  else printf("Hardware Revision: %c\n", (char)modrev);

  if (v280_get_serial(v280_region, &serial)) printf("Error: Failed to get Serial Number\n");
  else printf("Serial Number: %u\n", serial);

  if (v280_get_rom_id(v280_region, &rom_id)) printf("Error: Failed to get Firmware ID\n");
  else printf("Firmware ID: %u\n", rom_id);

  if (v280_get_rom_rev(v280_region, &rom_rev)) printf("Error: Failed to get Firmware Revision\n");
  else printf("Firmware Revision: %c\n", (char)rom_rev);

  if (v280_get_mcount(v280_region, &mcount)) printf("Error: Failed to get 1KHz Realtime Counter\n");
  else printf("1 KHz Realtime Counter: %u\n", mcount);

  if (v280_get_dash(v280_region, &dash)) printf("Error: Failed to get Dash Number\n");
  else printf("Dash (Module Version) Number: %u\n", dash);

  /** Getting V280 calibration information. */
  printf("\n--- V280 Calibration Information ---\n");
  if (v280_get_cal_id(v280_region, &cal_id)) printf("Error: Failed to get Calibration ID\n");
  else printf("Calibration ID: %u\n", cal_id);

  if (v280_get_ycal(v280_region, &ycal)) printf("Error: Failed to get Calibration Year\n");
  else printf("Calibration Year: %u\n", ycal);

  if (v280_get_dcal(v280_region, &dcal)) printf("Error: Failed to get Calibration Month/Day\n");
  else {
    uint8_t month = (dcal >> 8) & 0xFF;
    uint8_t day = (dcal & 0xFF);
    printf("Calibration Month/Day: %u/%u\n", month, day);
  }

  char cal_date[16];
  if (v280_get_cal_date(v280_region, cal_date, sizeof(cal_date))) {
    printf("Error: Failed to get Calibration Date\n");
  } else {
    printf("Calibration Date: %s\n", cal_date);
  }

  /** Setting and getting User LED pattern. */
  printf("\n--- V280 User LED Pattern ---\n");
  uint16_t uled_pattern = 0x5A5A;
  if (v280_set_uled(v280_region, uled_pattern)) printf("Error: Failed to set User LED pattern\n");
  if (v280_get_uled(v280_region, &uled_pattern)) printf("Error: Failed to get User LED pattern\n");
  else printf("User LED Pattern: 0x%04X\n", uled_pattern);

  /** Setting and getting rise time delay for channel 0. */
  printf("\n--- V280 Channel Timing Configuration ---\n");
  uint16_t rise_delay = 1000;
  if (v280_set_rise_time_delay(v280_region, 0, 1000)) {
    printf("Error: Failed to set rise time delay for channel 0\n");
  } else {
    printf("Set rise time delay for channel 0 to %u (10 us units)\n", rise_delay);
  }

  if (v280_get_rise_time_delay(v280_region, 0, &rise_delay)) {
    printf("Error: Failed to get rise time delay for channel 0\n");
  } else {
    printf("Rise Time Delay for channel 0: %u (10 us units)\n", rise_delay);
  }

  /** Setting and getting fall time delay for channel 0. */
  uint16_t fall_delay = 500;
  if (v280_set_fall_time_delay(v280_region, 0, fall_delay)) {
    printf("Error: Failed to set fall time delay for channel 0\n");
  } else {
    printf("Set fall time delay for channel 0 to %u (10 us units)\n", fall_delay);
  }

  if (v280_get_fall_time_delay(v280_region, 0, &fall_delay)) {
    printf("Error: Failed to get fall time delay for channel 0\n");
  } else {
    printf("Fall Time Delay for channel 0: %u (10 us units)\n", fall_delay);
  }

  /** Getting V280 input states. */
  printf("\n--- V280 Input States ---\n");
  uint64_t input_states;
  if (v280_get_input_states(v280_region, &input_states)) {
    printf("Error: Failed to get input states\n");
  } else {
    printf("Input States: 0x%012lX\n", input_states);
  }

  /** Writing and reading V280 buffer values. */
  printf("\n--- V280 Buffer Access ---\n");
  uint8_t buffer_index = 32;
  uint16_t buffer_value = 0xABCD;
  if (v280_write_buffer(v280_region, buffer_index, buffer_value)) {
    printf("Error: Failed to write to buffer index %u\n", buffer_index);
  } else {
    printf("Wrote 0x%04X to buffer index %u\n", buffer_value, buffer_index);
  }

  if (v280_read_buffer(v280_region, buffer_index, &buffer_value)) {
    printf("Error: Failed to read from buffer index %u\n", buffer_index);
  } else {
    printf("Read 0x%04X from buffer index %u\n", buffer_value, buffer_index);
  }

  /** Running Built-In Self Test (BIST). */
  printf("\n--- V280 Built-In Self Test (BIST) ---\n");
  if (v280_run_bist(v280_region)) printf("Error: Failed to run BIST\n");
  else printf("BIST completed successfully\n");

  /** Getting BIST error flags. */
  uint64_t bist_error_flags;
  if (v280_get_bist_error_flags(v280_region, &bist_error_flags)) {
    printf("Error: Failed to get BIST error flags\n");
  } else {
    printf("BIST Error Flags: 0x%012lX\n", bist_error_flags);
  }

  v120_close(hV120);
  v280_delete_region(v280_region);

  return 0;
}
