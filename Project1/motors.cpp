#include "motors.h"
#include <avr/io.h>
#include <Arduino.h>
int flag = 0;
int temp;

//registers for rangefinders
volatile uint8_t* pPORTB = (uint8_t*)0x25;
volatile uint8_t* pPINB = (uint8_t*)0x23;
volatile uint8_t* pDDRB = (uint8_t*)0x24;

//registers for motor control
volatile uint8_t* pPORTD = (uint8_t*)0x2B;
volatile uint8_t* pPIND = (uint8_t*)0x29;
volatile uint8_t* pDDRD = (uint8_t*)0x2A;

void delay1(int millisec)
{
  for(int j = 0; j < (millisec/5); j++)
  {
    for(uint16_t i = 0; i < 63900; i++)
    {
      //do nothing
      asm volatile("nop");
    }
  }
}
void sTurnLeft()
{ 
  *pPORTD = 0x18;
  delay1(40);
  *pPORTD = 0x00;
  delay1(100);
}

void sTurnRight()
{
  *pPORTD = 0x84;
  delay1(40);
  *pPORTD = 0x00;
  delay1(100);
}

//sees how far away the wall is
double checkWall()
{
  *pPORTB = 0x04;
  delayMicroseconds(10);
  *pPORTB = 0x00;

  TCNT1 = 0;
  ICR1 = 0;
  delay1(60);
  temp = ICR1;
  return ((temp/3));//get scalar to use inches here
}

//finds which side of the car the wall is
void initCar()
{
  while(flag == 0)
  {
    OCR2A = 3;
    delay1(500);
    Serial.print("RIGHT: ");
    Serial.println(checkWall());
    if(checkWall() < 15)
    {
      //wall is on right side
      flag = 1;
    }
    else if(flag == 0)
    {
      //wall is on left side
      OCR2A = 35;
      delay1(500);
      Serial.print("LEFT: ");
      Serial.println(checkWall());
      if(checkWall() < 15)
      {
        flag = 1;
      }
    }
    delay1(500);
  }
}
void wheelForward()
{
  *pPORTD = 0x90;
}

void wheelStop()
{
  *pPORTD = 0x00;
}

