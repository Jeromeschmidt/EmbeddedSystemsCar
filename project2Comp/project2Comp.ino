#include "motors.h"

volatile uint8_t* iDDRB = (uint8_t*)0x24;
volatile uint8_t* iDDRD = (uint8_t*)0x2A;

int OCRAWall;
int OCRAFront;

void setup() {
  // put your setup code here, to run once:
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
  OCR0A = 82;//90;
  OCR0B = 82;
  initCar();
  OCRAWall = OCR2A;
  OCRAFront = 19;
}

int tempWall;
int tempFront;

void loop() {
//  // put your main code here, to run repeatedly:
  for(int i = 0; i < 23; i++)
  {
    tempWall = checkWall();
    //if wall on right side
    if(OCRAWall == 3)
    {
      //go away from wall
      if(tempWall < 7)
      {
        adjustLeft();
      }
      //go towards wall
      if(tempWall > 10)
      {
        adjustRight();
      }
      if(tempWall < 6)
      {
        OCR1A = OCR1A - 2;
        OCR1B = OCR1B + 2;
      }
      if(tempWall > 11)
      {
        OCR1A = OCR1A + 2;
        OCR1B = OCR1B - 2;
      }
    }
  
    //if wall of left side
    if(OCRAWall == 35)
    {
      //go away from wall
      if(tempWall < 7)
      {
        adjustRight();
      }
      //go towards wall
      if(tempWall > 10)
      {
        adjustLeft();
      }
    }
    wheelForward();
  }

  
  wheelStop();
//  move servo to face front
  OCR2A = OCRAFront;
  delay1(100);

  tempFront = checkWall();
  //box close in front
  if(tempFront < 25)
  {
    //navigate around box
    //wall on right
    if(OCRAWall == 3)
    {
      //turn left
      sTurnLeft();
      delay1(10);
      wheelStop();
      delay1(100);
      
      //go straight
      wheelForward();
      delay1(400);
      wheelStop();
      delay1(100);
      
      //turn right
      sTurnRight();
      delay1(10);
      wheelStop();
      delay1(100);
      
      //go straight until close to wall
      wheelForward();
      delay1(450);
      wheelStop();
      delay1(100);

      //turn right
      sTurnRight();
      delay1(10);
      wheelStop();
      delay1(100);

      //go straight until close to wall
      wheelForward();
      delay1(450);
      wheelStop();
      delay1(100);
      
      //turn left
      sTurnLeft();
      delay1(10);
      wheelStop();
      delay1(100);
    }
    //wall on left
    if(OCRAWall == 3)
    {
      //turn right
      sTurnRight();
      delay1(10);
      wheelStop();
      delay1(100);
      
      //go straight
      wheelForward();
      delay1(400);
      wheelStop();
      delay1(100);
      
      //turn left
      sTurnLeft();
      delay1(10);
      wheelStop();
      delay1(100);
      
      //go straight
      wheelForward();
      delay1(450);
      wheelStop();
      delay1(100);

      //turn right
      sTurnLeft();
      delay1(10);
      wheelStop();
      delay1(100);

      //go straight
      wheelForward();
      delay1(450);
      wheelStop();
      delay1(100);
      
      //turn right
      sTurnRight();
      delay1(10);
      wheelStop();
      delay1(100);
    }

    //move servo to face wall
    OCR2A = OCRAWall;
    delay1(100);

    while(1)
    {
      tempWall = checkWall();  
      if(tempWall > 10)
      {
        sTurnRight();
      }
      else if(tempWall < 7)
      {
        sTurnLeft();
      }
      else
      {
        //go straight
        OCR0A = 90;
        OCR0B = 82;
      }
      wheelForward();
    }
  }
}
