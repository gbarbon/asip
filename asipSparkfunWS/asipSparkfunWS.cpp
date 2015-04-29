/*
 * asipSparkfunWS.cpp - Sparfun Weather Shield serivce for Arduino Services Interface Protocol (ASIP)
 * 
 * These services use I2C Arduino libraries from: 
 * 		https://github.com/sparkfun/MPL3115A2_Breakout
 * 		https://github.com/sparkfun/HTU21D_Breakout
 */

#include "asipSparkfunWS.h"  
#include "HTU21D.h" //humidity sensor
#include "MPL3115A2.h" //barometric pressure sensor

// #define ASIP_SERVICE_NAME  "SWS"

// MPL3115A2 altimeter;
HTU21D humiditysens; 
MPL3115A2 barometer;

// static PROGMEM const prog_char altitudeName[]  = "Altitude";
static PROGMEM const prog_char humidityName[]  = "Humidity";
static PROGMEM const prog_char pressureName[]  = "Pressure";


/*
 Altitute service not implemented, it uses the same sensor of the Pressure service
// Altitude  service
AltitudeClass::AltitudeClass(const char svcId) : asipServiceClass(svcId){svcName = altitudeName;}

void AltitudeClass::begin(byte nbrElements, serviceBeginCallback_t serviceBeginCallback) {
   nbrElements = constrain(nbrElements, 0, NBR_ALTITUDE_FIELDS);    
   asipServiceClass::begin(nbrElements,serviceBeginCallback);  
   altimeter.begin();
   altimeter.setModeAltimeter();
   altimeter.setOversampleRate(7); // Sets the # of samples from 1 to 128. See datasheet.
   altimeter.enableEventFlags(); // Enable all three pressure and temp event flags
}

void AltitudeClass::reset() { }

// send the value of the given device
void AltitudeClass::reportValue(int sequenceId, Stream * stream) {
  if( sequenceId < nbrElements) {
	  stream->print(field[sequenceId]);
  }
}

// send all values separated by commas, preceded by header and terminated with newline
void AltitudeClass::reportValues(Stream *stream) {
		
    // altimeter.setModeStandby(); // Puts the sensor into Standby mode. Required when changing CTRL1 register.
    // altimeter.setModeAltimeter();
    // altimeter.setOversampleRate(7); // Sets the # of samples from 1 to 128. See datasheet.
    // altimeter.enableEventFlags(); // Enable all three pressure and temp event flags
    // altimeter.setModeActive(); // Start taking measurements!
	
	field[0] = altimeter.readAltitude(); // Returns float with meters above sealevel. Ex: 1638.94
	field[1] = altimeter.readAltitudeFt(); // Returns float with feet above sealevel. Ex: 5376.68
    asipServiceClass::reportValues(stream); // the base class reports the data
}

void AltitudeClass::processRequestMsg(Stream *stream) {
   int request = stream->read();
   if( request == tag_AUTOEVENT_REQUEST) {
      setAutoreport(stream);
   }
   if( request == tag_ALTITUDE_REQUEST) {
      setAutoreport(stream);
   }
   else if(request == tag_ALTITUDE_MEASURE){ 
      reportValues(stream);  // send a single measurement
   }
   else {
     reportError(ServiceId, request, ERR_UNKNOWN_REQUEST, stream);
   }
}
*/

// Humidity  service
HumidityClass::HumidityClass(const char svcId) : asipServiceClass(svcId){svcName = humidityName;}

void HumidityClass::begin(byte nbrElements, serviceBeginCallback_t serviceBeginCallback) {
   nbrElements = constrain(nbrElements, 0, NBR_HUMIDITY_FIELDS);    
   asipServiceClass::begin(nbrElements,serviceBeginCallback);  
   humiditysens.begin();
   //humiditysens.setResolution();
}

void HumidityClass::reset() { }

// send the value of the given device
void HumidityClass::reportValue(int sequenceId, Stream * stream) {
  if( sequenceId < nbrElements) {
	  stream->print(field[sequenceId]);
  }
}

// send all values separated by commas, preceded by header and terminated with newline
void HumidityClass::reportValues(Stream *stream) {
	
	field[0] = humiditysens.readHumidity(); 
	field[1] = humiditysens.readTemperature(); 

    asipServiceClass::reportValues(stream); // the base class reports the data
}

void HumidityClass::processRequestMsg(Stream *stream) {
   int request = stream->read();
   if( request == tag_AUTOEVENT_REQUEST) {
      setAutoreport(stream);
   }
   if( request == tag_HUMIDITY_REQUEST) {
      setAutoreport(stream);
   }
   else if(request == tag_HUMIDITY_MEASURE){ 
      reportValues(stream);  // send a single measurement
   }
   else {
     reportError(ServiceId, request, ERR_UNKNOWN_REQUEST, stream);
   }
}

// Pressure  service
PressureClass::PressureClass(const char svcId) : asipServiceClass(svcId){svcName = pressureName;}

void PressureClass::begin(byte nbrElements, serviceBeginCallback_t serviceBeginCallback) {
   nbrElements = constrain(nbrElements, 0, NBR_PRESSURE_FIELDS);    
   asipServiceClass::begin(nbrElements,serviceBeginCallback);  
   barometer.begin();
   //barometer.setModeStandby(); // Puts the sensor into Standby mode. Required when changing CTRL1 register.
   //barometer.setModeActive(); // Start taking measurements!
   barometer.setModeBarometer();
   barometer.setOversampleRate(7); // Sets the # of samples from 1 to 128. See datasheet.
   barometer.enableEventFlags(); // Enable all three pressure and temp event flags
}

void PressureClass::reset() { }

// send the value of the given device
void PressureClass::reportValue(int sequenceId, Stream * stream) {
  if( sequenceId < nbrElements) {
	  stream->print(field[sequenceId]);
  }
}

// send all values separated by commas, preceded by header and terminated with newline
void PressureClass::reportValues(Stream *stream) {
	//
	// barometer.setModeStandby(); // Puts the sensor into Standby mode. Required when changing CTRL1 register.
	// barometer.setModeBarometer();
	// barometer.setOversampleRate(7); // Sets the # of samples from 1 to 128. See datasheet.
	// barometer.enableEventFlags(); // Enable all three pressure and temp event flags
	// barometer.setModeActive(); // Start taking measurements!

	field[0] = barometer.readPressure(); // Returns float with barometric pressure in Pa. Ex: 83351.25
	field[1] = barometer.readTemp(); // Returns float with current temperature in Celsius. Ex: 23.37
	field[2] = barometer.readTempF(); // Returns float with current temperature in Fahrenheit. Ex: 73.96

    asipServiceClass::reportValues(stream); // the base class reports the data
}

void PressureClass::processRequestMsg(Stream *stream) {
   int request = stream->read();
   if( request == tag_AUTOEVENT_REQUEST) {
      setAutoreport(stream);
   }
   if( request == tag_PRESSURE_REQUEST) {
      setAutoreport(stream);
   }
   else if(request == tag_PRESSURE_MEASURE){ 
      reportValues(stream);  // send a single measurement
   }
   else {
     reportError(ServiceId, request, ERR_UNKNOWN_REQUEST, stream);
   }
}