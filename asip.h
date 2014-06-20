/*
 * asip.h -  Arduino Services Interface Protocol (ASIP)
 * 
 * Copyright (C) 2014 Michael Margolis
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef asip_h
#define asip_h

#include "Boards.h"  // Hardware pin macros
#include "Arduino.h"
#if defined USE_I2C
#include "Wire.h"  // for I2C services
#endif

#define PRINTF_DEBUG
#define VERBOSE_DEBUG(X) //X  // uncomment x to enable verbose debug


/* Version numbers for the protocol.  
 * This number can be queried so that host software can test
 *  whether it will be compatible with the installed firmware. 
 */ 
const int ASIP_MAJOR_VERSION  = 0; // for non-compatible changes
const int ASIP_MINOR_VERSION  = 1; // for backwards compatibility

typedef byte pinArray_t; // the type used by services to provide an array of needed pins 

enum pinMode_t {UNALLOCATED_PIN_MODE, INPUT_MODE, INPUT_PULLUP_MODE, OUTPUT_MODE, ANALOG_MODE, PWM_MODE, INVALID_MODE=-3, OTHER_SERVICE_MODE=-2,  RESERVED_MODE=-1};

//System messages
// Request messages to Arduino
const char SYSTEM_MSG_HEADER      = '#';  // system requests are preceded with this tag
const char SYSTEM_GET_INFO        = '?';  // Get version and hardware info

// messages from Arduino
const char EVENT_HEADER        = '@';  // event messages are preceded with this tag 
const char ERROR_MSG_HEADER    = '~';  // error messages begin with this tag
const char DEBUG_MSG_HEADER    = '!';  // debug messages begin with this tag

// tags available all services (avoid these constants when creating service specific tags)
// request tags
const char AUTOEVENT_REQUEST = 'A';  // this tag sets autoevent status
const char REMAP_PIN_REQUEST = 'R';  // for services that can change pin numbers
// Reply tags common to all services
const char SERVICE_EVENT     = 'e';  //  


// error messages
enum asipErr_t {ERR_NO_ERROR, ERR_INVALID_SERVICE, ERR_UNKNOWN_REQUEST, ERR_INVALID_PIN, ERR_MODE_UNAVAILABLE, ERR_INVALID_MODE, ERR_WRONG_MODE, ERR_INVALID_DEVICE_NUMBER, ERR_DEVICE_NOT_AVAILABLE, ERR_I2C_NOT_ENABLED};

const byte MIN_MSG_LEN = 4;  // valid request messages must be at least this many characters

const char NO_EVENT = '\0';  // tag to indicate the a service does not produce an event


#ifdef PRINTF_DEBUG
extern char _buf[64];
#define printf(...)                         \
    Serial.write(DEBUG_MSG_HEADER);   \
    do {                            \
        sprintf(_buf, __VA_ARGS__); Serial.write(_buf); \
    } while (0) 
    
 #else
 #define printf(...) 
 #endif

class asipServiceClass 
{
public:
  asipServiceClass(const char svcId, const char evtId); 
  virtual ~asipServiceClass();  
  virtual void begin(byte nbrElements, byte pinCount, const pinArray_t pins[]);    
  virtual void begin(byte nbrElements); // begin with no pins starts an I2C service
  virtual void reportValue(int sequenceId, Stream * stream)  = 0; // send the value of the given device
  virtual void reportValues(Stream *stream); // send all values separated by commas, preceded by header and terminated with newline
  virtual void setAutoreport(Stream *stream); // how many ticks between events, 0 disables 
  virtual void processRequestMsg(Stream *stream) = 0;
  //virtual void reportInvalidRequest( const char svc, const char request, const char *errorMessage, Stream *stream);
  virtual void reportError( const char svc, const char request, asipErr_t errno, Stream *stream); // report service request errors
  
   // todo - protect these but give access to asipClass
   const char ServiceId;       // the unique character that identifies this service 
   unsigned int autoInterval;  // the number of ticks between each autoevent, 0 disables autoevents
   unsigned int nextTrigger;   // tick value for the next event (note this rolls over after 65 seconds so intervals should be limited to under one minute
   
protected:
   const char EventId;        // the unique character that identifies the default event provided by service
   byte nbrElements;         // the number of items supported by this service
   byte pinCount;            // total number of pins in the pins array 
   const pinArray_t *pins;   // stores pins used by this service  
  
};

class asipClass 
{
public:
  asipClass();
  void begin(Stream *s, int svcCount, asipServiceClass *serviceArray[], char *sketchName );
  void service();
  // low level interface 
  void sendAnalog(byte pin, int value);
  void sendDigitalPort(byte portNumber, int portData);
  asipErr_t registerPinMode(byte pin, pinMode_t mode);
  pinMode_t getPinMode(byte pin); 
  void sendPinModes(); 
  void sendPinMap(); 
  void sendErrorMessage( const char svc, const char tag, asipErr_t errno, Stream *stream); 
  void startI2C(); // must be called at least once to use I2C services
private:
  Stream *serial;
  char *programName;
  unsigned int autoEventTickDuration; // the number of milliseconds between each event tick
  unsigned int previousTick;
  asipServiceClass **services;
  int nbrServices; 
  pinMode_t pinModes[NUM_DIGITAL_PINS]; // defined in pins_arduino.h for each board
  boolean I2C_Started;

  void processSystemMsg();

 };
 
extern asipClass asip;

#endif