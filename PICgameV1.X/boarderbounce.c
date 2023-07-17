/* 
 * File:   boarderbounce.c
 * Author: Jarrett Cigainero
 *
 * Created on June 9, 2017, 10:34 PM
 */

#include "boarderbounce.h"
#include "videorend.h"

#ifndef BOARDER_C
#define	BOARDER_C


void simmove()
{
    if ((linemove == 4))
    {
        linemove = 0;
        if ((xdir == 0))
        {
            xpoint1--;
        }
        else
        {
            xpoint1++;
        }
        if ((ydir == 0))
        {
            ypoint1--;
        }
        else
        {
            ypoint1++;
        }
    }
    else
    {
        linemove++;
    }
    if ((linemove2 == 2))
    {
        linemove2 = 0;
        if ((xdir2 == 0))
        {
            xpoint2--;
        }
        else
        {
            xpoint2++;
        }
        if ((ydir2 == 0))
        {
            ypoint2--;
        }
        else
        {
            ypoint2++;
        }
    }
    else
    {
        linemove2++;
    }
    if ((linemove3 == 1))
    {
        linemove3 = 0;
        if ((xdir3 == 0))
        {
            xpoint3--;
        }
        else
        {
            xpoint3++;
        }
        if ((ydir3 == 0))
        {
            ypoint3--;
        }
        else
        {
            ypoint3++;
        }
    }
    else
    {
        linemove3++;
    }
}

void simbounce()
{
        if ((xpoint1 == 200))
        {
            xdir = 0;
        }
        if ((xpoint1 == 20))
        {
            xdir = 1;
        }
        if ((ypoint1 == 100))
        {
            ydir = 0;
        }
        if ((ypoint1 == 1))
        {
            ydir = 1;
        }
        if ((xpoint2 == 200))
        {
            xdir2 = 0;
        }
        if ((xpoint2 == 20))
        {
            xdir2 = 1;
        }
        if ((ypoint2 == 100))
        {
            ydir2 = 0;
        }
        if ((ypoint2 == 1))
        {
            ydir2 = 1;
        }
        if ((xpoint3 == 200))
        {
            xdir3 = 0;
        }
        if ((xpoint3 == 20))
        {
            xdir3 = 1;
        }
        if ((ypoint3 == 100))
        {
            ydir3 = 0;
        }
        if ((ypoint3 == 1))
        {
            ydir3 = 1;
        }
}

void simcopy()
{
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
}

#endif