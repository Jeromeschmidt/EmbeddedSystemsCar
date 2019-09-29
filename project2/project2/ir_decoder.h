/*--------------------------------------------------------------------
Name:   Jeffrey Falkinburg
Date:   24 Mar 19  
Course: CSCE 236 Embedded Systems (Spring 2019) 
File:   ir_decoder.h
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
#ifndef IR_DECODER_H
#define IR_DECODER_H

#include <avr/io.h>

#define		TRUE				1
#define		FALSE				0

/*-----------------------------------------------------------------
	Function prototypes found in project2.cpp
-----------------------------------------------------------------*/
void  IR_Decoder_Setup();


//-----------------------------------------------------------------
// Each ISC0n bit selects the external interrupt 0 edge for the corresponding I/O pin.
// Reference page 89 of the datasheet
//	Bits = 10 : The INT0 flag is set with a high-to-low (falling edge) transition
//  Bits = 11 : The INT0 flag is set with a low-to-high (rising edge) transition
//-----------------------------------------------------------------

//#define		IR_PIN			(P2IN & BIT6)
#define 	IR_DECODER_PIN  (PIND & (1 << PIND2))
#define 	IR_PIN    		(PIND2)
#define		HIGH_2_LOW		(EICRA = (EICRA & ~(1 << ISC00)) | (1 << ISC01))
#define		LOW_2_HIGH		(EICRA |= (1 << ISC00) | (1 << ISC01))


#define		averageLogic0Pulse	0x0492 
#define		averageLogic1Pulse	0x0D70 
#define		averageStartPulse	0x234E 
#define		minLogic0Pulse		averageLogic0Pulse - 5//100
#define		maxLogic0Pulse		averageLogic0Pulse + 5//100
#define		minLogic1Pulse		averageLogic1Pulse - 5//100
#define		maxLogic1Pulse		averageLogic1Pulse + 5//100
#define		minStartPulse		averageStartPulse - 5//100
#define		maxStartPulse		averageStartPulse + 5//100

/* Elegoo Remote */
#define		Elegoo_Up_Button		0x00FF629D
#define		Elegoo_Down_Button		
#define		Elegoo_Left_Button		
#define		Elegoo_Right_Button		
#define		Elegoo_Select_Button	


/* Other Remote */
#define		Up_Button		  0xFF629D
#define		Down_Button		0xFFA857
#define		Left_Button		0xFF22DD
#define		Right_Button	0xFFC23D
#define		OK_Button	    0xFF02FD
#define   Button_2      0xFF9867
#define   Button_4      0xFF30CF
#define   Button_6      0xFF7A85
#define   Button_8      0xFF38C7
#define   Button_5      0xFF18E7


#endif
