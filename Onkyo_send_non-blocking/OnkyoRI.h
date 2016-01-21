//---------------------------------------------------------------------------
//
// Name:        OnkyoRI.h
// Author:      Vita Tucek
// Created:     19.1.2016
// License:     MIT
// Description: Library for control Onkyo devices with RI port.
//              Message is composed from header, 12 databits and footer. 
//
//              Non-blocking. Counting micros() is used instead blocking timer.
//
//---------------------------------------------------------------------------

#ifndef ONKYORI_H
#define ONKYORI_H 

class OnkyoRI
{
  public:
    OnkyoRI() {}; 
    OnkyoRI(byte pin) {  _processing = false; _outputPin = pin; pinMode(_outputPin, OUTPUT);  digitalWrite(_outputPin, LOW); };    

    ~OnkyoRI() {  };  
    
    //send command message to device
    void send(short command);   
    //method which should be call at least every milisecond     
    bool processing();
  
  private:
    //
    byte _outputPin;
    short _command;
    bool _processing; 
    unsigned long _lasttime;
    byte _state;     

};

#endif
