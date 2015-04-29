/*
 * asipSparkfunWS.cpp - Sparfun Weather Shield serivce for Arduino Services Interface Protocol (ASIP)
 * 
 * These services use I2C Arduino libraries from: 
 * 		https://github.com/sparkfun/MPL3115A2_Breakout
 * 		https://github.com/sparkfun/HTU21D_Breakout
 * 
 */

#ifndef ASIP_SWS_h
#define ASIP_SWS_h
#include "asip.h"

#include <Wire.h>

// Service and method defines
// Service IDs must be unique across all services
// Method and event IDs must be unique within a service

//IDs already used by the core mirtle services:  I(I/O),M(Motor),E(Encoder),B(Bump sensor),R(IR line sensors)
//IDs used by other services:  D (distance),S (servo)

/*
 Altitute service not implemented, it uses the same sensor of the Pressure service
// Altitude service 
const char id_ALTITUDE_SERVICE = 'A';
const char  tag_ALTITUDE_REQUEST = 'R';   // enable auto events // FIXME: but there is also the tag_AUTOEVENT_REQUEST!!
const char  tag_ALTITUDE_MEASURE = 'M';   // measure and send a single event 
*/

// Humidity service
const char id_HUMIDITY_SERVICE = 'H';
const char  tag_HUMIDITY_REQUEST = 'R';   // enable auto events // FIXME: but there is also the tag_AUTOEVENT_REQUEST!!
const char  tag_HUMIDITY_MEASURE = 'M';   // measure and send a single event 

// Pressure service 
const char id_PRESSURE_SERVICE = 'P';
const char  tag_PRESSURE_REQUEST = 'R';   // enable auto events // FIXME: but there is also the tag_AUTOEVENT_REQUEST!!
const char  tag_PRESSURE_MEASURE = 'M';   // measure and send a single event 

// const int NBR_ALTITUDE_FIELDS = 2; //meters above sea level, feet above sea level
const int NBR_HUMIDITY_FIELDS = 2; //humidity and temperature from the humidity sensor
const int NBR_PRESSURE_FIELDS = 3; //pressure in Pa, temperature in Celsius, temperature in Fahrenheit

/*
class AltitudeClass : public asipServiceClass {  
public:
   AltitudeClass(const char svcId);  
   void begin(byte nbrElements,serviceBeginCallback_t serviceBeginCallback);   // classes that use I2C instead of specific pins use this begin method
   void reportValue(int sequenceId, Stream * stream) ; // send the value of the given device
   void reportValues(Stream *stream); // send all values separated by commas, preceded by header and terminated with newline  
   void processRequestMsg(Stream *stream);
   void reset();
private:
   float field[NBR_ALTITUDE_FIELDS];
};
*/

class HumidityClass : public asipServiceClass {  
public:
   HumidityClass(const char svcId);  
   void begin(byte nbrElements,serviceBeginCallback_t serviceBeginCallback);   // classes that use I2C instead of specific pins use this begin method
   void reportValue(int sequenceId, Stream * stream) ; // send the value of the given device
   void reportValues(Stream *stream); // send all values separated by commas, preceded by header and terminated with newline  
   void processRequestMsg(Stream *stream);
   void reset();
private:
   float field[NBR_HUMIDITY_FIELDS];
};

class PressureClass : public asipServiceClass {  
public:
   PressureClass(const char svcId);  
   void begin(byte nbrElements,serviceBeginCallback_t serviceBeginCallback);   // classes that use I2C instead of specific pins use this begin method
   void reportValue(int sequenceId, Stream * stream) ; // send the value of the given device
   void reportValues(Stream *stream); // send all values separated by commas, preceded by header and terminated with newline  
   void processRequestMsg(Stream *stream);
   void reset();
private:
   float field[NBR_PRESSURE_FIELDS];
};

//extern AltitudeClass altitude;
extern HumidityClass humidity;
extern PressureClass pressure;

#endif