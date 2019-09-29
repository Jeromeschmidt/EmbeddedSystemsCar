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
  PORTD |= (1 << PIND4) | (1 << PIND7);
}

void wheelStop()
{
    PORTD = 0x00;
    PORTB = 0x00;
    delay1(20);
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
  PORTB |= (1 << PINB2);
  delayMicroseconds(10);
  PORTB &= ~(1 << PINB2);

  TCNT1 = 0;
  ICR1 = 0;
  delay1(60);
  temp = ICR1;
  return ((temp/3));//get scalar to use inches here
}

void sTurnLeft()
{
  PORTD |= (1 << PIND4) | (1 << PIND3);
  delay1(120);
  wheelStop();
}

void sTurnRight()
{
  PORTD |= (1 << PIND7);
  PORTB |= (1 << PINB1);
  delay1(110);
  wheelStop();
}

void wheelBackward()
{
  PORTB |= (1 << PINB1);
  PORTD |= (1 << PIND3);
}

void adjustLeft()
{
  OCR1A = OCR1A - 1;
  OCR1B = OCR1B + 1;
}
void adjustRight()
{
  OCR1A = OCR1A + 1;
  OCR1B = OCR1B - 1;
}


