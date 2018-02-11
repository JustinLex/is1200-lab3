
/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x0000;

uint8_t timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void ) {
  TRISECLR = 0xff; //enable led output
  TRISDSET = 0xfd0; //enable button and switch input

  /*initialize timer2*/
  T2CONCLR = 0xFFFF; //disable timer 2 and clear registers if enabled
  T2CONSET = 0x70; //set timer prescale to 256:1 (we need to count to 8M cycles, which is not possible with 1:64 or lower)
  TMR2 = 0x0; //clear timer 2 count
  PR2 = 0x7a12; //set period to 31250 = 100ms@312.5khz (80Mhz@1:256)
  IFSCLR(0) = 0x100; //reset timer 2 Interrupt flag
  T2CONSET = 0x8000; //enable timer
  return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {


  /*check buttons and update time if it is manually set*/
  if((getbtns() & 0x4))
  mytime=(mytime & 0x0fff)|(getsw()<<12); //change the first digit to the switches input (in binary) X6:42
  if((getbtns() & 0x2))
  mytime=(mytime & 0xf0ff)|(getsw()<<8); //change the second digit to the switches input (in binary) 1X:42
  if((getbtns() & 0x1))
  mytime=(mytime & 0xff0f)|(getsw()<<4); // change the third digit to the switches input (in binary) 16:X2

  /*update display at 10hz*/
  //delay(100);
  if( IFS(0) & 0x100 ) {
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    display_image(96, icon);
    timeoutcount++; //increment timeoutcount so we can count to a full second
    IFSCLR(0) = 0x100; //reset timeout flag
  }



  /*tick every second*/
  if(timeoutcount >= 10) {
    tick( &mytime );
    timeoutcount = 0;
  }

  PORTE = TMR2 >> 8; //set the leds to show the current time in binary
}
