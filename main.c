/* Name: main.c
 * Author: Francisco de Villalobos
 * Copyright: Francisco de Villalobos
 * License: Francisco de Villalobos
 */

#include "m_general.h"
#include <avr/io.h>
#include "Functions.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"
#include "Settings.h"

int indx;
int des_freq;
int duration;
char input;
char commands[3] = {0, 0, 0};
volatile char flag;
char flag_2;
int conta;



// Table of Values following a Sine Function. 256 Values.
const unsigned char sinewave_data[] = {
    0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9c,0x9f,0xa2,0xa5,0xa8,0xab,0xae,
    0xb0,0xb3,0xb6,0xb9,0xbc,0xbf,0xc1,0xc4,0xc7,0xc9,0xcc,0xce,0xd1,0xd3,0xd5,0xd8,
    0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xed,0xef,0xf0,0xf2,0xf3,0xf5,
    0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfc,0xfd,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfc,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,
    0xf6,0xf5,0xf3,0xf2,0xf0,0xef,0xed,0xec,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
    0xda,0xd8,0xd5,0xd3,0xd1,0xce,0xcc,0xc9,0xc7,0xc4,0xc1,0xbf,0xbc,0xb9,0xb6,0xb3,
    0xb0,0xae,0xab,0xa8,0xa5,0xa2,0x9f,0x9c,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
    0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x63,0x60,0x5d,0x5a,0x57,0x54,0x51,
    0x4f,0x4c,0x49,0x46,0x43,0x40,0x3e,0x3b,0x38,0x36,0x33,0x31,0x2e,0x2c,0x2a,0x27,
    0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x13,0x12,0x10,0x0f,0x0d,0x0c,0x0a,
    0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x03,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x05,0x06,0x07,0x08,
    0x09,0x0a,0x0c,0x0d,0x0f,0x10,0x12,0x13,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
    0x25,0x27,0x2a,0x2c,0x2e,0x31,0x33,0x36,0x38,0x3b,0x3e,0x40,0x43,0x46,0x49,0x4c,
    0x4f,0x51,0x54,0x57,0x5a,0x5d,0x60,0x63,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c};


int main(void)
{
    init();                             // Run Timer Initializacion Code
    m_bus_init();
	m_usb_init();                       // Initialize the USB connection.
    m_rf_open(1, 0x15, 3);
    set(DDRB, 0);
    set(DDRB, 1);
    set(DDRB, 2);
    set(DDRB, 3);
    set(DDRB, 7);
    set(DDRD, 3);
    set(DDRD, 4);
    set(DDRD, 5);
    set(DDRD, 6);
    set(DDRD, 7);
    
    //while(!m_usb_isconnected());      // wait for a connection

    while(1){
        
        if(m_usb_rx_available())
        {
            input = m_usb_rx_char();
            //m_usb_tx_int(input);
            m_usb_tx_string(" ");
            if (input == 119) {
                OCR3A = OCR3A - 1;
            }
            //if (input == 115) {
            //    OCR3A = OCR3A + 1;
            //}
            //if (input == 105) {
            //    OCR0A = OCR0A - 1;
            //}
            if (input == 107) {
                OCR0A = OCR0A + 1;
            }
            if (input == 105) {
                m_red(TOGGLE);
                //des_freq = 100;
                OCR3A = (float)62500/des_freq;
                duration = 100;
                flag_2 = 1;
                flag = 0;
                ;
            }
            if (input == 115) {
                des_freq += 10;
            }
            m_usb_tx_int(des_freq);
        }
        
        if (flag) {
            m_red(TOGGLE);
            m_rf_read(commands,3);
            des_freq = *(int*)&commands[0];
            des_freq /= 10;
            // Measured: 130 is 60 Hz. 20 is 370 Hz. THIS IS INT.
            OCR3A = 130 - (float)(des_freq - 60)/(float)2.8;
            duration = commands[2];
            flag_2 = 1;
            flag = 0;
        }
        
        // Max Sound duration is 255 centiseconds, which is 2.55 sec.
        if (flag_2) {
            // TCNT0 = 0;
            conta = 0;
            m_green(ON);
            set(DDRB, 6);
            clear(PORTD, 4);
            flag_2 = 0;
        }
        if (conta == (4*duration)) {
            m_green(OFF);
            clear(DDRB, 6);
            clear(PORTB, 6);
            set(PORTD, 4);
        }
        
        //if (TCNT0 == OCR0A) {
        if (check(TIFR0, OCF0A)){
            conta += 1;
            set(TIFR0,OCF0A);
        }
        
        /*
        if (check(ADCSRA,ADIF)) {
            set(ADCSRA, ADIF);
            if (ADC>100) {
                set(PORTB, 0);
            }
            else{
                clear(PORTB, 0);
            }
            if (ADC>200) {
                set(PORTB, 1);
            }
            else{
                clear(PORTB, 1);
            }
            if (ADC>300) {
                set(PORTB, 2);
            }
            else{
                clear(PORTB, 2);
            }
            if (ADC>400) {
                set(PORTB, 3);
            }
            else{
                clear(PORTB, 3);
            }
            if (ADC>500) {
                set(PORTB, 7);
            }
            else{
                clear(PORTB, 7);
            }
            if (ADC>600) {
                set(PORTD, 3);
            }
            else{
                clear(PORTD, 3);
            }
            if (ADC>700) {
                set(PORTD, 4);
            }
            else{
                clear(PORTD, 4);
            }
            if (ADC>800) {
                set(PORTD, 5);
            }
            else{
                clear(PORTD, 5);
            }
            if (ADC>900) {
                set(PORTD, 6);
            }
            else{
                clear(PORTD, 6);
            }
            if (ADC>1000) {
                set(PORTD, 7);
            }
            else{
                clear(PORTD, 7);
            }
        }
        */
        
        //m_usb_tx_string("\r");      // Carriage Return
        //m_usb_tx_uint(ADC);
        
        //m_usb_tx_string("\r");      // Carriage Return
        //m_usb_tx_hex(commands[0]);
        //m_usb_tx_uint(des_freq);
        //m_usb_tx_string(" ");
        //m_usb_tx_char(commands[2]);
        //m_usb_tx_string(" OCR3A: ");
        //m_usb_tx_uint(OCR3A);
        //m_usb_tx_string("             ");
        
        // Measured: 0x81 (129, we can use 130) is 60 Hz. 46 (70) is 110 Hz.
        // I get values from 60 to 110
        
        
        
        // Do only every time the Timer Overflows
        //while(!check(TIFR3, OCF3A));
        if (check(TIFR3, OCF3A)) {
            if(indx<255){
                indx += 1;
            }
            else{
                indx = 0;
            }
            //Define new value for Duty Cycle According to Sine Table
            OCR1B = sinewave_data[indx];
            set(TIFR3, OCF3A);              //Clear Flag
        }
        
        

        
        // Display information for the SCREEN.
       /*
        m_usb_tx_string("ADC Input Value: ");      //
        m_usb_tx_uint(ADC);         //transmit over USB
        m_usb_tx_string("    DIP Switch Inputs:");      //
        //m_usb_tx_uint(check(PIND,4));
        //m_usb_tx_string("  "); m_usb_tx_uint(check(PIND,5));
        //m_usb_tx_string("  "); m_usb_tx_uint(check(PIND,6));
        //m_usb_tx_string("  "); m_usb_tx_uint(check(PIND,7));
        m_usb_tx_string(" "); m_usb_tx_uint((check(PIND, 4) * 1 + check(PIND, 5) * 2 + check(PIND, 6) * 4 + check(PIND, 7) * 8 + 1));
        m_usb_tx_string("   Duty Cycle "); m_usb_tx_uint(((OCR1A - OCR1B)/OCR1A)*100);
        m_usb_tx_string("\r");      // Carriage Return
        //m_usb_tx_string("\n");      // Enter
        //m_usb_tx_string("\f");      // Form Feed
        //m_usb_tx_string("\f");      // Tab
        //m_usb_tx_string("\v");      // Vertical Tab
        
*/
    }
    return 0;                       /* never reached */
}

/*ISR(TIMER3_OVF_vect){
    
    if(index<255){
        index = index + 1;
    }
    else{
        index = 0;
    }
    OCR1B = sinewave_data[index];

}*/

ISR(INT2_vect){
    flag = 1;
}