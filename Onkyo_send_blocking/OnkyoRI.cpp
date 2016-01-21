//---------------------------------------------------------------------------
//
// Name:        OnkyoRI.cpp
// Author:      Vita Tucek
// Created:     19.1.2016
// License:     MIT
// Description: Library for control Onkyo devices with RI port.
//              Message is composed from header, 12 databits and footer.
//
//              For timing delay() function is used. That means until send
//              is completed program is blocked (max send() duration is 61ms).
//
//---------------------------------------------------------------------------

#include "Arduino.h"
#include "OnkyoRI.h"

/// send command message to device
///
/// \param command  command to device
///
void OnkyoRI::send(int command)
{
  writeHeader();
  
  for(int i=0;i<12;i++)
  {
    bool level = command & 0x800;
    command <<= 1;
    writeBit(level);
  }

  writeFooter();
}
   
/// write message header 
void OnkyoRI::writeHeader()
{
  digitalWrite(_outputPin,HIGH);
  delayMicroseconds(3000);
  digitalWrite(_outputPin,LOW);
  delayMicroseconds(1000);
}

/// write message bit
///
/// \param level  requested bit level (0/1)
///
void OnkyoRI::writeBit(bool level)
{
  digitalWrite(_outputPin,HIGH);
  delayMicroseconds(1000);  
  digitalWrite(_outputPin,LOW);
    
  if(level)
    delayMicroseconds(2000); 
  else
    delayMicroseconds(1000); 
}

/// write message footer
void OnkyoRI::writeFooter()
{
  digitalWrite(_outputPin,HIGH);
  delayMicroseconds(1000);
  digitalWrite(_outputPin,LOW);
  delay(20);
}
