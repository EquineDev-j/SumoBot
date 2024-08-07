/*
 * Week_4.c
 *
 *  Created on: Dec 14, 2018
 *      Author: jannel bennett
 *      EE2931 - Embedded Systems
 */
#include "msp.h"
#include "msoe_lib_all.h"
#include <stdio.h>

///Function Prototypes///

void pin_setup(void);
void setup_TIMERA(void);
void timerA_setup(void);
void LCD_setup (void);

//Delays not working//
//int Delay_48MHz_sec(uint8_t val);
//int Delay_3MHz_us(uint32_t val);



void main(void){


    char EXAM_1[6] = "EXAM 1";
    char EE2931[6]="EE2931";
    char section[5]="- 021" ;
    char STEP[4]="STEP";

    LCD_Config(); // configure ports
    LCD_clear(); // clear LCD

    pin_setup();
    setup_TIMERA();
    timerA_setup();


    while(1){


        // Print on LCD //
        LCD_goto_xy(0,0); // go to 0,0 on LCD , first line of LCD
        LCD_print_str(EXAM_1);

        LCD_goto_xy(3,1); // go to 3,1 on LCD , first line of LCD
        LCD_print_str(EE2931);

        LCD_goto_xy(5,2); // go to 0,0 on LCD , first line of LCD
        LCD_print_str(section);

        LCD_goto_xy(8,5); // go to 0,0 on LCD , first line of LCD
        LCD_print_str(STEP);

        // Bot Movement //

        int i;

        //STEP 1//

        LCD_goto_xy(0,5); // go to 0,0 on LCD , first line of LCD
        LCD_print_dec3(1);
                // Move Left one full rotation //
        P3->OUT=0x40; //  set port 3 bit 7 to one to set high
        P2->OUT=0x40; // set port 2 bit 6 to one to set high
        TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714*10/100); //  @ 20% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay
        for(i=1000000;i>0;i--){}  //Lousy Delay
        for(i=10000;i>0;i--){}  //Lousy Delay
                // STOP //
        TIMER_A2->CCR[3]=(0); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(0); //  @ 20% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay



        //STEP 2 //

        LCD_goto_xy(0,5); // go to 0,0 on LCD , first line of LCD
        LCD_print_dec3(2);
                // Move Foward  @ half speed for 1 second//
        P3->OUT=0x40; //  set port 3 bit 6 to one to set high
        P2->OUT=0x40; // set port 2 bit 6 to one to set high
        TIMER_A2->CCR[3]=(1714*30/100); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714*30/100); //  @ 50% duty cycle count value
        for(i=100000;i>0;i--){}  //Lousy Delay
        for(i=1000;i>0;i--){}  //Lousy Delay

        //STEP 3//

        LCD_goto_xy(0,5); // go to 0,0 on LCD , first line of LCD
        LCD_print_dec3(3);
                    // STOP //
        TIMER_A2->CCR[3]=(0); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(0); //  @ 20% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay
                // Turn Left 45 degrees //
        P3->OUT=0x40; //  set port 3 bit 6 to one to set high
        P2->OUT=0x40; // set port 2 bit 6 to one to set high
        TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714*0/100); //  @ 00% duty cycle count value
        for(i=250000;i>0;i--){}  //Lousy Delay

        //STEP 4//

        LCD_goto_xy(0,5); // go to 0,0 on LCD , first line of LCD
        LCD_print_dec3(4);
                            //Stop//
        TIMER_A2->CCR[3]=(1714*0/100); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714*0/100); //  @ 20% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay
                // Move Backward for 4 seconds @ full speed //
        P3->OUT=0x80; //  set port 3 bit 6 to one to set high
        P2->OUT=0x80; // set port 2 bit 6 to one to set high
        TIMER_A2->CCR[3]=(1714*60/100); // compare value start at 60% duty cycle
        TIMER_A2->CCR[4]=(1714*60/100); //  @ 60% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay

        //STEP 5//

        LCD_goto_xy(0,5); // go to 0,0 on LCD , first line of LCD
        LCD_print_dec3(5);
                // STOP //
        TIMER_A2->CCR[3]=(0); // compare value start at 0% duty cycle
        TIMER_A2->CCR[4]=(0); //  @ 0% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay
                // Move Right 45 degrees //
        P3->OUT=0x40; //  set port 3 bit 6 to one to set high
        P2->OUT=0x40; // set port 2 bit 6 to one to set high

        TIMER_A2->CCR[3]=(1714*10/100); // compare value start at 10% duty cycle
        TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value
        for(i=250000;i>0;i--){}  //Lousy Delay

        //STEP 6//

        LCD_goto_xy(0,5); // go to 0,0 on LCD , first line of LCD
        LCD_print_dec3(6);
                // STOP //
        TIMER_A2->CCR[3]=(0); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(0); //  @ 20% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay
                    // Move Foward for 1 second @ half speed//
          P3->OUT=0x40; //  set port 3 bit 6 to one to set high
          P2->OUT=0x40; // set port 2 bit 6 to one to set high

          TIMER_A2->CCR[3]=(1714*30/100); // compare value start at 30% duty cycle
          TIMER_A2->CCR[4]=(1714*30/100); //  @ 30% duty cycle count value
          for(i=100000;i>0;i--){}  //Lousy Delay
          for(i=1000;i>0;i--){}  //Lousy Delay

          //STEP 7//

          LCD_goto_xy(0,5); // go to 0,0 on LCD , first line of LCD
          LCD_print_dec3(7);
                  // STOP //
          TIMER_A2->CCR[3]=(0); // compare value start at 50% duty cycle
          TIMER_A2->CCR[4]=(0); //  @ 20% duty cycle count value
          for(i=1000000;i>0;i--){}  //Lousy Delay
                  // Move Right one rotation //
          P3->OUT=0x40; //  set port 3 bit 6 to one to set high
          P2->OUT=0x40; // set port 2 bit 6 to one to set high

          TIMER_A2->CCR[3]=(1714*10/100); // compare value start at 10% duty cycle
          TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value
          for(i=1000000;i>0;i--){}  //Lousy Delay
          for(i=1000000;i>0;i--){}  //Lousy Delay
          for(i=10000;i>0;i--){}  //Lousy Delay

//stop//
TIMER_A2->CCR[3]=(1714*0/100); // compare value start at 00% duty cycle
TIMER_A2->CCR[4]=(1714*0/100); //  @ 00% duty cycle count value
for(i=1000000;i>0;i--){}  //Lousy Delay

        return 0;


    }}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
     /*   // Move Foward //
        P3->OUT=0x40; //  set port 3 bit 6 to one to set high
        P2->OUT=0x40; // set port 2 bit 6 to one to set high

        TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay


        // Move Backward //
        P3->OUT=0x80; //  set port 3 bit 6 to one to set high
        P2->OUT=0x80; // set port 2 bit 6 to one to set high

        TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay


        // Move Left //
        P3->OUT=0x40; //  set port 3 bit 7 to one to set high
        P2->OUT=0x40; // set port 2 bit 6 to one to set high

        TIMER_A2->CCR[3]=(1714/2); // compare value start at 50% duty cycle
        TIMER_A2->CCR[4]=(1714*20/100); //  @ 20% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay


        // Move Right //
        P3->OUT=0x40; //  set port 3 bit 6 to one to set high
        P2->OUT=0x40; // set port 2 bit 6 to one to set high

        TIMER_A2->CCR[3]=(1714*20/100); // compare value start at 20% duty cycle
        TIMER_A2->CCR[4]=(1714/2); //  @ 50% duty cycle count value
        for(i=1000000;i>0;i--){}  //Lousy Delay
    */
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
