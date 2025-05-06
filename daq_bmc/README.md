C program for interfacing a Comedi DAQ board to MQTT using jSON data format
The overlay:  arch/arm64/boot/dts/allwinner/overlay/sun50i-h616-spi1-spibmc.dts
The comed module:  drivers/comedi/drivers/daq_bmc.c
The diff kernel patch: opi.diff
Kernel config file: daq_bmc.config
Boot loader file: orangepiEnv.txt

Uses spi1 CS1, there is no spi direct access device file created from the SPI device. The protocol kernel modules makes a /dev/comedi0 or next numbered comedi device for the user program to access the DAQ resources in the standard comedi devce API.

The kernel must be recreated after being configured to not load the spi_dev module and to used the
spi_bmc protocol module instead. The comedi driver selection name is  daq_bmc. 
Kernel config file: daq_bmc.config
copy to .config in the kernel root directory and then run, oldconfig to setup the kernel compile kernel. 
make -j4  bindeb-pkg will create debian deb files in the /usr/src director to load for the new kernel.  
 
http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_Zero_3

At Linux boot the sun50i-h616-spi1-spibmc overlay binary is loaded per the orangepiEnv.txt file. This configures the spi port physical pins for SPI1 and CS1. Next the module section named spibmc (in the daq_bmc module file) is loaded to run the SPI probe for device information for the protocol part of the driver daq_bmc. The protocol part of the driver then creates the /dev/comedi0 user API file for porgrams to open and use for the SPI connected DAQ device (pic18f47Q84 and others).
