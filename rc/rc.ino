/* RC-Controller-Code */
#include <XBee.h>
#include <Nextion.h>
#include <NextionPage.h>
#include <SoftwareSerial.h>

SoftwareSerial nextionSerial(10, 11); //RX, TX
Nextion nex(nextionSerial);
NextionPage pgMain(nex, 0, 0, "pgMain");
NextionPage pgPID (nex, 1, 0, "pgPID");

XBee xbee = XBee();

uint8_t payload[] = { 0, 0 };

// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(0x5678, payload, sizeof(payload));

// 64-bit addressing: This is the SH + SL address of remote XBee
//XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4008b490);
// unless you have MY on the receiving radio set to FFFF, this will be received as a RX16 packet
//Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

#define KILL_MOTORS 2 // analog pin connected to switch output
#define X_JOY1 0 // analog input of X-Joystick1 for speed

int x_joy1 = 0;

boolean checkSuccessResponse();

void setup()
{
  nextionSerial.begin(9600);
  nex.init();
  pgMain.show();
  
  pinMode(KILL_MOTORS, INPUT);
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
}

void loop()
{
  nex.poll();
  if(digitalRead(KILL_MOTORS) == 0) {
    // break down 10-bit reading into two bytes and place in payload
    x_joy1 = analogRead(X_JOY1);
    Serial.println(x_joy1);
    payload[0] = x_joy1 >> 8 & 0xff;
    payload[1] = x_joy1 & 0xff;          
  } else {
    payload[0] = 'm';
    Serial.print(digitalRead(KILL_MOTORS));
    Serial.println(" motors off");
  }
  xbee.send(tx);
  
  //checkSuccessResponse();*/
}
