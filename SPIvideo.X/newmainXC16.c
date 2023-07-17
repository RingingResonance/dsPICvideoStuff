/***********************************************
 * Jarrett Cigainero, May 2017
 * This is example code to help teach newcomers
 * how to use IRQs in the C programming language
 * for dsPIC micro controllers. This one uses
 * SPI for video data output in monochrome
 * composite video. 416 * 220 pixels.
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

unsigned int hsync;                      //sync variable.
unsigned int vsync;
unsigned int vhsync;
unsigned int vstep;
unsigned int bporch;                     //back porch variable.
unsigned int pixels;                     //pixel timing.
unsigned int fporch;                     //end of line.
unsigned int pxstep;                     //which step of the video are we on.
unsigned int pix;                        //what pixel we are on.
unsigned int pixline[27];                //pixel data.
unsigned int pchunk;
unsigned int p;
unsigned int scanline;
unsigned int oddvstep;
unsigned int charmap;
unsigned int planepos;
unsigned int planedelay;
unsigned int planeposplus;

const unsigned int LetterJ[7] =
{0x7F,0x45,4,8,72,40,48};
const unsigned int LetterA[7] =
{0x3C,0x24,0x24,0x3C,0x42,0x42,0xC3};
const unsigned int LetterR[7] =
{0xCC,0x54,0x62,0x41,0x40,0x40,0x40};
const unsigned int LetterE[7] =
{0xFE,0x40,0x40,0x78,0x40,0x40,0xFE};
const unsigned int LetterT[7] =
{0x7F,0x49,0x8,0x8,0x8,0x8,0x1C};

const unsigned int AIRPLANE[17] =
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
    IPC0 = 0xFFFF;
    SPI1CON = 0x043B;       //0x043E for 1:4
    SPI1STAT = 0x0000;


//	T1CONbits.TON = 1;		//turn on timer 1
    hsync = 94;             //94 * 20 mips is 4.7uS.
    vhsync = 47;            //47
    vsync = 520;             //520
    bporch = 160;            //94 * 20 mips is 4.7uS.
    pixels = 1010;            //magic number Close to 52.?? mS
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

/*****************************/
/*****************************/
    
    for (;;)                    //loop forever. TEST CODE
    {    
            pixline[0] = 0xF0F7;

            pixline[22] = 0x0FFF;
            pixline[23] = 0xF0F0;
            pixline[24] = 0xF0F0;
            pixline[25] = 0xFFFF;
            pixline[26] = 0xFF0F;
//            pixline[14] = 0xFF0F;
            
            
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

/*IRQs GO DOWN HERE************************************************************/

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
            LATEbits.LATE1 = 1;
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
        else if ((vstep == 3))
        {
            PR1 = vsync;
            LATEbits.LATE1 = 1;
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
            PR1 = hsync;
            LATE = 0x0;
            pxstep = 2;
//            SPI1STATbits.SPIEN = 0;
        }
        else if((pxstep == 2))
        {
            PR1 = bporch;
            LATE = 0x1;
            pxstep = 3;
//            SPI1STATbits.SPIEN = 0;
        }
        else
        {
//          T1CONbits.TON = 0;		//turn off timer 1
            PR1 = pixels;
            LATE = 0x1;

            if ((scanline >= 20))
            {
            /* Start the SPI and load it with the first byte of data.*/
            SPI1STATbits.SPIROV = 0;
            pchunk = 0;
            SPI1STATbits.SPIEN = 1;
            SPI1BUF = pixline[pchunk];
            __asm volatile ("NOP");
            __asm volatile ("NOP");
            pchunk++;
            SPI1STATbits.SPIROV = 0;
            SPI1BUF = pixline[pchunk];
            pchunk++;
            }
            p = 0;
            pxstep = 1;

            if (scanline == 220)
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



//IRQ service routine for SPI 1.
void __attribute__((interrupt, no_auto_psv)) _SPI1Interrupt (void)
{
    if ((pchunk <= 26))
    {
        SPI1STATbits.SPIROV = 0;
        SPI1BUF = pixline[pchunk];
        pchunk++;
    }
    else
    {
//        SPI1STATbits.SPIEN = 0;
        pchunk = 0;
    }
    
    IFS0bits.SPI1IF = 0;	// Clear interrupt flag
}