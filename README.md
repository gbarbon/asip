# asip
Arduino Service Interface Protocol -  Sparkfun Weather Station service

asipSparkfunWS aims to support the Sparkfun Weather Shield for Arduino through an ASIP I2C service that provides  IMU (Inertial Measurement Unit) data.

Sensors to be supported are:
    HTU21D - humidity sensor
    MPL3115A2 - barometric pressure sensor
    ALS-PT19 - light sensor
    GP635T - GPS module(optional)
    rain (optional)
    wind (optional)

Libraries
This project uses external libraries, not committed in the repository. 
Please refer to respective repositories:
   https://github.com/sparkfun/HTU21D_Breakout for the HTU21D humidity sensor
   https://github.com/sparkfun/MPL3115A2_Breakout for the MPL3115A2 barometric pressure sensor
