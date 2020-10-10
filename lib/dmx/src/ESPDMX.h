// - - - - -
// ESPDMX - A Arduino library for sending and receiving DMX using the builtin serial hardware port.
// ESPDMX.cpp: Library implementation file
//
// Copyright (C) 2015  Rick <ricardogg95@gmail.com>
// This work is licensed under a GNU style license.
//
// Last change: Marcel Seerig <https://github.com/mseerig>
//
// Documentation and samples are available at https://github.com/Rickgg/ESP-Dmx
// - - - - -

#include <inttypes.h>
#ifdef UNIT_TEST
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
#endif
#include <Arduino.h>

#ifndef ESPDMX_h
#define ESPDMX_h

// ---- Methods ----

class DMXESPSerial {
public:
  uint8_t *dmxData;
  void init(HardwareSerial *serialPort, unsigned int sendPin, uint16_t MaxChan);
  uint8_t read(uint16_t Channel);
  void write(uint16_t channel, uint8_t value);
  void update();
  void end();
private:
  bool dmxStarted;
  HardwareSerial *serialPort;
  unsigned int chanSize;
  unsigned int sendPin;
};

#endif
