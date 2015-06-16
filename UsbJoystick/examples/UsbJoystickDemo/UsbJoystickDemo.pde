/* This demo uses the UsbJoystick.h library, a library based on AVRUSB from ObDev
 * It demonstrates a 6-axis joystick with 4 buttons by reading all the analog ports and
 * digtal pins 9,10,11,and 12
 * Adapted from the UsbKeyboard.h library from Phil Lindsay
 *
 * by Michel Gutlich   19-8-2008
 */

#include <USBJoystick.h>

unsigned short a2dValue;
unsigned char high;
unsigned char low;
unsigned char temp;
unsigned char report[8];


void setup() {
  
  pinMode (9,INPUT);
  pinMode (10,INPUT);
  pinMode (11,INPUT);
  pinMode (12,INPUT);
  
}

void loop () {
  UsbJoystick.refresh(); // Let the AVRUSB driver do some houskeeping
  calculateReport(); // Jump to our port read routine that orders the values
  UsbJoystick.sendJoystick(report[0],report[1],reportBuffer[2],report[3],report[4],report[5],report[6],report[7]); // send the values
}

void calculateReport() {  //The values read from the analog ports have to be ordered in a way the HID protocol wants it; a bit confusing.

  a2dValue = analogRead(0);
  high = a2dValue >> 8;
  low = a2dValue & 255;
  report[0] = low;
  temp = high;

  a2dValue = analogRead(1);
  high = a2dValue >> 6;
  low = a2dValue & 63;
  report[1] = (low << 2) + temp;
  temp = high;

  a2dValue =analogRead(2);
  high = a2dValue >> 4;
  low = a2dValue & 15;
  report[2] = (low << 4) + temp;
  temp = high;

  a2dValue = analogRead(3);
  high = a2dValue >> 2;
  low = a2dValue & 3;
  report[3] = (low << 6) + temp;
  temp = high;

  high = 0;
  low = 0;
  report[4] = temp;
  temp = high;

  a2dValue = analogRead(4);
  high = a2dValue >> 8;
  low = a2dValue & 255;
  report[5] = low + temp;
  temp = high;

  a2dValue = analogRead(5);
  high = a2dValue >> 6;
  low = a2dValue & 63;
  report[6] = (low << 2) + temp;
  temp = high;

  // 4 buttons , tossed around

  report[7] = (temp & 15) + (digitalRead(9) << 4) +  (digitalRead(10) << 5) +  (digitalRead(11) << 6) +  (digitalRead(12) << 7);

}
