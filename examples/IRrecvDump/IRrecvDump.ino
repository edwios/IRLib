/* Example program for from IRLib – an Arduino library for infrared encoding and decoding
 * Version 1.3   January 2014
 * Copyright 2014 by Chris Young http://cyborg5.com
 * Based on original example sketch for IRremote library 
 * Version 0.11 September, 2009
 * Copyright 2009 Ken Shirriff
 * http://www.righto.com/
 */
/*
 * IRLib: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 */
#include "application.h"
#include "IRLib.h"

int RECV_PIN = D5;
int IRID = 0;
int pin = TIMER_PWM_PIN;

IRrecv My_Receiver(RECV_PIN);
IRsend My_Sender;
IRdecode My_Decoder;
unsigned int Buffer[RAWBUF];

void setup()
{
  Serial.begin(9600);
  delay(2000);
  //while(!Serial);//delay for Leonardo
  if (!My_Receiver.enableIRIn()) {
    Serial.println("Time init failed");
  } // Start the receiver
  My_Decoder.UseExtnBuf(Buffer);
  My_Receiver.blink13(true); // blink LED
  Serial.println("Setup");
  if (PIN_MAP[pin].timer_ch == TIM_Channel_1) {
    Serial.println("Ch 1");
  } else if (PIN_MAP[pin].timer_ch == TIM_Channel_2) {
        Serial.println("Ch 2");
  } else if (PIN_MAP[pin].timer_ch == TIM_Channel_3) {
        Serial.println("Ch 3");
  } else if (PIN_MAP[pin].timer_ch == TIM_Channel_4) {
        Serial.println("Ch 4");    
  }
}

void loop() {
  if (My_Receiver.GetResults(&My_Decoder)) {
    //Restart the receiver so it can be capturing another code
    //while we are working on decoding this one.
    My_Receiver.resume(); 
    IRID = My_Decoder.decodeAndId();
      switch (IRID) {
        case 1:
          Serial.println("NEC");
          break;
        case 2:
          Serial.println("SONY");
          break;
        case 3:
          Serial.println("RC5");
          break;
        case 4:
          Serial.println("RC6");
          break;
        case 5:
          Serial.println("Panasonic Old");
          break;
        case 6:
          Serial.println("NECx");
          break;
         case 7:
          Serial.println("JVC");
          break;
       default:
          Serial.println("No ID found");
      }
    My_Decoder.DumpResults2();   
  }
  if (Serial.read() != -1) {
    //send a code  every time a character is received from the serial port
    //Sony DVD power A8BCA
    Serial.println("Sending code");
    // Pioneer TV On 0x55AA38C7
    // Apple TV Menu 0x77E1C029
    My_Sender.send(NEC,0x55AA38C7, 0);
    delay(100);
    Serial.println("Sending repeat");
    for (int i=0; i<3; i++) {
      My_Sender.send(NEC,REPEAT, 0);
      delay(100);
   }
    Serial.println("Done.");
    My_Receiver.enableIRIn();
  }
 
}

