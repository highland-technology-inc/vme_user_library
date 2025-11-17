/**
 * Example program to demonstrate the usage of the V210 library.
 */

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <V120.h>

#include "v210.h"

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
	printf("  --v210_address <addr>   V210 base address (e.g. 0xC000)\n");
	printf("  --v210_addr_mode <mode> Addressing mode (a16, a24, a32)\n");
	printf("  --v210_name <name>      Logical name for V210 module\n");
	printf("  --help                  Show this help message\n");
}

int main(int argc, char* argv[]) {
  int v120_id = -1;
  uint32_t v210_address = 0;
  const char* v210_addr_mode = NULL;
  const char* v210_name = NULL;

  static struct option long_opts[] = {
    {"v120_id",        required_argument, 0, 'i'},
    {"v210_address",   required_argument, 0, 'a'},
    {"v210_addr_mode", required_argument, 0, 'm'},
    {"v210_name",      required_argument, 0, 'n'},
    {"help",           no_argument,       0, 'h'},
    {0, 0, 0, 0},
  };

  int opt;
  while ((opt = getopt_long(argc, argv, "i:a:m:n:h", long_opts, NULL)) != -1) {
    switch (opt) {
      case 'i':
        v120_id = atoi(optarg);
        break;
      
      case 'a':
        v210_address = strtoul(optarg, NULL, 0);
        break;
      
      case 'm':
        v210_addr_mode = optarg;
        break;

      case 'n':
        v210_name = optarg;
        break;
      
      case 'h':
      default:
        print_usage(argv[0]);
        return 0;
    }
  }

  if (v120_id == -1 || v210_addr_mode == NULL || v210_name == NULL) {
    print_usage(argv[0]);
    return 1;
  }

  /** V210 operates in the 16-bit or 24-bit address space. */
  V120_PD addr_mode;
  if (strcasecmp(v210_addr_mode, "a16") == 0) {
    addr_mode = V120_A16;
  } else if (strcasecmp(v210_addr_mode, "a24") == 0) {
    addr_mode = V120_A24;
  } else {
    printf("Error: Invalid V210 addressing mode '%s'\n", v210_addr_mode);
    return 1;
  }

  V120_HANDLE* hV120 = v120_open(v120_id);
  if (hV120 == NULL) {
    printf("Error: Failed to open V120 handle\n");
    return 1;
  }

  VME_REGION* v210_region = v210_add_region(hV120, v210_address, addr_mode, v210_name);
  if (v210_region == NULL) {
    printf("Error: Failed to add V210 region\n");
    v120_close(hV120);
    return 1;
  }

  if (v120_allocate_vme(hV120, 0) < 0) {
    printf("Error: Failed to allocate VME region\n");
    v120_close(hV120);
    v210_delete_region(v210_region);
    return 1;
  }

  uint16_t board_id, vxi_mfr, vxi_type, fpga_rev;
  /** Getting V210 Overhead Information. */
  printf("\n--- V210 Overhead Information ---\n");
  if (v210_get_board_id(v210_region, &board_id) < 0) printf("Error: Failed to get V210 board ID\n");
  else printf("V210 Board ID: 0x%04X\n", board_id);

  if (v210_get_vxi_mfr(v210_region, &vxi_mfr) < 0) printf("Error: Failed to get V210 VXI MFR\n");
  else printf("V210 VXI MFR: 0x%04X\n", vxi_mfr);

  if (v210_get_vxi_type(v210_region, &vxi_type) < 0) printf("Error: Failed to get V210 VXI Type\n");
  else printf("V210 VXI Type: 0x%04X\n", vxi_type);
  
  if (v210_get_fpga_rev(v210_region, &fpga_rev) < 0) printf("Error: Failed to get V210 FPGA Rev\n");
  else printf("V210 FPGA Rev: %c\n", (char)fpga_rev);

  bool is_error_led_on;
  if (v210_turn_off_error_led(v210_region) < 0) printf("Error: Failed to turn off error LED\n");
  if (v210_is_error_led_on(v210_region, &is_error_led_on) < 0) {
    printf("Error: Failed to get error LED status\n");
  } else {
    if (is_error_led_on) printf("Error: V210 Error LED should be OFF but is ON\n");
  } 

  bool is_p4tm_enabled, is_p3tm_enabled;
  if (v210_enable_relay_drivers(v210_region) < 0) printf("Error: Failed to enable relay drivers\n");
  if (v210_is_p4tm_driver_enabled(v210_region, &is_p4tm_enabled) < 0) {
    printf("Error: Failed to get P4TM driver status\n");
  } else {
    if (!is_p4tm_enabled) printf("Error: P4TM Driver should be enabled but is disabled!\n");
  }
  if (v210_is_p3tm_driver_enabled(v210_region, &is_p3tm_enabled) < 0) {
    printf("Error: Failed to get P3TM driver status\n");
  } else {
    if (!is_p3tm_enabled) printf("Error: P3TM Driver should be enabled but is disabled!\n");
  }

  /** Test on Channel 32. */
  uint64_t channel_mask = (1ULL << 32);
  if (v210_set_relays(v210_region, channel_mask) < 0) {
    printf("Error: Failed to set relay for channel 32\n");
  }
  if (v210_get_relays(v210_region, true, &channel_mask) < 0) {
    printf("Error: Failed to get relay status for channel 32\n");
  } else {
    if ((channel_mask & (1ULL << 32)) == 0) printf("Error: Relay 32 should be ON but is OFF\n");
  }

  channel_mask = 0;
  if (v210_set_relays(v210_region, channel_mask) < 0) {
    printf("Error: Failed to clear relay for channel 32\n");
  }
  if (v210_get_relays(v210_region, true, &channel_mask) < 0) {
    printf("Error: Failed to get relay status for channel 32\n");
  } else {
    if ((channel_mask & (1ULL << 32)) != 0) printf("Error: Relay 32 should be OFF but is ON\n");
  }

  /** Set all relays. */
  channel_mask = 0xFFFFFFFFFFFFFFFF;
  if (v210_set_relays(v210_region, channel_mask) < 0) printf("Error: Failed to set all relays\n");
  if (v210_get_relays(v210_region, true, &channel_mask) < 0) {
    printf("Error: Failed to get all relay statuses\n");
  } else {
    if (channel_mask != 0xFFFFFFFFFFFFFFFF) printf("Error: Not all relays are ON\n");
  }

  /** Clear all relays. */
  channel_mask = 0;
  if (v210_set_relays(v210_region, channel_mask) < 0) printf("Error: Failed to clear all relays\n");
  if (v210_get_relays(v210_region, true, &channel_mask) < 0) {
    printf("Error: Failed to get all relay statuses\n");
  } else {
    if (channel_mask != 0x0000000000000000) printf("Error: Not all relays are OFF\n");
  }

  /** Sequentially set each relay with a delay. */
  channel_mask = 0;
  for (int i = 0; i < V210_CHANNEL_COUNT; i++) {
    channel_mask |= (1ULL << i);
    if (v210_set_relays(v210_region, channel_mask) < 0) {
      printf("Error: Failed to set relay for channel %d\n", i);
    }
    usleep(250000);
  }

  /** Sequentially clear each relay with a delay. */
  for (int i = V210_CHANNEL_COUNT - 1; i >= 0; i--) {
    channel_mask &= ~(1ULL << i);
    if (v210_set_relays(v210_region, channel_mask) < 0) {
      printf("Error: Failed to clear relay for channel %d\n", i);
    }
    usleep(250000);
  }

  if (v210_disable_relay_drivers(v210_region) < 0) {
    printf("Error: Failed to disable relay drivers\n");
  }

  if (v210_turn_on_error_led(v210_region)) printf("Error: Failed to turn on error LED\n");
  if (v210_is_error_led_on(v210_region, &is_error_led_on) < 0) {
    printf("Error: Failed to get error LED status\n");
  } else {
    if (!is_error_led_on) printf("Error: V210 Error LED should be ON but is OFF\n");
  }

  v120_close(hV120);
  v210_delete_region(v210_region);

  return 0;
}