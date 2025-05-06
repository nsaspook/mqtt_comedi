C program for interfacing a Comedi DAQ board to MQTT using jSON data format
The overlay:  arch/arm64/boot/dts/allwinner/overlay/sun50i-h616-spi1-spibmc.dts
The comed module:  drivers/comedi/drivers/daq_bmc.c
The diff kernel patch: opi.diff
Kernel config file: daq_bmc.config

Uses spi1 CS1, there is no spi direct access device file created from the SPI device. The protocol kernel modules makes a /dev/comedi0 or next numbered comedi device for the user program to access the DAQ resources in the standard comedi devce API.

Kernel config file: daq_bmc.config
 
http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_Zero_3
