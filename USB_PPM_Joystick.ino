/*
Anschluss: 
  D+ auf D2
  D- auf D3
  1.5k D- PullUp auf D4
  PPM-In auf D8
*/

#include "UsbJoystick.h"

#define CHANNELS 8 
#define READ_PIN 8

unsigned short a2dValue;
unsigned char high;
unsigned char low;
unsigned char temp;
unsigned char report[8];

unsigned int channel[CHANNELS]; 
unsigned int lastReadChannel[CHANNELS]; 
unsigned int counter=0; 

unsigned int pulseUpTime, pulseLowTime;
boolean lastPinState = LOW;
int numCurPulse=-1;

void setup() {
  pinMode (READ_PIN, INPUT);
  usbDeviceConnect();
  lastPinState = digitalRead(READ_PIN);
}

void loop () {
  UsbJoystick.refresh(); // Let the AVRUSB driver do some houskeeping
  calculateReport(); // Jump to our port read routine that orders the values
  UsbJoystick.sendJoystick(report[0],report[1],report[2],report[3],report[4],report[5],report[6],report[7]); // send the values
}

void calculateReport() {  //The values read from the analog ports have to be ordered in a way the HID protocol wants it; a bit confusing.
  boolean curPinState = digitalRead(READ_PIN);

  // slope change
  if (curPinState != lastPinState) {
    unsigned long pulseLength;
    // rising
    if (curPinState==HIGH && lastPinState==LOW) {
      pulseUpTime=micros();
      pulseLength=pulseUpTime-pulseLowTime;
    }
    else { // falling: curPinState==LOW && lastPinState==HIGH
      pulseLowTime=micros();
      pulseLength=pulseLowTime-pulseUpTime;
    }
    // rising und pause > 3000µs
    if (pulseLength>3000 && curPinState==HIGH) {
      numCurPulse=0;
    }
    else if (curPinState == HIGH && numCurPulse < CHANNELS && pulseLength<2000) {
      unsigned int newValue = constrain(pulseLength-450,1,1023);
      if (abs((int)newValue-(int)channel[numCurPulse])<300 || channel[numCurPulse]==0) {
        channel[numCurPulse] = newValue;
      }
      numCurPulse += 1;
    }
  }
  lastPinState = curPinState;
  
  a2dValue = channel[1];
  high = a2dValue >> 8;
  low = a2dValue & 255;
  report[0] = low;
  temp = high;

  a2dValue = channel[0];
  high = a2dValue >> 6;
  low = a2dValue & 63;
  report[1] = (low << 2) + temp;
  temp = high;

  a2dValue =channel[2];
  high = a2dValue >> 4;
  low = a2dValue & 15;
  report[2] = (low << 4) + temp;
  temp = high;

  a2dValue = channel[3];
  high = a2dValue >> 2;
  low = a2dValue & 3;
  report[3] = (low << 6) + temp;
  temp = high;

  high = 0;
  low = 0;
  report[4] = temp;
  temp = high;

  a2dValue = channel[4];
  high = a2dValue >> 8;
  low = a2dValue & 255;
  report[5] = low + temp;
  temp = high;

  a2dValue = channel[5];
  high = a2dValue >> 6;
  low = a2dValue & 63;
  report[6] = (low << 2) + temp;
  temp = high;

  // 4 buttons , tossed around

  report[7] = (temp & 15);

}
