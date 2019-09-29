#include "motors.h"

volatile uint8_t* iDDRB = (uint8_t*)0x24;
volatile uint8_t* iDDRD = (uint8_t*)0x2A;

void setup() 
{
  //init Timer0 for Wheel pwm
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << CS02);

  //init Timer1 for rangefinder
  TCCR1A = (1 << COM1A1) | (1 << WGM13) | (1 << WGM12) | (1 << WGM11);
  TCCR1B = (1 << CS12) | (1 << CS10); // N = 1024
  TCCR1C = 0;
  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;  
  //Make sure interrupts are disabled 
  TIMSK1 = 0;
  TIFR1 = 0;

  // set up timer2 for servo
  TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // N = 1024
  
  //set motor pins to outputs
  *iDDRD = 0xFF;
  *iDDRB = 0x04;
  DDRB |= (1 << DDB3);
  Serial.begin(9600);
  initCar();
}
int tempWall;
void loop()
{
  //straight
  //OCR0A = 90;
  //OCR0B = 82;
  tempWall = checkWall();
  Serial.print("tempWall: ");
  Serial.println(tempWall);
  if(tempWall > 9)
  {
    if(tempWall > 10)
    {
      wheelStop();
      delay1(100);
      if(OCR2A == 35)
      {
        sTurnLeft();
        delay1(20);
      }
      else
      {
        sTurnRight();
        delay1(20);
      }
    }
    // go towards wall
    if(OCR2A == 3)
    {
      //go towards wall on right
      OCR0A = 90;
      OCR0B = 84;
    }
    else
    {
      //go towards wall on left
      OCR0A = 88;
      OCR0B = 82;
    }
  }
  else if(tempWall < 7)
  {
    if(tempWall < 6)
    {
      wheelStop();
      delay1(100);
      if(OCR2A == 35)
      {
        sTurnRight();
        delay1(20);
      }
      else
      {
        sTurnLeft();
        delay1(20);
      }
    }
    //go away from wall
    if(OCR2A == 35)
    {
      //go away from wall on right
      OCR0A = 88;
      OCR0B = 82;
    }
    else
    {
      //go away from wall on left
      OCR0A = 90;
      OCR0B = 84;
    }
  }
  else
  {
    //go straight
    OCR0A = 90;
    OCR0B = 82;
  }
  wheelForward();
}
