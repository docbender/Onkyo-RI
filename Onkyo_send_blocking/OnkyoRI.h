//---------------------------------------------------------------------------
//
// Name:        OnkyoRI.h
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

#ifndef ONKYORI_H
#define ONKYORI_H 

class OnkyoRI
{
  public:
    OnkyoRI() {}; 
    OnkyoRI(int pin) {  _outputPin = pin; pinMode(_outputPin, OUTPUT);  digitalWrite(_outputPin, LOW); };    

    ~OnkyoRI() {  };  
    
    //send command message to device
    void send(int command);   
  
  private:
    //
    int _outputPin;
    
    //write message header 
    void writeHeader();
    //write message bit
    void writeBit(bool level);
    //write message footer
    void writeFooter();
};

#endif
