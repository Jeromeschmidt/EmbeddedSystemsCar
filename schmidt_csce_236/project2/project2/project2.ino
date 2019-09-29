/*--------------------------------------------------------------------
Name:   Jeffrey Falkinburg
Date:   24 Mar 19  
Course: CSCE 236 Embedded Systems (Spring 2019) 
File:   project2.iso
HW/Lab: Lab 5/Project 2, Decoding an IR Packet

Purp: Uses counters and interrupts to decode an IR packet for a 
    remote. 

Doc:  <list the names of the people who you helped>
    <list the names of the people who assisted you>

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <avr/io.h>
#include "ir_decoder.h"
#include "motors.h"

#define SAMPLE_SIZE       34
#define   BIT0            0x00000001
volatile uint32_t irPacket;
volatile uint8_t  newIrPacket = FALSE;
volatile uint16_t packetData[34];
volatile uint8_t  packetIndex = 0;
volatile uint8_t* iDDRD = (uint8_t*)0x2A;
int flag = 0;//for wheel speed

void setup() {
    //init Timer0 for Wheel pwm
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << CS02);
  
  //set motor pins to outputs
//  *iDDRD = 0xFF;
  PORTD |= (1 << PIND2);
  *iDDRD = 0xFF;
  DDRD &= ~(1 << PIND2);//0 | (1 << PIND2);
//  *iDDRB = 0x04;

  //DDRB
  DDRB |= (1 << DDB3);
  Serial.begin(9600);
  Serial.print("Starting up...");
  IR_Decoder_Setup();
  sei(); /* Enable Global Interrupts*/
}

/*
 * main loop
 */
void loop() {

  /* Check if new IR packet has arrived bad way */ 
//  if (packetIndex > 33) {
//    packetIndex = 0;
//  } // end if new IR packet arrived
// Check if new IR packet has arrived better way 
  if (newIrPacket == TRUE){
//code to convert packetData[] counts to 32 bit irpacket
    for(int i = 2; i <= 33; i++){
      if(packetData[i] > 200){
        irPacket = ((irPacket << 1 ) | 1);
      }else{
        irPacket = irPacket<<1;
      } 
    }
    Serial.print("\nButton value is 0x");
  Serial.println(irPacket, HEX);
    newIrPacket = FALSE;
  } //end if newIrPacket
  if(flag == 0)
  {
    OCR0A = 90;
    OCR0B = 82;
  }
  else
  {
    OCR0A = 150;
    OCR0B = 135;
  }

  /* Do something with new IR packet */ 
  switch(irPacket)
  {
    case(Up_Button):
      Serial.println("Up_Button");
      irPacket = 0;
      //forward
      wheelForward();
      break;
    case(Down_Button):
      Serial.println("Down_Button");
      irPacket = 0;
      //backward
      wheelBackward();
      break;
    case(Left_Button):
      Serial.println("Left_Button");
      irPacket = 0;
      //left
      sTurnLeft();
      break;
    case(Right_Button):
      Serial.println("Right_Button");
      irPacket = 0;
      //right
      sTurnRight();
      break;
    case(OK_Button):
      Serial.println("OK_Button");
      irPacket = 0;
      if(flag == 0)
      {
        flag = 1;
      }
      else
      {
        flag = 0;
      }
      break;
    case(Button_2):
      Serial.println("Button_2");
      irPacket = 0;
      //forward fast
      wheelForward();
      break;
    case(Button_4):
      Serial.println("Button_4");
      irPacket = 0;
      //fast left
      sTurnLeft();
      break;
    case(Button_6):
      Serial.println("Button_6");
      irPacket = 0;
      //fast right
      sTurnRight();
      break;
    case(Button_8):
      Serial.println("Button_8");
      irPacket = 0;
      //fast backward
      wheelBackward();
      break;
    case(Button_5):
      Serial.println("Button_5");
      irPacket = 0;
      if(flag == 0)
      {
        flag = 1;
      }
      else
      {
        flag = 0;
      }
      break;
  }
  delay1(40);
  wheelStop();
  
//  if (irPacket == Up_Button || irPacket == Elegoo_Up_Button){    
////    moveForward();
//    Serial.println("Up_Button");
//    irPacket = 0;     // Clear irPacket
//  } //end if

}// end loop

// -----------------------------------------------------------------------
// Since the IR decoder is connected to INT0, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on INT0
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we are going into the logic 1 half bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------

ISR(INT0_vect){

  uint8_t   pin;
  uint16_t  pulseDuration;      // The timer is 16-bits

  if (IR_DECODER_PIN)   pin=1;  else pin=0;

  switch (pin) {          // read the current pin level
    case 0:           // !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
      pulseDuration = TCNT1;    //**Note** Timer Count register is 16-bits
      packetData[packetIndex++] = pulseDuration;
      LOW_2_HIGH;               // Set up pin INT0 on positive edge
                //Disable interrupt on match with OCR1A
      TIMSK1 = (0 << OCIE1A);
      break;

    case 1:             // !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
      TCNT1 = 0x0000;           // time measurements are based at time 
                // Clear Timer/Counter1, Output Compare A Match Flag by writing 1 
      TCNT1H = 0;
      TCNT1L = 0;
      TIFR1 = (1 << 1);
                // Enable interrupt on match with OCR1A
      TIMSK1 = (1 << OCIE1A);
      HIGH_2_LOW;               // Set up pin INT0 on falling edge
      break;
  } // end switch

} // end pinChange ISR

// -----------------------------------------------------------------------
//            0 half-bit  1 half-bit    TIMER 1 COUNTS    TIMER 1 COUNTS
//  Logic 0   xxx
//  Logic 1
//  Start
//  End
//
// -----------------------------------------------------------------------
/**
 * This function is called whenever the timer 1 output compare match OCR1A
 * is generated.
 **/
ISR(TIMER1_COMPA_vect){

              // Disable interrupt on match with OCR1A
              TIMSK1 = (0 << OCIE1A);
              packetIndex = 0;
              newIrPacket = TRUE;
              //irPacket = 0x00FF629D;
}
