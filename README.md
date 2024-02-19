# TQ-MBaX4XxL

## Description

This repository contains the Board Support Package (BSP) for the R5 core(s) present in the TQMa24XxL and TQMa64XxL modules.

## Table of Contents

[[_TOC_]]

## Requirements

__ATTENTION__: Use the exact versions described here unless otherwise indicated by a symbol. Other versions may lead to
unexpected behavior.

- [TI Code Composer Studio](https://www.ti.com/tool/download/CCSTUDIO/12.1.0): v12.1.0
- [TI MCU-PLUS-SDK-AM64X](https://www.ti.com/tool/download/MCU-PLUS-SDK-AM64X/08.06.00.45): v08.06.00.45
- [TI SysConfig](https://www.ti.com/tool/download/SYSCONFIG): v1.14.0.2667
- [TI Clang](https://www.ti.com/tool/download/ARM-CGT-CLANG/2.1.2.LTS): v2.1.2.LTS (Included with Code Composer Studio).
- [Python](https://www.python.org/): v3.10.4^
- [Patch](#patching)
  - The patch also requires:
    - [gcc-arm-9.2-2019](https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-win32.zip)
    - [gcc-arm-aarch64-none-eabi](https://developer.arm.com/-/media/Files/downloads/gnu-a/9.2-2019.12/binrel/gcc-arm-9.2-2019.12-mingw-w64-i686-aarch64-none-elf.tar.xz)

__NOTE__: This repository requires the TI toolchain from [Code Composer Studio](https://www.ti.com/tool/download/CCSTUDIO/12.3.0)
and associated TI tools. Access the [API guide from TI](https://software-dl.ti.com/mcu-plus-sdk/esd/AM243X/latest/exports/docs/api_guide_am243x/GETTING_STARTED.html)
for more information on the toolchain.

### Hardware Requirements

| Hardware          | Description                                                                           | Pin Reference |
| ----------------- | ------------------------------------------------------------------------------------- | ------------- |
| Micro USB cable   | Required for connecting the board to your computer for communication or flashing.     | X2            |
| XDS110            | Used for debugging and programming the board.                                         | X47           |
| Personal Computer | Required for programming and interacting with the board.                              | -             |
| Power Supply      | A power supply with a voltage of 24V is required to power the board.                  | X40           |

__NOTE__: These requirements are specific to the MBaX4XxL board using the TQMaX4XxL module.

## Preparation

### Set up Toolchain

- Download the specified version of [Code Composer Studio](https://www.ti.com/tool/download/CCSTUDIO/12.1.0) (`CCS`).
- Download the [TI MCU-PLUS-SDK-AM64X SDK](https://www.ti.com/tool/download/MCU-PLUS-SDK-AM64X/08.06.00.45).
  - __NOTE__: The recommended installation directory is `C:\ti`.
- Install the required [tools](https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/08_06_00_45/exports/docs/api_guide_am64x/SDK_DOWNLOAD_PAGE.html)
  while setting up the SDK.
- [Patch](#patching) the toolchain.
- Import the project:
  - Open `CCS`.
  - Click `File` in the upper right corner of the IDE and select `Import`.
  - Navigate to `Code Composer Studio` > `CCS Projects`.
  - Browse to this repository's path on your machine.
  - Select the `TQMaX4XxL.MCU.bsp` project and click `Finish`.
    - __NOTE__: Errors may occur if the [C/C++ configuration](./TQMaX4XxLMCU.bsp.xml) isn't set yet.
  - Right-click on the project in the Project Explorer and select `Properties`.
  - Verify the [requirements](#requirements) under `CCS General`.
  - Go to `C/C++ General` > `Paths and Variables`.
  - Click `Import` and navigate to the [C/C++ configuration](./TQMaX4XxLMCU.bsp.xml) file to import it.
  - You should now be able to build the target.

### Patching

__ATTENTION__: Ensure all necessary [tools](https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/08_06_00_45/exports/docs/api_guide_am64x/SDK_DOWNLOAD_PAGE.html)
are installed.

- Copy the [ccs folder](./templates/ccs/) into the `Code Composer Studio` installation path.
  - __NOTE__: This step integrates the appropriate [target configuration](#debugging-and-booting).
- Copy the [QSPI patch](./templates/qspi_sdk_file/source) into the SDK installation path.
  - __NOTE__: Necessary for using qspi_nor_flash.
- Rebuild the library following these [instructions](https://software-dl.ti.com/mcu-plus-sdk/esd/AM243X/latest/exports/docs/api_guide_am243x/MAKEFILE_BUILD_PAGE.html).
Rebuild for both debug and release profile.
  - __Note__: Adjust the `imports.mak` file as necessary if your `CCS` version differs or if you've installed the
    [tools](https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/08_06_00_45/exports/docs/api_guide_am64x/SDK_DOWNLOAD_PAGE.html)
    in a different location. This file is located in the `<SDK installation path>`.

## Debugging and Booting

- Connect all [necessary hardware](#hardware-requirements) to your board.
- Select your project.
- Set up the [target configuration](#setting-up-target-configuration).
- Choose a boot medium and follow the corresponding [instructions](#booting).
- Select the R5 core you wish to use. Reset it.
- Click on the menu `Run` in the upper menu bar and click on load.
- Browse and run your application.
  - __NOTE__: For reloading the program, pause the execution and load it again after a CPU reset.
  - __NOTE__: If you hard reset the board, redo the steps for your booting medium.
- Follow the steps for [DDR-Initialization](#initializing-ddr-ram) if you wish to use the lpddr.

### Setting up Target Configuration

For booting, a `target configuration` is needed:

- In the menu bar, go to `View` > `Target Configuration`.
- Right-click on `User defined` and select `New Target Configuration`.
- Name the new configuration, e.g., `TQMa64xxL.ccxml`.
- Open the target configuration.
  - Select `AM64x_GP_TQ` as your board.
  - __NOTE__: If this option isn't available, review the [patching steps](#patching).

### Booting

#### Booting via JTAG

__NOTE__: To boot from the internal SRAM, it is necessary to initialize the DMSC and perform the secure boot routine.
For further information, read the [developers guide](https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/08_06_00_45/exports/docs/api_guide_am64x/BOOTFLOW_GUIDE.html)
offered by TI.

- Set up the [target configuration](#setting-up-target-configuration) if not done.
- Power off the board and set it to `NOBOOT` mode:

    |     | 1   | 2   | 3   | 4   |
    | --- | --- | --- | --- | --- |
    | S4  | ON  | ON  | OFF | ON  |
    | S5  | ON  | ON  | ON  | OFF |
    | S6  | OFF | ON  | ON  | OFF |
    | S7  | ON  | ON  | OFF | OFF |

- [Power on](#hardware-requirements) the board, open `CCS`.
- Select the [user-defined target configuration](#setting-up-target-configuration), right-click on it and select
  `Launch Target Configuration`.
  - __ATTENTION__: Do this every time before debugging to avoid any confusion or CCS selecting other target configurations.

- **Using the Initialization Script**
  - Use the `load_dmsc.js` script:
    - Open the Script Console under the `View` menu and type into the console:

      ```shell
        loadJSfile '<SDK installation path>\\tools\\ccs_load\\am64x\\load_dmsc.js'
      ```

    - __NOTE__: Please mind the architecture of your chip. For HS_FS devices, use the `load_dmsc_hsfs.js` script.
    - The script will wait for an [SBL](#second-boot-loader-sbl); you can either load one SBL of your choice or skip it by
      pausing execution.

- **Using SBL_NULL**
  - Alternatively, you can also load the [sbl_null image](./sbl) into the flash:
    - [Flash](#flashing) the [sbl_null image](./sbl).
      - Use the `tq_sbl_null.cfg`.
      - [Boot](#booting-from-flash).

- Now you can [load a program](#debugging-and-booting) into the SRAM.

#### Booting from Flash

In order to boot from flash, first [load an application into the flash](#flashing).

- Power off the board and set it to `OSPI BOOT`:
    |     | 1   | 2   | 3   | 4   |
    | --- | --- | --- | --- | --- |
    | S4  | ON  | ON  | OFF | OFF |
    | S5  | ON  | ON  | ON  | OFF |
    | S6  | OFF | ON  | OFF | OFF |
    | S7  | OFF | OFF | OFF | OFF |

- [Power on](#hardware-requirements); if the application loaded in the flash is valid, the boot should be complete.

### Flashing

Before flashing, follow the instructions on how to [initialize the OSPI](#initializing-flash).

Here are the necessary steps and possibilities listed in order to flash an image into the
device's flash.

* [UART Uniflash](./uniflash/README.md)

### Set up Terminal

To communicate with the board, connect it to your computer using a USB cable (micro-USB to USB 2.0). Open a terminal
application such as "Tera Term" and use the following settings:

| Setting       | Value                 |
| ------------- | --------------------- |
| Baud Rate     | 115200 bps            |
| Data Bits     | 8 bits                |
| Parity        | None                  |
| Stop Bits     | 1                     |
| Flow Control  | None                  |
| New Line Char | LF (Line Feed)        |
| Local Echo    | Enabled (Recommended) |

## Application Note

### Second Boot Loader (SBL)

In a nutshell, an SBL is an application like any other which is loaded into address 0x0 of the boot medium. After primary
boot executed through the ROM code, the R5 executes the SBL in order to set up the DMSC and peripherals that are
absolutely necessary for the application. Further information about the SBL can be found [here](https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/09_00_00_35/exports/docs/api_guide_am64x/BOOTFLOW_GUIDE.html).

This repository contains modified [SBL](./sbl/). The modified SBLs are used for OSPI-Booting and Uniflash on the MBaX4XxL.

### Initializing Flash

To flash an image into the flash of the MBax4xXL or to use it, it is first necessary to configure the driver of
the OSPI, because the hardware only supports QSPI.

* [Patch the OSPI driver and rebuild the library of the SDK](#patching).

* __NOTE__: Set the hardware's register `OSPI_RD_DATA_CAPTURE_REG_ADD = 0x05` before using the OSPI in your code.
The register is located at `0x0FC40010`. This is already done with the patch in the nor-flash-driver.
  * __Example__:

  ```c
      #define OSPI_RD_DATA_CAPTURE_REG_ADD  (0x0FC40010)
      /*...*/
      /* DELAY_FD = 2 and BYPASS_FLD = enable
      * The default DELAY_FD = 4 does not work with QSPI flash, so DELAY_FD = 2 must be used.
      * This has to be implemented while OSPI is idle. Best practice would be to do it after initialization.
      */
      *(uint32_t*) OSPI_RD_DATA_CAPTURE_REG_ADD = 0x05;
  ```

### Initializing DDR-RAM

* DDR Initialization should typically be done by the [SBL](#second-boot-loader-sbl) or by your application.

* If the DDR-RAM was somehow not initialized:
  * Reset the CPU.
  - Select scripts in the `scripts` menu and click on `AM64_DDR_Initialization_ECC_Disabled`.
  - Now the DDR-RAM should be available.

## License

Except where otherwise noted, all files within this repository are licensed under the following terms,
excluding build system files (such as .cmake), configuration files for development environments
(such as IDE setup files):

SPDX-License-Identifier: BSD-3-Clause

Copyright (c) 2021 - 2023 TQ-Systems GmbH <license@tq-group.com>,
D-82229 Seefeld, Germany.
