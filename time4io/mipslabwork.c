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

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile uint8_t* reg_trise_clr = 0xbf886104; //output-enable register
  *reg_trise_clr = 0xff;
  volatile uint32_t* reg_trisd_set = 0xbf8860c8; //input-enable register
  *reg_trisd_set = 0xfd0;
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile uint8_t* reg_porte = 0xbf886110; //IO port E output register
  delay( 10 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  if(getbtns)
  {
      if((getbtns&0x4))
      mytime=(mytime&0xf0ff)|(getsw<<8); //change the first digit to the switches input (in binary) X6:42
      if((getbtns&0x2))
      mytime=(mytime&0xf0ff)|(getsw<<4); //change the second digit to the switches input (in binary) 1X:42
      if((getbtns&0x1))
      mytime=(mytime&0xff0f)|(getsw); // change the third digit to the switches input (in binary) 16:X2
  }
  (*reg_porte)++; //set the leds to show the current time in binary
  display_image(96, icon);
}
