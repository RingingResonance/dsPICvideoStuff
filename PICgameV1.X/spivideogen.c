/*
 * File:   spivideogen.c
 * Author: Jarrett Cigainero
 *
 * Created on June 9, 2017, 2:48 PM
 */
#include <p30f3011.h>
#include "spivideogen.h"

/* To display information you must load the "pixline" array with a single line
 * of video data for each line as pointed by the "scanline" variable.
 * The "pchunk" pointer is used strictly by the video output driver and must 
 * not be disturbed. "pixline" is the only variable here that should ever be
 * written to by user code. However, any one of these may be read from at any 
 * time. */

#ifndef spivideo
#define	spivideo


/* This initializes the appropriate IRQs, Timers, and SPI interface for
 video output. */

void vidinit ()
{
    PR1 = 30;           //where timer 1 stops counting.
    TMR1 = 0x0000;          //where timer 1 starts counting from.
    T1CON = 0x8000;         //Timer 1 is already turned on here anyways.
    IPC0 = 0xFFFF;
    SPI1CON = 0x043D;       //0x043A for 1:8 0x043A for 1:16
    SPI1STAT = 0x0000;
    SPI1STATbits.SPIEN = 1;
    pxstep = 1;             /* 1 is hsync
                             * 2 is bporch
                             * 3 is our pixels */
    pchunk = 0;             //POINTER used for our pixel chunks
    scanline = 1;           //what scan line are we on?
    
    /* IRQ initialization */
    // Clear all interrupts flags
	IFS0bits.T1IF = 0;	// Clear timer 1 flag
	IFS0bits.CNIF = 0;	// Clear interrupt flag
	IFS1bits.IC7IF = 0;	// Clear interrupt flag
	IFS1bits.IC8IF = 0;	// Clear interrupt flag
	IFS2bits.PWMIF = 0;	// Clear interrupt flag
    IFS0bits.SPI1IF = 0;	// Clear interrupt flag

	// enable interrupts
	__asm__ volatile ("DISI #0x3FFF");
	IEC0bits.T1IE = 1;	// Enable interrupts for timer 1
    IEC0bits.SPI1IE = 1;	// Enable interrupts for SPI
//	IEC0bits.CNIE = 1;	// Enable interrupts on CN5
//	IEC1bits.IC7IE = 1;	// Enable interrupts on IC7
//	IEC1bits.IC8IE = 1;	// Enable interrupts on IC8
//	IEC2bits.PWMIE = 1;	// Enable PWM interrupts
DISICNT = 0;
}

/*IRQs GO DOWN HERE************************************************************
 * This is what runs the display. 
 * These IRQs must always have the highest priority. *************************/


//IRQ service routine for timer 1. Horizontal vertical sync and video data init.

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt (void)
    {
    __asm volatile ("BCLR LATB,#7");

    //Vertical Sync
    //EVEN, we aren't doing odd lines yet.
    if ((scanline <= 3))
    {
        LATEbits.LATE1 = 1;
        if ((vstep == 0))
        {
            PR1 = vhsync;
            LATEbits.LATE0 = 0;
            vstep = 1;
        }
        else if ((vstep == 1))
        {
            PR1 = vsync;
            LATEbits.LATE0 = 1;
            vstep = 2;
        }
        else if ((vstep == 2))
        {
            PR1 = vhsync;
            LATEbits.LATE0 = 0;
            vstep = 3;
        }
        else if ((vstep == 3))
        {
            PR1 = vsync;
            LATEbits.LATE0 = 1;
            vstep = 0;
            scanline++;
        }
    }
    
    else if ((scanline <= 6))
    {
        LATEbits.LATE1 = 0;
        if ((vstep == 0))
        {
            PR1 = vsync;
            LATEbits.LATE0 = 0;
            vstep = 1;
        }
        else if ((vstep == 1))
        {
            PR1 = hsync;
            LATEbits.LATE0 = 1;
            vstep = 2;
        }
        else if ((vstep == 2))
        {
            PR1 = vsync;
            LATEbits.LATE0 = 0;
            vstep = 3;
        }
        else if ((vstep == 3))
        {
            PR1 = hsync;
            LATEbits.LATE0 = 1;
            vstep = 0;
            scanline++;
        }
    }
    
    else if ((scanline <= 9))
    {
        if ((vstep == 0))
        {
            PR1 = vhsync;
            LATEbits.LATE0 = 0;
            vstep = 1;
        }
        else if ((vstep == 1))
        {
            PR1 = vsync;
            LATEbits.LATE1 = 1;
            vstep = 0;
            scanline++;
        }
    }

    //Horizontal line data.
    else
    {
        if((pxstep == 1))
        {
            PR1 = hsync;
            LATE = 0x0;
            pxstep = 2;
        }
        else if((pxstep == 2))
        {
            PR1 = bporch;
            LATE = 0x1;
            pxstep = 3;
        }
        
        else
        {
            PR1 = pixels;
            LATE = 0x1;
            if ((scanline >= 20))
            {
            /* Start the SPI and load it with the first byte of data.*/
                pchunk = 0;
                SPI1STATbits.SPIROV = 0;
                SPI1BUF = pixline[pchunk];
                __asm volatile ("NOP");
                __asm volatile ("NOP");
                pchunk++;
                SPI1STATbits.SPIROV = 0;
                SPI1BUF = pixline[pchunk];
                pchunk++;
            }

            pxstep = 1;
            if (scanline == 220)
            {
                scanline = 1;
                simup = 1;
            }
            
            else
            {
                scanline++;
            }
            
        }
    }
    T1CONbits.TON = 1;		//turn on timer 1
	IFS0bits.T1IF = 0;      //Clear this IRQ flag after the routine has finished.
}



//IRQ service routine for SPI 1. This is what spits out the rest of the video data.
void __attribute__((interrupt, no_auto_psv)) _SPI1Interrupt (void)
{
    if ((pchunk <= 6))
    {
        SPI1STATbits.SPIROV = 0;
        SPI1BUF = pixline[pchunk];
        pixline[pchunk] = 0;
        pchunk++;
    }
    
    else
    {
        pchunk = 0;
        buffcopy = 1;
    }
    
    IFS0bits.SPI1IF = 0;	// Clear interrupt flag
}

#endif