#include "motors.h"
#include <avr/io.h>
#include <Arduino.h>

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

void wheelForward()
{
  //right turnPORTD |= (1 << PIND4) | (1 << PIND3);//|= (1 << PIND0)|(1 << PIND7);//*pPORTD = 0x90;
  PORTD |= (1 << PIND4) | (1 << PIND7);
}

void wheelStop()
{
    PORTD = 0;
    PORTB = 0;
}

void initCar()
{
  int flag = 0;
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

double checkWall()
{
  int temp = 0;
  PORTB |= (1 << PINB5);//*pPORTB = 0x04;
  delayMicroseconds(10);
  PORTB &= ~(1 << PINB5);//*pPORTB = 0x00;

  TCNT1 = 0;
  ICR1 = 0;
  delay1(60);
  temp = ICR1;
  return ((temp/3));//get scalar to use inches here
}

void sTurnLeft()
{
  PORTD |= (1 << PIND4) | (1 << PIND3);
//  *pPORTD = 0x18;
//  delay1(40);
//  *pPORTD = 0x00;
//  delay1(100);
}

void sTurnRight()
{
  PORTD |= (1 << PIND7);
  PORTB |= (1 << PINB1);
//  //*pPORTD = 0x84;
//  delay1(40);
//  //*pPORTD = 0x00;
//  delay1(100);
}

void wheelBackward()
{
  PORTB |= (1 << PINB1);
  PORTD |= (1 << PIND3);
}

