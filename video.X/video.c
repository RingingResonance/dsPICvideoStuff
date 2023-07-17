/***********************************************
 * Jarrett Cigainero, May 2017
 * This is example code to help teach newcomers
 * how to use IRQs in the C programming language
 * for dsPIC micro controllers.
***********************************************/
#include <p30f3011.h>


/******************************
* Chip Configuration
******************************/
_FOSC(CSW_FSCM_OFF & XT_PLL8);   /* 10MHz Crystal with PLLx8 = 20 MIP */
_FWDT(WDT_OFF);                 /* Watch dog Timer OFF */
_FBORPOR(PBOR_OFF & MCLR_EN);   /* Brown Out OFF, MCLR Enabled */
_FGS(CODE_PROT_OFF);            /* Code Protect OFF */

/***********************************************************
***********************************************************/

unsigned char hsync;                      //sync variable.
unsigned int vsync;
unsigned char vhsync;
unsigned char vstep;
unsigned char bporch;                     //back porch variable.
unsigned int pixels;                     //pixel timing.
unsigned char fporch;                     //end of line.
unsigned char pxstep;                     //which step of the video are we on.
unsigned char pix;                        //what pixel we are on.
unsigned char pixline[11];               //pixel data.
unsigned int pchunk;
unsigned char p;
unsigned int scanline;
unsigned char oddvstep;
unsigned char charmap;
unsigned int planepos;
unsigned char planedelay;
unsigned int planeposplus;

const unsigned char LetterJ[7] =
{0x7F,0x45,4,8,72,40,48};
const unsigned char LetterA[7] =
{0x3C,0x24,0x24,0x3C,0x42,0x42,0xC3};
const unsigned char LetterR[7] =
{0xCC,0x54,0x62,0x41,0x40,0x40,0x40};
const unsigned char LetterE[7] =
{0xFE,0x40,0x40,0x78,0x40,0x40,0xFE};
const unsigned char LetterT[7] =
{0x7F,0x49,0x8,0x8,0x8,0x8,0x1C};

const unsigned char AIRPLANE[17] =
{0x18,0x18,0x18,0x18,0x3C,0x7E,0xFF,0xDB,0x18,0x18,0x18,0x18,0x3C,0x3C,0x18,0x18,0x18};

int main(void)
{

/******************************
 * initialization values setup.
******************************/

    TRISB = 0;              //set portb to all outputs
    LATB = 0;               //clear portb
    TRISE = 0;              //set porte to all outputs
    LATE = 0;               //set all porte outputs to 0
    PTCON = 0x0002;         //disable the PWM.
    PR1 = 30;           //where timer 1 stops counting.
    TMR1 = 0x0000;          //where timer 1 starts counting from.
    T1CON = 0x8000;         //Timer 1 is already turned on here anyways.
    IPC0 = 0x1000;
//	T1CONbits.TON = 1;		//turn on timer 1
    hsync = 94;             //94 * 20 mips is 4.7uS.
    vhsync = 47;            //47
    vsync = 520;             //520
    bporch = 170;            //94 * 20 mips is 4.7uS.
    pixels = 1040;            //40 * 20 mips is 52 uS for 26 pixels. Close to 52.6 mS
    fporch = 30;            //30 * 20 mips is 1.5 uS.
    pxstep = 1;               /* 0 is fporch
                             * 1 is hsync
                             * 2 is bporch
                             * 3 is our pixels
                             */
    pix = 8;                // 0 through 26 pixels.
    pchunk = 13;
    p = 0;
    scanline = 1;
    
    
    // Clear all interrupts flags
	IFS0bits.T1IF = 0;	// Clear timer 1 flag
	IFS0bits.CNIF = 0;	// Clear interrupt flag
	IFS1bits.IC7IF = 0;	// Clear interrupt flag
	IFS1bits.IC8IF = 0;	// Clear interrupt flag
	IFS2bits.PWMIF = 0;	// Clear interrupt flag

	// enable interrupts
	__asm__ volatile ("DISI #0x3FFF");
	IEC0bits.T1IE = 1;	// Enable interrupts for timer 1
//	IEC0bits.CNIE = 1;	// Enable interrupts on CN5
//	IEC1bits.IC7IE = 1;	// Enable interrupts on IC7
//	IEC1bits.IC8IE = 1;	// Enable interrupts on IC8
//	IEC2bits.PWMIE = 1;	// Enable PWM interrupts
DISICNT = 0;

/*****************************/
/*****************************/
    
    for (;;)                    //loop forever.
    {    
            pixline[0] = 0xFF;

            pixline[9] = 0xFF;
            pixline[10] = 0xFF;
            
            if ((scanline == 1))
            {
                if ((planedelay == 5))
                {
                    planedelay = 0;
                    if ((planepos < 2))
                    {
                        planepos = 250;
                    }
                    else
                    {
                        planepos--;
                    }
                }
                else
                {
                    planedelay++;
                }
            }
            
            planeposplus = planepos + 17;
            if ((scanline >= planepos && scanline <= planeposplus))
            {
                charmap = scanline - planepos;
                pixline[1] = AIRPLANE[charmap];
            }
            else
                
            {
                pixline[1] = 0;
            }
            
            if ((scanline >= 100 && scanline <= 106))
            {
                charmap = scanline - 100;
                pixline[2] = LetterJ[charmap];
            }
            else
            {
                pixline[2] = 0;
            }
            
            if ((scanline >= 100 && scanline <= 106))
            {
                charmap = scanline - 100;
                pixline[3] = LetterA[charmap];
            }
            else
            {
                pixline[3] = 0;
            }
            
            if ((scanline >= 100 && scanline <= 106))
            {
                charmap = scanline - 100;
                pixline[4] = LetterR[charmap];
            }
            else
            {
                pixline[4] = 0;
            }
            
            if ((scanline >= 100 && scanline <= 106))
            {
                charmap = scanline - 100;
                pixline[5] = LetterR[charmap];
            }
            else
            {
                pixline[5] = 0;
            }
            
            
            if ((scanline >= 100 && scanline <= 106))
            {
                charmap = scanline - 100;
                pixline[6] = LetterE[charmap];
            }
            else
            {
                pixline[6] = 0;
            }
            
            if ((scanline >= 100 && scanline <= 106))
            {
                charmap = scanline - 100;
                pixline[7] = LetterT[charmap];
            }
            else
            {
                pixline[7] = 0;
            }
            
            if ((scanline >= 100 && scanline <= 106))
            {
                charmap = scanline - 100;
                pixline[8] = LetterT[charmap];
            }
            else
            {
                pixline[8] = 0;
            }
            
    }
    return 0;
}


//IRQ service routine for timer 1.
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt (void)
    {
    __asm volatile ("BCLR LATB,#7");

    //Vertical Sync
    //EVEN
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
            LATEbits.LATE0 = 1;
            vstep = 0;
            scanline++;
        }
/*        else if ((vstep == 2))
        {
            PR1 = vhsync;
            LATEbits.LATE0 = 0;
            vstep = 0;
            scanline++;
        }
/*        else if ((vstep == 3))
        {
            PR1 = vsync;
            LATEbits.LATE0 = 1;
            vstep = 0;
            scanline++;
        }*/
    }
/*    
    //ODD
    else if ((scanline >= 263 && scanline <= 265))
    {
//        LATEbits.LATE1 = 1;
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
            vstep = 0;
            scanline++;
        }
    }

    else if ((scanline >= 266 && scanline <= 268))
    {
//        LATEbits.LATE1 = 0;
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
            vstep = 0;
            scanline++;
        }
    }
/*
    else if ((scanline >= 269 && scanline <= 271))
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
            LATEbits.LATE0 = 1;
            vstep = 0;
            scanline++;
        }
    }
*/
    //Horizontal line data.
    else
    {
        if((pxstep == 1))
        {
            PR1 = vhsync;
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
//          T1CONbits.TON = 0;		//turn off timer 1
            PR1 = pixels;
            LATE = 0x1;
            while((pchunk <= 9))
            {
                if ((scanline <= 19))
                {
                    LATB = 0;
                }
                else
                {
                    LATB = pixline[pchunk]; //can be a 1, 2, or 3 for black grey and white.
                    __asm volatile ("NOP");
                    __asm volatile ("NOP");
                }
//                __asm volatile ("NOP");
                while((pix < 8))
                {
                    __asm volatile ("RLNC LATB");
                    pix++;
                
                }
                LATB = 0;
                pix = 0;
                pchunk++;
            }
        
            pchunk = 0;
            pxstep = 1;
            p = 0;
            if (scanline == 220)            //525 262.5
            {
                scanline = 1;
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