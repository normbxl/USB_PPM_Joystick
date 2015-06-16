/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * UsbJoystick.h is a library for Arduino that wraps around the AVRUSB driver from ObDev
 *
 * Adapted from the UsbKeyboard.h library from Phil Lindsay
 *
 * by Michel Gutlich
 *
 */
#ifndef __UsbJoystick_h__
#define __UsbJoystick_h__

#include <avr/pgmspace.h>

#include "usbdrv.h"

#include <avr/interrupt.h>
#include <string.h>
// typedef uint8_t byte; // avoid int() issue

static uchar reportBuffer[8];				/* buffer for HID reports */

static uchar    idleRate;           // in 4 ms units 


PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {  // USB report descriptor for a 6-axis, 4 button joystick
0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
0x09, 0x04,        // USAGE (Joystick)
0xa1, 0x01,        // COLLECTION (Application)

0x09, 0x30,        // USAGE (X)
0x15, 0x00,        // LOGICAL_MINIMUM (0)
0x26, 0xff, 0x03, 	           //	  Logical Maximum (1024)
0x75, 0x0a,        // REPORT_SIZE (10)
0x95, 0x01,        // REPORT_COUNT (1)
0x81, 0x02,        // INPUT (Data,Var,Abs)

0x09, 0x31,        // USAGE (Y)
0x15, 0x00,        // LOGICAL_MINIMUM (0)
0x26, 0xff, 0x03, 	           //	  Logical Maximum (1024)
0x75, 0x0a,        // REPORT_SIZE (10)
0x95, 0x01,        // REPORT_COUNT (1)
0x81, 0x02,        // INPUT (Data,Var,Abs)

0x09, 0x32,        // USAGE (Z)
0x15, 0x00,        // LOGICAL_MINIMUM (0)
0x26, 0xff, 0x03, 	           //	  Logical Maximum (1024)
0x75, 0x0a,        // REPORT_SIZE (10)
0x95, 0x01,        // REPORT_COUNT (1)
0x81, 0x02,        // INPUT (Data,Var,Abs)

0x09, 0x33,        // USAGE (Rotate X)
0x15, 0x00,        // LOGICAL_MINIMUM (0)
0x26, 0xff, 0x03, 	           //	  Logical Maximum (1024)
0x75, 0x0a,        // REPORT_SIZE (10)
0x95, 0x01,        // REPORT_COUNT (1)
0x81, 0x02,        // INPUT (Data,Var,Abs)

0x09, 0x34,        // USAGE (Rotate Y)
0x15, 0x00,        // LOGICAL_MINIMUM (0)
0x26, 0xff, 0x03, 	           //	  Logical Maximum (1024)
0x75, 0x0a,        // REPORT_SIZE (10)
0x95, 0x01,        // REPORT_COUNT (1)
0x81, 0x02,        // INPUT (Data,Var,Abs)

0x09, 0x35,        // USAGE (Rotate Z)
0x15, 0x00,        // LOGICAL_MINIMUM (0)
0x26, 0xff, 0x03, 	           //	  Logical Maximum (1024)
0x75, 0x0a,        // REPORT_SIZE (10)
0x95, 0x01,        // REPORT_COUNT (1)
0x81, 0x02,        // INPUT (Data,Var,Abs)

0x05, 0x09,			   //   Usage_Page (Button)
0x19, 0x01,			   //   Usage_Minimum (Button 1)
0x29, 0x04,			   //   Usage_Maximum (Button 4)
0x15, 0x00,			   //   Logical_Minimum (0)
0x25, 0x01,			   //   Logical_Maximum (1)
0x75, 0x01,			   //   Report_Size (1)
0x95, 0x04,			   //   Report_Count (4)
0x55, 0x00,			   //   Unit_Exponent (0)
0x65, 0x00,			   //   Unit (None)
0x81, 0x02,			   //   Input (Data, Const, Abs)

0xc0               // END_COLLECTION
};


class UsbJoystickDevice {
 public:
  UsbJoystickDevice () {
    
    usbInit();
      
    sei();

      }
    
  void refresh() {
    usbPoll();
  }
    
	void sendJoystick(){
		sendJoystick(reportBuffer[0],reportBuffer[1],reportBuffer[2],reportBuffer[3],reportBuffer[4],reportBuffer[5],reportBuffer[6],reportBuffer[7]);
	}
		
	
  void sendJoystick(uchar val0, uchar val1, uchar val2, uchar val3, uchar val4 , uchar val5 , uchar val6, uchar val7) {
      
	reportBuffer[0] = val0;
    reportBuffer[1] = val1;
	reportBuffer[2] = val2;
	reportBuffer[3] = val3;
	reportBuffer[4] = val4;
	reportBuffer[5] = val5;
	reportBuffer[6] = val6;
	reportBuffer[7] = val7;

	  if(usbInterruptIsReady()) {
		  usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
	  }
  }
    
  //private: TODO: Make friend?
  uchar    reportBuffer[8];    // buffer for HID reports

};

UsbJoystickDevice UsbJoystick = UsbJoystickDevice();

#ifdef __cplusplus
extern "C"{
#endif 
	
  // USB_PUBLIC uchar usbFunctionSetup
uchar usbFunctionSetup(uchar data[8]) 
  {
	usbRequest_t    *rq = (usbRequest_t *)((void *)data);
	  
    usbMsgPtr = UsbJoystick.reportBuffer; //
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){ /* class request type */
      if(rq->bRequest == USBRQ_HID_GET_REPORT){ /* wValue: ReportType (highbyte), ReportID (lowbyte) */
		  reportBuffer[0]= 0;
		  reportBuffer[1]= 0;
		  reportBuffer[2]= 0;
		  reportBuffer[3]= 0;
		  reportBuffer[4]= 0;
		  reportBuffer[5]= 0;
		  reportBuffer[6]= 0;
		  reportBuffer[7]= 0;
	   
	return sizeof(reportBuffer);

      }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
		usbMsgPtr = &idleRate;
		return 1;
      }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
		idleRate = rq->wValue.bytes[1];
      }else{
		  /* no vendor specific requests implemented */
	}
    return 0;
  }
}
#ifdef __cplusplus
} // extern "C"
#endif


#endif // __UsbJoystick_h__
