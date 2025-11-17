#!/bin/bash

# --------------------------------------------------------------------------------------------------
# Run one or more VME module programs with predefined configurations.
#
# Supports multiple module types (V210, V230, V280) and multiple configurations
# per module. Each configuration has hard-coded parameters for known hardware.
#
# Note: Manually run 'make' to build the module programs before executing this script.
# --------------------------------------------------------------------------------------------------

set -euo pipefail

# --------------------------------------------------------------------------------------------------
# V210 Configurations
# --------------------------------------------------------------------------------------------------

run_v210_config_1() {
  echo "Running V210 Configuration 1"
  ./apps/v210/run_v210 --v120_id 0 --v210_address 0xC000 --v210_addr_mode a16 --v210_name V210_SLOT1
}

run_v210_config_2() {
  echo "Running V210 Configuration 2"
  ./apps/v210/run_v210 --v120_id 0 --v210_address 0xF000 --v210_addr_mode a16 --v210_name V210_SLOT2
}

run_v210_all() {
  run_v210_config_1
  run_v210_config_2
}

# --------------------------------------------------------------------------------------------------
# V230 Configurations
# --------------------------------------------------------------------------------------------------

run_v230_config_1() {
  echo "Running V230 Configuration 1"
  ./apps/v230/run_v230 --v120_id 0 --v230_address 0xF000 --v230_addr_mode a16 --v230_name V230_SLOT1
}

run_v230_all() {
  run_v230_config_1
}

# --------------------------------------------------------------------------------------------------
# V280 Configurations
# --------------------------------------------------------------------------------------------------

run_v280_config_1() {
  echo "Running V280 Configuration 1"
  ./apps/v280/run_v280 --v120_id 0 --v280_address 0xF000 --v280_addr_mode a16 --v280_name V280_SLOT1
}

run_v280_all() {
  run_v280_config_1
}

# --------------------------------------------------------------------------------------------------
# Main Execution
# --------------------------------------------------------------------------------------------------

print_usage() {
  echo "Usage:"
  echo "  $0 [module] [config|all]"
  echo ""
  echo "Examples:"
  echo "  $0 v210 1        # Run V210 config 1"
  echo "  $0 v230 all      # Run all V230 configs"
  echo "  $0 all           # Run all modules with all configs"
}

if [[ $# -lt 1 ]]; then
  print_usage
  exit 1
fi

module=${1,,}   
config=${2:-all}

case "$module" in
  v210)
    if [[ "$config" == "all" ]]; then
      run_v210_all
    else
      run_v210_config_$config
    fi
    ;;
  v230)
    if [[ "$config" == "all" ]]; then
      run_v230_all
    else
      run_v230_config_$config
    fi
    ;;
  v280)
    if [[ "$config" == "all" ]]; then
      run_v280_all
    else
      run_v280_config_$config
    fi
    ;;
  all)
    run_v210_all
    run_v230_all
    run_v280_all
    ;;
  *)
    print_usage
    exit 1
    ;;
esac
