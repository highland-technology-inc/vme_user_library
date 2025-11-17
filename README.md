# Guide to using Highland Technology Device Interface Libraries to Configure & Access VME Boards w/ User Code Examples

## Overview

The vme_interface_library provides access to the capabilities of the Highland Technology VME boards. The primary components of this repository are the board-level interface libraries, which enable direct interaction with the hardware.

In this release, the supported Interface Libraries are

- V210
- V230
- V280
- More to come...

To help users get started, this distribution includes example programs demonstrating typical usage, as well as a sample script to simplify program execution.

## Downloads

1. Download the V120 driver and command line tools by cloning the v120_driver and v120_clt repositories as shown below.
  
    ```bash
    git clone https://github.com/highland-technology-inc/v120_driver v120_driver
    git clone https://github.com/highland-technology-inc/v120_clt v120_clt
    ```

    Follow the directions in the README.md of the respective repository to compile and install driver and command line tools.
  
2. Download VME Interface Libraries & User Code Examples

    ```bash
    git clone <insert git repository> vme_interface_library
    ```

## Building VME Interface Libraries & User Code Examples

```bash
cd vme_interface_library
make clean all
```

This will build all VME interface libraries and the included user example programs.

By default, the V230 module is compiled with the `-DV230_21` compiler flag. To build for a different V230 version, run

```bash
make V230_DASH=-DV230_<version>
```

where `<version>` is one of `{1, 11, 2, 21}`.

## Example User Code

The vme_interface_library/apps directory contains example source files, including:

- run_v210.c: Demonstrates how to use the V210 interface library to interact with the V210 VME Relay Module.
- run_v230.c: Demonstrates how to use the V230 interface library to interact with the V230 Analog Input Module.
- run_v280.c: Demonstrates how to use the V280 interface library to interact with the V280 VME Digital Input Module.

## Example Script

An example script is provided to demonstrate how to run the V210 VME Relay Module, V230 Analog Input Module, and V280 VME Digital Input Module programs.

This script shows how to execute the run_v210, run_v230, and run_v280 applications with predefined configuration - so you don't need to manually pass command-line arguments each time.

The script supports multiple modules and module configurations (e.g., different V210 base addresses). It can also easily be extended to include more setups and modules. It's meant to serve as a starting point for users who want to automate running the VME module programs with consistent paramters.
