# Onkyo-RI
Control Onkyo devices is possible among others through Remote Interactive port. This port is normally used for direct communication between two Onkyo devices (ex. receiver and CD player). But why not turn on the receiver automatically when you start your own player?

## Connection
To connect to the RI port is used 3.5mm mono jack. Tip is for data signal and sleeve is ground (GND). Data are sent via TTL logic. So it is easy to connect RI device to 5V MCU (Arduino). Just connect data signal to some output pin and connect GND between each other.

## Protocol
Protocol description could be found at:
*    http://lirc.sourceforge.net/remotes/onkyo/Remote_Interactive

or with grafical representation at:
*    http://fredboboss.free.fr/articles/onkyo_ri.php .

Protocol is pretty simple for implementation. In one message is transfered 12 bit code. This code represents action for target device. Most significant bit is send first.

## Library
There are two Onkyo-RI library:
* blocking - send() method blocks other program execution until whole command is sent. It takes up to 61 ms.
* non-blocking - send() method only start command sending. The execution is handled by processing() function. This function must be called periodically with maximum 1 ms period. Function return bool value about sending status (true - data is being sent, false - nothing to sent/sending is done). Before the command is completely sent other functions can be executed. Library use internaly Arduino micros() function, so no other timer is not blocked.

## RI codes
At mentioned sites are also listed codes for Onkyo devices. Unfortunnately none of the codes is not valid for my receiver TX-8020. To determine the valid codes I wrote a simple loop for Arduino (more below) that goes through the whole 12bit code range (0x0-0xFFF). Results are listed below commands.

### TX-8020 receiver 
Codes are valid for TX-8020 receiver. With a high probability it will work with other Onkyo receivers.
<table>
  <tr><td><b>Action</b></td><td><b>Command</b></td><td><b>Notes</b></td></tr>
  <tr><td>Input CD</td><td>0x20</td><td>Switch input to CD channel</td></tr>
  <tr><td>Turn On + CD</td><td>0x2F</td><td>Turn on receiver and select CD as input channel</td></tr>
  <tr><td>Input TAPE</td><td>0x70</td><td>Switch input to TAPE channel</td></tr>
  <tr><td>Turn On + TAPE</td><td>0x7F</td><td>Turn on receiver and select TAPE as input channel</td></tr>
  <tr><td>Input BD/DVD</td><td>0x120</td><td>Switch input to BD/DVD channel</td></tr>
  <tr><td>Turn On + BD/DVD</td><td>0x12F</td><td>Turn on receiver and select BD/DVD as input channel</td></tr>
  <tr><td>Input DOCK</td><td>0x170</td><td>Switch input to DOCK channel</td></tr>
  <tr><td>Turn On + DOCK</td><td>0x17F</td><td>Turn on receiver and select DOCK as input channel</td></tr>  
  <tr><td>Dimmer Hi</td><td>0x2B0</td><td>Set dimmer brightness to highest level</td></tr>
  <tr><td>Dimmer Mid</td><td>0x2B1</td><td>Set dimmer brightness to mid level</td></tr>  
  <tr><td>Dimmer Lo</td><td>0x2B2</td><td>Set dimmer brightness to lowest level</td></tr>  
  <tr><td>Dimmer Hi</td><td>0x2B8</td><td>Set dimmer brightness to highest level</td></tr>
  <tr><td>Dimmer Lo</td><td>0x2BF</td><td>Set dimmer brightness to lowest level</td></tr>    
  <tr><td>Turn Off</td><td>0x420</td><td>Turn off(set into standby) receiver</td></tr>  
  <tr><td>Test mode</td><td>0x421 - 0x424</td><td>Some kind of test modes. Leave test mode is possible by code 0x420 (Turn Off). Test modes provides clear of receiver setting.</td></tr>
  <tr><td>Radio search next</td><td>0x430</td><td>Tune next radio station when radio is selected.</td></tr>  
  <tr><td>Radio search previous</td><td>0x431</td><td>Tune previous radio station when radio is selected.</td></tr>  
  <tr><td>Radio Stereo/Mono</td><td>0x432</td><td>Switch between Stereo and Mono when FM radio is selected.</td></tr>  
  <tr><td>Radio station next</td><td>0x433</td><td>Jump to next stored radio station when radio is selected.</td></tr>  
  <tr><td>Radio station previous</td><td>0x434</td><td>Jump to previous stored radio station when radio is selected.</td></tr>
</table>

### TX-SR304 receiver 
Thanks to lonejeeper 
<table>
  <tr><td><b>Action</b></td><td><b>Command</b></td><td><b>Notes</b></td></tr>
  <tr><td>Input CD</td><td>0x20</td><td>Switch input to CD channel</td></tr>
  <tr><td>Input TAPE</td><td>0x70</td><td>Switch input to TAPE channel</td></tr>
  <tr><td>Input BD/DVD</td><td>0x120</td><td>Switch input to BD/DVD channel</td></tr>
  <tr><td>Input HDD</td><td>0x170</td><td>Switch input to HDD channel</td></tr>
  <tr><td>Input Video2</td><td>0x1A0</td><td>Switch input to Video2 channel</td></tr>  
  <tr><td>Vol Up</td><td>0x1A2</td><td>Volume Up</td></tr>
  <tr><td>Vol Down</td><td>0x1A3</td><td>Volume Down</td></tr>
  <tr><td>Mute</td><td>0x1A4</td><td>Mute</td></tr>
  <tr><td>Power OFF</td><td>0x1AE</td><td>Power OFF</td></tr>
  <tr><td>Power ON</td><td>0x1AF</td><td>Power ON</td></tr>
  <tr><td>Dim</td><td>0x2B0</td><td>Set dimmer brightness to highest level</td></tr>
  <tr><td>Dimer</td><td>0x2B1</td><td>Set dimmer brightness to mid level</td></tr>  
  <tr><td>Dimmest</td><td>0x2B2</td><td>Set dimmer brightness to lowest level</td></tr>  
  <tr><td>Dimmer Daytime</td><td>0x2B8</td><td>Set daytime brightness</td></tr>
  <tr><td>Dimmer Nighttime</td><td>0x2BF</td><td>Set nighttime brightness</td></tr>    
  <tr><td>Test mode</td><td>0x421 - 0x423, 0x430</td><td>RDS TEST</td></tr>  
</table>

## Test program
Program is located in repo folder Onkyo_test. It serves for check all codes (0x0 - 0xFFF) on target device in 500ms interval. For data line pin 10 is used as default. 

Actual checked code is sent as ASCII through serial port and can be displayed in terminal. For serial port 9600b/s is set.  Using terminal test program can be stopped, reset or user defined code could be used. 

Terminal commands:
* p - pause/run command sending
* r - reset loop (program start from 0)
* hexadecimal number - number in hexadecimal format represents code that user want to test on target device. From this code automatical procedure will continue.
