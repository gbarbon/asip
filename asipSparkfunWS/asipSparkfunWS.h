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

// Pressure (Altitude) service 
const char id_PRESSURE_SERVICE = 'P';
// methods
const char  tag_PRESSURE_REQUEST = 'R';   // enable auto events
const char  tag_PRESSURE_MEASURE = 'M';   // measure and send a single event 
// events

const int NBR_PRESSURE_FIELDS = 1; //as for now, only one field used



// the following is not yet implemented
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
   int32_t lastMicros; // time of the previous readings
};

extern PressureClass pressure;

#endif