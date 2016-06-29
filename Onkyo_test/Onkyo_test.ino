//---------------------------------------------------------------------------
//
// Name:        Onkyo_test.cpp
// Author:      Vita Tucek
// Created:     10.1.2016
// License:     MIT
// Description: Test loop for check all RI codes on tagret Onkyo device
//
//---------------------------------------------------------------------------

#include "Arduino.h"

// UART
#define UART_SPEED  9600
// ONKYO
#define ONKYO_PIN 10

int i=0;
bool pause = 0;

void setup() {
  Serial.begin(UART_SPEED);

  //nastaveni ONKYO vystupu
  pinMode(ONKYO_PIN, OUTPUT);
  digitalWrite(ONKYO_PIN, LOW);

  Serial.println("Ready");
}

void loop() {
  int cmd = 0;
  
  if(Serial.available())
  {
    char c = Serial.read();

    if(c == 'p')
    {
      pause = !pause;
    }
    if(c == 'r')
    {
      i=0;
      pause = false;
    }
    if(c >= '0' and c <= '9' || c >= 'a' and c <= 'f' || c >= 'A' and c <= 'F')
    {
      if(c >= '0' and c <= '9')
        cmd = c-0x30;
      else if(c >= 'a' and c <= 'f')
        cmd = c-0x61+0xA;
      else if(c >= 'A' and c <= 'F')
        cmd = c-0x41+0xA;        

      delay(10);
        
      while(Serial.available())
      {
        c = Serial.read();

        if(c >= '0' and c <= '9' || c >= 'a' and c <= 'f' || c >= 'A' and c <= 'F')
        {
          cmd <<=4;
  
          if(c >= '0' and c <= '9')
            cmd += c-0x30;
          else if(c >= 'a' and c <= 'f')
            cmd += c-0x61+0xA;
          else if(c >= 'A' and c <= 'F')
            cmd += c-0x41+0xA;             
  
          delay(10);
        }
      }
    }
  }

  if(!pause && cmd==0)
  {
    if(i<0xfff)
      i++;
     else
      i=0; 
     
    onkyoSend(i); 
  
    Serial.print("0x");
    Serial.println(i,HEX);
    delay(500);    
  }
  else if(cmd>0)
  {
    onkyoSend(cmd); 
  
    Serial.print("user - 0x");
    Serial.println(cmd,HEX);  

    i = cmd;  
  }
  
  
//  if(Serial.available())
//  {
//    String input = Serial.readString();
//    //cut new line cahrs
//    input.trim();
//
//    Serial.print(input);
//    Serial.println(" processing");    
//    
//    if(input == "on")
//      onkyoSend(0x00E9);      
//    else if(input == "off")
//      onkyoSend(0x00EA);      
//    else if(input == "mute")
//      onkyoSend(0x0AA1);      
//    else if(input == "amp1")
//      onkyoSend(0x0AA2);    
//    else if(input == "amp2")
//      onkyoSend(0x0AA0);    
//    else if(input == "dim")
//      onkyoSend(0x01B2);       
//    else if(input == "cdon")
//      onkyoSend(0x0604);         
//    else
//      Serial.println("Unknown command");            
//  }
//  else
//  {
//    delay(10);    
//  }
}

void onkyoSend(int command)
{
  onkyoWriteHeader();

  
  for(int i=0;i<12;i++)
  {
    bool level = command & 0x800;
    command <<= 1;
    onkyoWriteBit(level);
  }

  onkyoWriteFooter();
}

void onkyoWriteHeader()
{
  //Serial.println(micros());
  digitalWrite(ONKYO_PIN,HIGH);
  delayMicroseconds(3000);
  digitalWrite(ONKYO_PIN,LOW);
  delayMicroseconds(1000);
  //Serial.println(micros());
}
void onkyoWriteBit(bool level)
{
  digitalWrite(ONKYO_PIN,HIGH);
  delayMicroseconds(1000);  
  digitalWrite(ONKYO_PIN,LOW);
    
  if(level)
    delayMicroseconds(2000); 
  else
    delayMicroseconds(1000); 
}
void onkyoWriteFooter()
{
  digitalWrite(ONKYO_PIN,HIGH);
  delayMicroseconds(1000);
  digitalWrite(ONKYO_PIN,LOW);
  delay(20);
}
