# MLX90393 to STM32 Nucleo

WORK IN PROGRESS - NO WARRANTY FOR ANYTHING!

This is a quick test of 3-axis magnetometer MLX90393 on a breakout board CJMCU-90393 compatible with [Sparkfun](https://learn.sparkfun.com/tutorials/qwiic-magnetometer-mlx90393-hookup-guide/) or [Adafruit 4022](https://www.adafruit.com/product/4022), wired to STM32C031 Nucleo-C031.

<img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/7/5/9/PU.png" width="40%" > [SCHEMA](https://cdn.sparkfun.com/assets/b/b/3/5/a/Qwiic_MLX90393_Magnetometer.pdf)

The breakout board has two sets of soldering holes for SPI and I2C interfaces. No "qwiic" connectors.

Started from I2C variant because the device comes configured in I2C mode. Also hoping that I2C may be more tolerant to long wires.


Adaptation for STM32 requires replacing the Arduino I2C or SPI layer with bare-metal STM32 (and optionally serial and GPIO).

Prints go to the UART thru the Nucleo's onboard debugger. 115200/8,1,N

Using the STM32C031 Nucleo just because it is simple, cheap and we got a spare one in the drawer. It has only 32KB of flash, so no room for fancy stuff.

Took as prototype the [Adafruit library by Kevin Townsend](https://github.com/adafruit/Adafruit_MLX90393_Library).

TODO: use this library https://github.com/tedyapo/arduino-MLX90393 as in the Sparkfun demo;
with the DRDY pin.

TODO: would like SPI for better robustness. Cut the "CS" strap on the board! On my board the straps are on the reverse side.

CURRENT STATE:
* simple I2C working, with (tweaked) Adafruit library; no DRDY, TRIG pins yet
* with CubeIDE; STM32Cube_FW_C0_V1.4.0
