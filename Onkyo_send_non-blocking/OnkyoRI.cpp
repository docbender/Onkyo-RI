//---------------------------------------------------------------------------
//
// Name:        OnkyoRI.cpp
// Author:      Vita Tucek
// Created:     19.1.2016
// License:     MIT
// Description: Library for control Onkyo devices with RI port.
//              Message is composed from header, 12 databits and footer.
//
//              Non-blocking. Counting micros() is used instead using timer. 
// 
//---------------------------------------------------------------------------

#include "Arduino.h"
#include "OnkyoRI.h"

/// send command message to device
///
/// \param command  command to device
///
void OnkyoRI::send(short command)
{
  if(_processing) 
    return;
    
  _processing = true;
  _command = command;
  _state = 0;  
  digitalWrite(_outputPin,HIGH);
  _lasttime = micros();  
}

/// method for correct timing output pin state
/// should be call at least every milisecond
bool OnkyoRI::processing()
{
  if(!_processing) 
    return false;
    
   unsigned long time = micros();
   unsigned long diff;
   
   if(time >= _lasttime)
      diff = time - _lasttime;
   else
      diff = 0xffffffff - _lasttime + time;  
      
   //not enough time for nothing   
   if(diff < 1000)
      return true;      
      
   bool levelHi = digitalRead(_outputPin);
   
   //header
   if(_state==0)
   {
      if(levelHi)
      {
         if(diff >= 3000)
         {
            digitalWrite(_outputPin,LOW);
            _lasttime = micros();
         }
      }
      else
      {
         if(diff >= 1000)
         {
            digitalWrite(_outputPin,HIGH);
            _lasttime = micros();
            _state = 1;
         }
      }              
   }   
   //command
   else if(_state>0 && _state<13)
   {      
      if(levelHi)
      {         
         if(diff >= 1000)
         {
            digitalWrite(_outputPin,LOW);
            _lasttime = micros();
         }
      }
      else
      {
         short delay = 1000;
         if(_command & (0x800 >> (_state-1)))
            delay = 2000;
          
          
         if(diff >= delay)
         {
            digitalWrite(_outputPin,HIGH);
            _lasttime = micros();
            _state++;
         }                                 
      }
   }
   //footer
   else if(_state == 13)
   {
      if(levelHi)
      {         
         if(diff >= 1000)
         {
            digitalWrite(_outputPin,LOW);
            _lasttime = micros();
         }
      }
      else
      {
         if(diff >= 20000)
         {
            _processing = false;
            return false;
         }
      }
   }
    
   return true;  
}
   
