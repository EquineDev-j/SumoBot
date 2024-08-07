/*
 * Week10.c
 *
 *  Created on: Feb 11, 2019
 *      Author: janne
 */
#include "msp.h"
#include "msoe_lib_all.h"
#include <stdio.h>

///Function Prototypes///Function Prototypes///Function Prototypes///Function Prototypes///

void pin_setup(void);

void setup_TIMERA(void);
void timerA_setup(void);
void timerA_Setup(void);
void NVIC_setup(void);

void LCD_setup (void);

uint8_t Sensor_4(void);
uint8_t Sensor_3(void);


int ISR=0;
int i=0;
int start=0;


void main(void){

    WDT_A->CTL = 0x5A00 | 0x80; // start watchdog
    Clock_Init_48MHz();

    LCD_Config(); // configure ports
    LCD_clear(); // clear LCD

    pin_setup();
    setup_TIMERA();
    timerA_setup();
    timerA_Setup();
    NVIC_setup();


    while(1){

            if(start==0){
                //Stop for 5 seconds//
            TIMER_A2->CCR[3]=(1714*0/100); // compare value start at 50% duty cycle
            TIMER_A2->CCR[4]=(1714*0/100); //  @ 20% duty cycle count value
            Delay_48MHz_sec(5);
            start=1;}

        uint8_t pattern_code=0;

        pattern_code+=Sensor_4();
        pattern_code+=Sensor_3();

        LCD_goto_xy(0,0);
        LCD_print_bin8(pattern_code);


        if((ISR==1)&&(pattern_code==0)){ // if detects object and not on white line
            //Move Forward//
           P3->OUT=0x40; //  set port 3 bit 7 to one to set high ~ move forward
           P2->OUT=0x40; // set port 2 bit 6 to one to set high ~ move forward
           TIMER_A2->CCR[3]=(6857*70/100); // compare value start at 50% duty cycle
           TIMER_A2->CCR[4]=(6857*70/100); //  @ 50% duty cycle count value
        }

        if((ISR==0)&&(pattern_code==0)){ // if doesn't detect object and not on white line

           //Rotate Forward//
          P3->OUT=0x40; //  set port 3 bit 7 to one to set high ~ move forward
          P2->OUT=0x40; // set port 2 bit 6 to one to set high ~ move forward
          TIMER_A2->CCR[3]=(6857*60/100); // compare value start at 50% duty cycle
          TIMER_A2->CCR[4]=(6857*20/100); //  @ 50% duty cycle count value
          for(i=50000;i>0;i--){}  //Lousy Delay // 1 sec
        }

        if(((ISR==1)||(ISR==0))&&(pattern_code!=0)){ // if on white line

           if(pattern_code==1){ // if the sensors 4 ~ Front Right Sensors ~ see white

               // Move Forward Left ~ Move backward Right //
               P3->OUT=0x40; //  set port 3 bit 6 to one to set high ~ move forward
               P2->OUT=0x80; // set port 2 bit 7 to one to set high ~ move backward
               TIMER_A2->CCR[3]=(6857*20/100); // compare value start at 80% duty cycle
               TIMER_A2->CCR[4]=(6857*20/100); //  @ 80% duty cycle count value
               for(i=100000;i>0;i--){}  //Lousy Delay
           }

           if(pattern_code==2){ // if the sensors 3 ~ Front Left Sensors ~ see white

               // More Forward Right ~ Move backward Left //
               P3->OUT=0x80; //  set port 3 bit 7 to one to set high ~ move backward
               P2->OUT=0x40; // set port 2 bit 6 to one to set high ~ move forward
               TIMER_A2->CCR[3]=(6857*20/100); // compare value start at 80% duty cycle
               TIMER_A2->CCR[4]=(6857*20/100); //  @ 80% duty cycle count value
               for(i=100000;i>0;i--){}  //Lousy Delay
           }

           if(pattern_code==3){ // if the sensors 3&4 ~ Front Sensors ~ see white

               // Move Backward //
               P3->OUT=0x80; //  set port 3 bit 6 to one to set high
               P2->OUT=0x80; // set port 2 bit 6 to one to set high
               TIMER_A2->CCR[3]=(6857/2); // compare value start at 50% duty cycle
               TIMER_A2->CCR[4]=(6857*20/100); //  @ 50% duty cycle count value
               for(i=100000;i>0;i--){}  //Lousy Delay
           }

        }


    } // End While
} // End Main

/////////////FUNCTIONS//////////////////FUNCTIONS/////////////////FUNCTIONS////////////////////FUNCTIONS////////////////
void LCD_setup(void){

    LCD_Config(); // configure ports
    LCD_clear(); // clear LCD

} // END LCD_setup FUNCTION //

void pin_setup(void){

    // P10.2 set as for LED Output //
    P10->DIR|=0x04; // sets port 9 bit 0 equal to one for output
    P10->OUT&=~0x04; // turn LED Off initially

    // P8.6 set as input to read IR Sensor //
    P8->DIR&=~0x40; // set to input

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

    // P8.2 in PSEL=01 mode for IR Transmitter& set as output Using TIMERA ~ 38KHz Wave//
    P8->SEL0|=0x04;
    P8->SEL1&=~0x04;
    P8->DIR|=0x04;

} // END pin_setup FUNCTION //
void setup_TIMERA(void){
//P6.6 ~ Motor A//
   // xxxx xx  SMclk  Div1 UpMode x NoClr Int TAIFG
   // 0000 00    10    00    01   0   0    0   0
    TIMER_A2->CTL=0x0210;

    TIMER_A2->CCR[0]=(6857); // Top Value = Wanted 1.75KHz square wave
    TIMER_A2->CCR[3]=(6857/2); // compare value start at 50% duty cycle


    // NoCap xx x x x Comp  x r-s  int x x x x
    //   00  00 0 0 0  0    0 111   0  0 0 0 0
    TIMER_A2->CCTL[3]=0x00E0;

} // END setup_timerA FUNCTION //
void timerA_setup(void){
//P6.7 ~ Motor B//

    // xxxx xx  SMclk  Div1 UpMode x NoClr Int TAIFG
    // 0000 00    10    00    01   0   0    0   0
     TIMER_A2->CTL=0x0210;

     TIMER_A2->CCR[0]=(6857); // Top Value = Wanted 1.75KHz square wave
     TIMER_A2->CCR[4]=(6857/2); //  @ 50% duty cycle count value

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
         Delay_48MHz_us(1000); // wait 10 microseconds
     }

    if(count<=5){ // 25
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
         Delay_48MHz_us(1000); // wait 10 microseconds
     }

    if(count<=5){
        return 2;
    }
    else{
        return 0;
    }
} // END Sensor_3 FUNCTION

void timerA_Setup(void){
//P8.2 ~ IR Transmitter//

     // xxxx xx  SMclk  Div1 UpMode x NoClr Int TAIFG
     // 0000 00    10    00    01   0   0    1   0
       TIMER_A3->CTL=0x0212;

       TIMER_A3->CCR[0]=(316); // Top Value = Wanted 38KHz square wave
       TIMER_A3->CCR[2]=(316/2); // 38KHz @ 50% duty cycle count value

     // NoCap xx x x x Comp  x r-s  int x x x x
     //   00  00 0 0 0  0    0 111   1  0 0 0 0
       TIMER_A3->CCTL[2]=0x00F0;

} // END timerA_setup FUNCTION //

void NVIC_setup(void){
//p8.2//
      NVIC->IP[15]|=0x04; // set priority
      NVIC->ISER[0]|=0x00008000; // ISER1 starts at 32
} // END NVIC_setup FUNCTION //

void TA3_N_IRQHandler(void){
// 8.2 Handler ~ IR Transmitter //
    static count; //1 count = 81.81us
    count++; // each count takes 81.81us

    if(count==490){ // PWM off for 500us or 5ms;

        // NoCap xx x x x Comp  x OUT  int x x x x
        //   00  00 0 0 0  0    0 000   1  0 0 0 0
        TIMER_A3->CCTL[2]=0x0010;

        if(((P8->IN)&(0x40))==0){
             P10->OUT|=0x04; // turn on LED
             ISR=1;
         }

         else{
             P10->OUT&=~0x04; // turn off LED
             ISR=0;
         }

    }

    if(count==900){ // PWM on for 600us or 6ms

        // NoCap xx x x x Comp  x r-s  int x x x x
        //   00  00 0 0 0  0    0 111   1  0 0 0 0
        TIMER_A3->CCTL[2]=0x00F0;
        count=0;
    }

    int clear_flag=(TIMER_A3->IV);  // just reading reg clears flag
} // END TA2_N_IRQHandler FUNCTION //




