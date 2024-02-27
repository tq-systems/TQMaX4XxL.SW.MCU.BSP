# Changelog

All notable changes to this project will be documented in this file.
Releases are named with the following scheme:

`[TQMaX4XxL.SW.MCU.BSP.<version number>]`

[[_TOC_]]

## [TQMaX4XxL.SW.MCU.SDK_08.06.00.45.BSP.0002]

### Added

* Directory "templates" for holding actual needed files that facilitate import.
* Possibility of flashing a bootable image through uniflash.
* Own SBL and related uniflash configurations.
* License texts.

### Changed

* Build system: Changed .projectspecs improving the import of the project.
* Moved setting `OSPI_DataCaptureDelay` register to the nor_flash_driver.  
* Documentation: updated all documentation.

### Fixed

* AFE example: changed frame format.
* GPIO example: solved bug that threw out wrong reading of IO states.
* EEPROM example: solved bug that didn't allow operations.
* EEPROM example: both EEPROMS can now be operated.
* CAN example: solved bug that didn't allow operations.

## [TQMaX4XxL.SW.MCU.SDK_08.06.00.45.BSP.0001]

First release.
