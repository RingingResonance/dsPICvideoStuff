/***********************************************
 * Jarrett Cigainero, June 2017
 * This is example code to help teach newcomers
 * how to use IRQs in the C programming language
 * for dsPIC micro controllers. This one uses
 * SPI for video data output in monochrome
 * composite video. 208 * 220 pixels.
***********************************************/
#include <p30f3011.h>
#include "videorend.c"
#include "videorend.h"
#include "boarderbounce.c"
#include "boarderbounce.h"

/******************************
* Chip Configuration
******************************/
_FOSC(CSW_FSCM_OFF & XT_PLL16);   /* 10MHz Crystal with PLLx16 = 40 MIP. We have
                                   * to over clock a little bit. */
_FWDT(WDT_OFF);                 /* Watch dog Timer OFF */
_FBORPOR(PBOR_OFF & MCLR_EN);   /* Brown Out OFF, MCLR Enabled */
_FGS(CODE_PROT_OFF);            /* Code Protect OFF */

/***********************************************************
***********************************************************/

int main(void)
{
/*******************************
 * initialization values setup.
*******************************/

    TRISB = 0;              //set portb to all outputs
    LATB = 0;               //clear portb
    TRISE = 0;              //set porte to all outputs
    LATE = 0;               //set all porte outputs to 0
    PTCON = 0x0002;         //disable the PWM.
    vidinit();              //initialize our video output vars.
    
    vspace[1] = 1;
    vspace[8] = 1;
    vspace[15] = 1;
    xpoint1 = 30;
    ypoint1 = 30;
    xpoint2 = 150;
    ypoint2 = 90;
    xpoint3 = 145;
    ypoint3 = 50;

    vspace[4] = xpoint1;
    vspace[5] = ypoint1;
    vspace[6] = xpoint2;
    vspace[7] = ypoint2;
            
    vspace[11] = xpoint2;
    vspace[12] = ypoint2;
    vspace[13] = xpoint3;
    vspace[14] = ypoint3;
            
    vspace[18] = xpoint3;
    vspace[19] = ypoint3;
    vspace[20] = xpoint1;
    vspace[21] = ypoint1;

    calcupdate();

/*****************************/
/*****************************/

    
    for (;;)                    //loop forever. TEST CODE
    {
        if ((simup == 1))       //user code goes here for now.
        {
            simup = 0;
            simbounce();
            simmove();
            simcopy();
            calcupdate();       //bulk calculations, its not much right now.
        }

        if ((scanline >= 18 && scanline <= 220))  //Recalculate each scan line.
        {
            slineupdate();
        }
        Idle();
    }

    return 0;
}

