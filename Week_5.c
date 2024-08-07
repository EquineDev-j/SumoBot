/*
 * Week_6.c
 *
 *  Created on: January 15, 2019
 *      Author: jannel bennett
 *      EE2931 - Embedded Systems
 */
#include "msp.h"
#include "msoe_lib_all.h"
#include <stdio.h>

///Function Prototypes///Function Prototypes///Function Prototypes///Function Prototypes///

void pin_setup(void);

void setup_TIMERA(void);
void timerA_setup(void);

void LCD_setup (void);

uint8_t Sensor_4(void);
uint8_t Sensor_3(void);
uint8_t Sensor_2(void);
uint8_t Sensor_1(void);


void main(void){

    int i;

    LCD_Config(); // configure ports
    LCD_clear(); // clear LCD

    pin_setup();
    setup_TIMERA();
    timerA_setup();




    while(1){

        //Move Forward//
        P3->OUT=0x40; //  set port 3 bit 7 to one to set high ~ move forward
        P2->OUT=0x40; // set port 2 bit 6 to one to set high ~ move forward
        TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value

        uint8_t pattern_code=0;
        pattern_code+=Sensor_4();
        pattern_code+=Sensor_3();
        pattern_code+=Sensor_2();
        pattern_code+=Sensor_1();

        LCD_goto_xy(0,0);
        LCD_print_bin8(pattern_code);


        if(pattern_code==1){ // if the sensors 4 ~ Front Right Sensors ~ see white

            // Move backward Right //
            P3->OUT=0x40; //  set port 3 bit 6 to one to set high ~ move forward
            P2->OUT=0x80; // set port 2 bit 7 to one to set high ~ move backward
            TIMER_A2->CCR[3]=(1714*80/100); // compare value start at 80% duty cycle
            TIMER_A2->CCR[4]=(1714*80/100); //  @ 80% duty cycle count value
            for(i=100000;i>0;i--){}  //Lousy Delay
        }

        if(pattern_code==2){ // if the sensors 3 ~ Front Left Sensors ~ see white

            // Move backward Left //
            P3->OUT=0x80; //  set port 3 bit 7 to one to set high ~ move backward
            P2->OUT=0x40; // set port 2 bit 6 to one to set high ~ move forward
            TIMER_A2->CCR[3]=(1714*80/100); // compare value start at 80% duty cycle
            TIMER_A2->CCR[4]=(1714*80/100); //  @ 80% duty cycle count value
            for(i=100000;i>0;i--){}  //Lousy Delay
        }

        if(pattern_code==3){ // if the sensors 3&4 ~ Front Sensors ~ see white

            // Move Backward //
            P3->OUT=0x80; //  set port 3 bit 6 to one to set high
            P2->OUT=0x80; // set port 2 bit 6 to one to set high
            TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
            TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value
            for(i=100000;i>0;i--){}  //Lousy Delay
        }

        if(pattern_code==12){ // if the sensors 1&2 ~ Back Sensors ~ see white

            // Move Foward //
            P3->OUT=0x40; //  set port 3 bit 6 to one to set high
            P2->OUT=0x40; // set port 2 bit 6 to one to set high
            TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
            TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value
            for(i=100000;i>0;i--){}  //Lousy Delay
        }




    } // End While
} // End Main

/////////////FUNCTIONS//////////////////FUNCTIONS/////////////////FUNCTIONS////////////////////FUNCTIONS////////////////
void LCD_setup(void){

    LCD_Config(); // configure ports
    LCD_clear(); // clear LCD

} // END LCD_setup FUNCTION //

void pin_setup(void){

    //STBY Signal//
    P4->DIR|=0x01; // set port 4 bit 0 to one for output
    P4->OUT|=0x01; // set port 4 bit 0 to one to set high

    //Motor A Directions//
    P3->DIR|=0xC0; // set port 3 bit 6,7 to one for output
    P3->OUT|=0x00; //  set port 3 bit NONE to one to set high
    P3->OUT&=~0xC0; // set port 3 bit 6&7 to zero to set low

    //Motor B Directions//
    P2->DIR|=0xC0; // set port 2 bit 6,7 to one for output
    P2->OUT|=0x00; // set port 2 bit NONE to one to set high
    P2->OUT&=0xC0; // set port 2 bit 6&7 to zero to set low

    /////////PWM WAVES////////
    //P6.6 & P6.7 in PSEL=01 mode & set as output for PWMa & PWMb//
    P6->SEL0|=0xC0;
    P6->SEL1&=~0xC0;
    P6->DIR|=0xC0;

} // END pin_setup FUNCTION //
void setup_TIMERA(void){
//P6.6//
   // xxxx xx  SMclk  Div1 UpMode x NoClr Int TAIFG
   // 0000 00    10    00    01   0   0    0   0
    TIMER_A2->CTL=0x0210;

    TIMER_A2->CCR[0]=(1714); // Top Value = Wanted 1.75KHz square wave
    TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle


    // NoCap xx x x x Comp  x r-s  int x x x x
    //   00  00 0 0 0  0    0 111   0  0 0 0 0
    TIMER_A2->CCTL[3]=0x00E0;

} // END setup_timerA FUNCTION //
void timerA_setup(void){
//P6.7//

    // xxxx xx  SMclk  Div1 UpMode x NoClr Int TAIFG
    // 0000 00    10    00    01   0   0    0   0
     TIMER_A2->CTL=0x0210;

     TIMER_A2->CCR[0]=(1714); // Top Value = Wanted 1.75KHz square wave
     TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value

     // NoCap xx x x x Comp  x r-s  int x x x x
     //   00  00 0 0 0  0    0 111   0  0 0 0 0
     TIMER_A2->CCTL[4]=0x00E0;

} // END timerA_setup FUNCTIONS //

uint8_t Sensor_4(void){
//P4_3 ~ Front Right Sensor//

    P4->DIR|=0x08; // set P4_3 to output ~ pull high
    P4->OUT|=0x08; // value 1
    Delay_48MHz_ms(1); // wait 1 milliseconds ~ gives time for capacitor to discharge
    P4->DIR&=~0x08; // set P4_3 to input ~ pull low

    int count=0;

    while((P4->IN&(0x08))==8){ // if the in register received a 1
         count++; // add 1 to count
         Delay_48MHz_us(10); // wait 10 microseconds
     }

    if(count<=20){
        return 1;
    }
    else{
        return 0;
    }
} // END Sensor_4 FUNCTION
uint8_t Sensor_3(void){
//P4_2 ~ Front Left Sensor//

    P4->DIR|=0x04; // set P4_2 to output ~ pull high
    P4->OUT|=0x04; // value 1
    Delay_48MHz_ms(1); // wait 1 milliseconds ~ gives time for capacitor to discharge
    P4->DIR&=~0x04; // set P4_2 to input ~ pull low

    int count=0;

    while((P4->IN&(0x04))==4){ // if the in register received a 1
         count++; // add 1 to count
         Delay_48MHz_us(10); // wait 10 microseconds
     }

    if(count<=13){
        return 2;
    }
    else{
        return 0;
    }
} // END Sensor_3 FUNCTION
uint8_t Sensor_2(void){
//P4_1 ~ Rear Left Sensor//
    P4->DIR|=0x02; // set P4_1 to output ~ pull high
    P4->OUT|=0x02; // value 1
    Delay_48MHz_ms(1); // wait 1 milliseconds ~ gives time for capacitor to discharge
    P4->DIR&=~0x02; // set P4_1 to input ~ pull low

    int count=0;

    while((P4->IN&(0x02))==2){ // if the in register received a 1
         count++; // add 1 to count
         Delay_48MHz_us(10); // wait 10 microseconds
     }

    if(count<=8){
        return 4;
    }
    else{
        return 0;
    }
} // END Sensor_2 FUNCTION
uint8_t Sensor_1(void){
//P4_5 ~ Rear Right Sensor//

    P4->DIR|=0x20; // set P4_5 to output ~ pull high
    P4->OUT|=0x20; // value 1
    Delay_48MHz_ms(1); // wait 1 milliseconds ~ gives time for capacitor to discharge
    P4->DIR&=~0x20; // set P4_5 to input ~ pull low

    int count=0;

    while((P4->IN&(0x20))==32){ // if the in register received a 1
         count++; // add 1 to count
         Delay_48MHz_us(10); // wait 10 microseconds
     }

    if(count<=8){
        return 8;
    }
    else{
        return 0;
    }
} // END Sensor_1 FUNCTION









