//
//  Settings.c
//  Speakers
//
//  Created by Francisco de Villalobos on 10/6/13.
//  Copyright (c) 2013 Francisco de Villalobos. All rights reserved.
//

#include <stdio.h>
#include "m_general.h"

void init(void){
    m_clockdivide(0);
    
    // Define Timer 1 Settings
    // We are going to use Timer 1 as the PWM modulated signal to generate
    // the filtered sine signal output.
    
    // Define Prescaler as 8.
    clear(TCCR1B,CS12);
    set(TCCR1B,CS11);
    clear(TCCR1B,CS10);
    
    // Set Mode 15: UP to OCR1A, PWM mode
    set(TCCR1B,WGM13);
    set(TCCR1B,WGM12);
    set(TCCR1A,WGM11);
    set(TCCR1A,WGM10);
    
    // Toggle Mode on OC1B, which is multiplexed to B6
    set(DDRB,6);
    set(TCCR1A,COM1B1);
    set(TCCR1A,COM1B0);
    
    // Set the OCR1A and OCR1B. (Only Initial values. Will Change dynamically)
    OCR1A = 0x00FF;
    OCR1B = OCR1A/5;
    
    // Set Interrupt in Overflow
    // set(TIMSK1,TOIE1);
    
    
    //
    // Define Timer 3 Settings
    // We are going to use Timer 3 to vary the speed of the duty cycle change
    // This timer will define how fast does the duty cycle goes through the
    // table of values, changing the output sine wave frequency.
    
    // Define Prescaler as 1.
    clear(TCCR3B,CS32);
    clear(TCCR3B,CS31);
    set(TCCR3B,CS30);
    
    // Set Mode 04: UP to OCR3A.
    clear(TCCR3B,WGM33);
    set(TCCR3B,WGM32);
    clear(TCCR3A,WGM31);
    clear(TCCR3A,WGM30);
    
    // Toggle C6
    set(DDRC,6);
    clear(TCCR3A,COM3A1);
    set(TCCR3A,COM3A0);
    
    // Set the OCR1A and OCR1B.
    // Observed: 5F is nearly 80 Hz. 4F is 98 Hz. 3F is 122 Hz.
    // Measured: 81 is 60 Hz. 46 is 110 Hz. THIS IS HEX
    // Measured: 130 is 60 Hz. 20 is 370 Hz. THIS IS INT.
    OCR3A = 0x003F;
    
    //Set Interrupt Enable in Overflow
    //set(TIMSK3,TOIE3);
    
 
    
    // Timer 0 for sound duration
    
    // 1024 Prescaler
    set(TCCR0B, CS02);
    clear(TCCR0B, CS01);
    set(TCCR0B, CS00);
    
    // UP to OCR0A
    clear(TCCR0B, WGM02);
    set(TCCR0A, WGM01);
    clear(TCCR0A, WGM00);
    OCR0A = 0x004D;         //16 is 200Hz. 9B is 50 Hz. 4D is 100 Hz which is 1 centi second
    
    // No Change Pin B7
    clear(TCCR0A, COM0A1);
    clear(TCCR0A, COM0A0);
    
    
    
    
    
    
    

    //ADC Initialization
    
    //AREF Voltage Reference
    clear(ADMUX,REFS1);
    clear(ADMUX,REFS0);
    
    //ADC Prescaler (Keep between 50 and 200kHz)
    set(ADCSRA,ADPS2);
    set(ADCSRA,ADPS1);
    set(ADCSRA,ADPS0);
    
    //Disable inputs in Channel DIDR0, bit 0 for F0
    set(DIDR0,ADC0D);
    
    //Enable Auto-Conversion
    set(ADCSRA,ADATE);
    
    //Connect F0 to the ADC Unit.
    clear(ADCSRA,ADEN);
    clear(ADCSRB,MUX5);
    clear(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    clear(ADMUX,MUX0);
    
    //Enable ADC Subsystem
    set(ADCSRA,ADEN);
    
    //Start Conversion (Only 1 time needed to start because of Auto-Conversion)
    set(ADCSRA,ADSC);
    
    // Returns Conversion from 0 to 1023. In 10 steps we get every 100 counts.

}