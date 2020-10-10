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

/* ----- LIBRARIES ----- */
#include "ESPDMX.h"

#define dmxMaxChannel 512
#define defaultMax 32

#define DMXSPEED 250000
#define DMXFORMAT SERIAL_8N2
#define BREAKSPEED 83333
#define BREAKFORMAT SERIAL_8N1


void DMXESPSerial::init(HardwareSerial *serialPort, unsigned int sendPin, uint16_t chanQuant)
{
  if (chanQuant > dmxMaxChannel || chanQuant <= 0)
  {
    chanQuant = defaultMax;
  }
  chanSize = chanQuant;

  dmxData = (uint8_t *)malloc(chanSize * sizeof(uint8_t));

  this->serialPort = serialPort;
  this->sendPin = sendPin;

  this->serialPort->begin(DMXSPEED);
  pinMode(sendPin, OUTPUT);
  dmxStarted = true;
}

// Function to read DMX data
uint8_t DMXESPSerial::read(uint16_t Channel)
{
  if (dmxStarted == false)
  {
    return 0;
  }

  if (Channel < 1)
    Channel = 1;
  if (Channel > dmxMaxChannel)
    Channel = dmxMaxChannel;
  return (dmxData[Channel]);
}

// Function to send DMX data
void DMXESPSerial::write(uint16_t Channel, uint8_t value)
{
  if (dmxStarted == false)
  {
    return;
  }

  if (Channel < 1)
  {
    return;
  }
  if (Channel > chanSize)
  {
    Channel = chanSize;
  }

  dmxData[Channel] = value;
}

void DMXESPSerial::end()
{
  free(dmxData);
  chanSize = 0;
  this->serialPort->end();
  dmxStarted = false;
}

void DMXESPSerial::update()
{
  if (dmxStarted == false) {
    return;
  }

  // Send break
  digitalWrite(sendPin, HIGH);
  serialPort->begin(BREAKSPEED, BREAKFORMAT);
  serialPort->write(0);
  serialPort->flush();
  delay(1);
  serialPort->end();

  //send data
  serialPort->begin(DMXSPEED, DMXFORMAT);
  digitalWrite(sendPin, LOW);
  serialPort->write(dmxData, chanSize);
  serialPort->flush();
  delay(1);
  serialPort->end();
}
