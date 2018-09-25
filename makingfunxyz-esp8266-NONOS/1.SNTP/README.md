# SNTP example

Refresh time every 10 seconds!

## Hardware:
* NODEMCU

## Tool:
* [ESPFlashDownload](https://www.espressif.com/sites/default/files/tools/flash_download_tools_v3.6.4.rar)

## Download:
* boot_v1.7.bin --> 0x00000
* user1.4096.new.6.bin --> 0x01000
* blank.bin --> 0x3FE000
* esp_init_data_default_v08.bin --> 0x3FC000

## Options:
* CrystalFreq:26M
* SPI SPEED: 40MHz
* SPI MODE: DIO
* FLASH SIZE: 32Mbit-C1
* BAUD: 115200

## Result:
![](https://github.com/imliubo/makingfunxyz-esp8266/blob/master/makingfunxyz-esp8266-NONOS/1.SNTP/sntp.png)