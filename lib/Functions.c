//
//  Functions.c
//  Test_1
//
//  Created by Francisco de Villalobos on 9/25/13.
//  Copyright (c) 2013 Francisco de Villalobos. All rights reserved.
//

#include <stdio.h>
#include "Functions.h"
#include "m_general.h"

void counter(double count)
{
    double i = 0;
    for (i=0; i<count; i++){
    }
}

char blinker(double perc){
    
    double Dc;      //Duty Cycle Prescaler
    
    //Function to dynamically adjust the frecuency
    //62500 for 0.5Hz, then the counter goes down
    OCR1A = -(62500-50)/0x03FF * perc + 62500;
    
    // Set the duty Cycle accordingly to DIP switches
    Dc = 100 - (check(PIND, 4) * 1 + check(PIND, 5) * 2 + check(PIND, 6) * 4 + check(PIND, 7) * 8 + 1) * 6;
    OCR1B = OCR1A * (Dc/100);
    
    return OCR1A;
}

void timer_init(char timer_num, char timer_presc, char timer_mode, char interr_mode){
    
#ifndef timer_presc
#define timer_presc     1
#endif
    
#ifndef timer_mode
#define timer_mode      0
#endif
    
#ifndef interr_mode
#define interr_mode     1
#endif
    
    int a;
    
    switch(timer_num){
            case 1:
            a = 1;
            break;
            case 2:
            a=2;
            break;
    }
    
}