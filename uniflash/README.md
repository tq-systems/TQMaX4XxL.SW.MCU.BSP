<!---
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2024 TQ-Systems GmbH <license@tq-group.com>,
D-82229 Seefeld, Germany.
--->
# UART Uniflash

The UART Uniflash is a scripted method to load an application into the device's flash using Python's interpreter.
To flash an application into the flash via UART using Uniflash, it is necessary to first load a flash-writer application
that is run by the R5 core from internal RAM. The flash writer then writes the application into the flash using some allocated
memory (in MSRAM or DDR-RAM) as a cache.

## Flashing

* Power off the board.
* Set the boot mode to `UART BOOT` via the DIP-switches:
    |     | 1   | 2   | 3   | 4   |
    | --- | --- | --- | --- | --- |
    | S4  | ON  | ON  | OFF | ON  |
    | S5  | ON  | ON  | OFF | OFF |
    | S6  | OFF | ON  | OFF | OFF |
    | S7  | OFF | OFF | OFF | OFF |
* Power on the board.
* Open CMD or shell.
  * Execute Uniflash using Python's interpreter.
  * Select the desired configuration file or create a configuration file.

  ```bat
    python uart_uniflash.py -p COM<X> --cfg examples/bsp_flash_hsfs.cfg
  ```

  * Optionally, use the uniflash_gui.
  * __NOTE__: All files selected are relative to their root directory.
* After successfull flash, power off the board set `OSPI BOOT` via DIP-switches and power on again.
* Your application should be running.

## Notes

The flash-writer itself is a ROM-loader readable boot image that can be seen as an SBL application.
The flash-writer will write another SBL into the flash along with the application image.
To edit both SBLs used in this repository, you can use the projects listed in this directory.

* tq_sbl_uart_uniflash (Flash-writer)
* tq_sbl_ospi (SBL for the xSPI boot).
* __NOTE__: Both default SBL applications do not use or initialize DDR-RAM.

* __NOTE__: Be aware of the [OSPI configurations](../../README.md) that may be needed.
* __NOTE__: Note that the ROM-loader will look for a second bootloader (SBL) at address 0x00.
* __NOTE__: By default, the SBL is configured to jump to address `0x80000` after boot. The application is expected
  there by the SBL.
* __NOTE__: Further information on the flashing process can be found [here](https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/09_00_00_35/exports/docs/api_guide_am64x/GETTING_STARTED_FLASH.html)
