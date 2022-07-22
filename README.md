# TQMaX4XxL.SW.MCU.BSP

## Description
This repository contains the microcontroller Board Support Package for the R5 core(s), that are present on the TQMa24XxL and TQMa64XxL modules.

## Build requirements
This project requires:
- [TI Code Composer Studio](https://www.ti.com/tool/download/CCSTUDIO/11.1.0.00011) 
- [TI MCU-PLUS-SDK-AM64X](https://www.ti.com/tool/download/MCU-PLUS-SDK-AM64X/08.02.00.31)
- [TI SysConfig](https://www.ti.com/tool/download/SYSCONFIG/1.11.0_2225)

## Usage
- Import project in CCS
- Make sure the environment variable COM_TI_MCU_PLUS_SDK_AM64X_INSTALL_DIR points to the correct directory (e.g.: C:\ti\mcu_plus_sdk_am64x_08_02_00_31)
- Build
- prepare the SOC following the TI SDK documentation ${COM_TI_MCU_PLUS_SDK_AM64X_INSTALL_DIR}/docs/api_guide_am64x/EVM_SETUP_PAGE.html#EVM_SOC_INIT_SD_BOOT_MODE
- Debug

## License
BSD-3-Clause
