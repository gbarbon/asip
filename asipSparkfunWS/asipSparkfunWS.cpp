/*
 * asipSparkfunWS.cpp - Sparfun Weather Shield serivce for Arduino Services Interface Protocol (ASIP)
 * 
 * These services use I2C Arduino libraries from: 
 * 		https://github.com/sparkfun/MPL3115A2_Breakout
 * 		https://github.com/sparkfun/HTU21D_Breakout
 * 
 */

#include "asipSparkfunWS.h"  
//#include "HTU21D.h" //humidity sensor
#include "MPL3115A2.h" //barometric pressure sensor

#define ASIP_SERVICE_NAME  "SWS"

//HTU21D humidity; 
MPL3115A2 barometer;

static PROGMEM const prog_char pressureName[]  = "Pressure";

// Pressure  service
PressureClass::PressureClass(const char svcId) : asipServiceClass(svcId){svcName = pressureName;}

void PressureClass::begin(byte nbrElements, serviceBeginCallback_t serviceBeginCallback)
{
   nbrElements = constrain(nbrElements, 0, NBR_PRESSURE_FIELDS);    
   asipServiceClass::begin(nbrElements,serviceBeginCallback);  
   barometer.begin();
   barometer.setModeBarometer();
   barometer.setOversampleRate(7); // Sets the # of samples from 1 to 128. See datasheet.
   barometer.enableEventFlags(); // Enable all three pressure and temp event flags
}

void PressureClass::reset()
{

}

void PressureClass::reportValue(int sequenceId, Stream * stream)  // send the value of the given device
{
  if( sequenceId < nbrElements) {
	  stream->print(field[sequenceId]);
  }
}

void PressureClass::reportValues(Stream *stream) // send all values separated by commas, preceded by header and terminated with newline
{
    // request pressure (3x oversampling mode, high detail, 23.5ms delay)
    // while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());  
    // read calibrated pressure value in Pascals (Pa)
    field[0] =  barometer.readPressure();
	// field[2] =  barometer.getAltitude(pressure);
	// field[0] = pressure / 100; //convert to millibars 

    asipServiceClass::reportValues(stream); // the base class reports the data
}

void PressureClass::processRequestMsg(Stream *stream)
{
   int request = stream->read();
   if( request == tag_AUTOEVENT_REQUEST) {
      setAutoreport(stream);
   }
   else if(request == tag_PRESSURE_MEASURE){ 
      reportValues(stream);  // send a single measurement
   }
      
   else {
     reportError(ServiceId, request, ERR_UNKNOWN_REQUEST, stream);
   }
}

