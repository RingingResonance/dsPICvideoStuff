/*
 * File:   videorend.c
 * Author: Jarrett Cigainero
 *
 * Created on June 9, 2017, 2:55 PM
 * 
 * This library is dependent on the "spivideogen.c" library, and
 * the "math.c" library.
 */

#include "videorend.h"
#include "math.c"
#include "math.h"
#include "spivideogen.c"
#include "spivideogen.h"

#ifndef videorend
#define	videorend

/* This takes the info contained in the array "cspace" and calculates the point
 onto a single video line after updating the video buffer.*/
void slineupdate()
{
    if ((buffcopy == 1))
    {
        workskip = scanline - workline;
        workskip = workskip + 2;
        workline = scanline + 2;
        vbuffercopy();
        buffcopy = 0;
        vpoint = 0;
    

    while ((vpoint != 50))
    {
        if ((vspace[vpoint] == 1))
        {
            line();
        }
        else
        {
            vpoint++;
        }
    }
    buffcopy = 0;
    }
}

void line()
{
    vpoint++;
    slope = vspace[vpoint];
    vpoint++;
    yinter = vspace[vpoint];
    vpoint++;
    xone = vspace[vpoint];
    vpoint++;
    yone = vspace[vpoint];
    vpoint++;
    xtwo = vspace[vpoint];
    vpoint++;
    ytwo = vspace[vpoint];
    vpoint++;

    if ((xone > xtwo)) //swap our point order to make things a little faster if needed.
    {
        xtemp = xone;
        xone = xtwo;
        xtwo = xtemp;
        ytemp = yone;
        yone = ytwo;
        ytwo = ytemp;
    }

    xtemp = workline - xone;
    ytemp = workline - xtwo;
            
    if ((xtemp >= 0))
    {
        if ((ytemp <= 0))
        {
            if ((xone == xtwo))
            {
                oldy = ytwo;
                newy = yone;
            }
            else if ((yone == ytwo))
            {
                newy = yone;
                oldy = yone;
            }
            else
            {
                /*if ((workline == xone))
                {
                    oldy = yone;
                    lincalc();
                }
                else if ((workline == xtwo))
                {
                    workline = workline - workskip;
                    lincalc();
                    workline = workline + workskip;
                    oldy = ytwo;
                }
                else
                {*/
                    workline = workline - workskip;
                    lincalc();
                    oldy = newy;
                    workline = workline + workskip;
                    lincalc();
                //}
            }
            //lincalc();
            //drawpoint();
            if ((newy == oldy))
            {
                drawpoint();
            }
            else
            {
                while ((newy != oldy))
                {
                    drawpoint();
                    if ((newy > oldy))
                    {
                        newy--;
                    }
                    if ((newy < oldy))
                    {
                        newy++;;
                    }
                }
            }
        }
    }
}  


void lincalc()
{
    floatline = workline;
    youtput = floatline * slope;
    newy = youtput + yinter;
}


/* This updates the array "cspace" using the info stored in the array "vspace" */

void calcupdate()
{
    vpoint = 0;
    while ((vpoint != 50))
    {
        if ((vspace[vpoint] == 1))       //vector type one, linear.
        {
            vpoint = vpoint + 3;
            xone = vspace[vpoint];
            vpoint++;
            yone = vspace[vpoint];
            vpoint++;
            xtwo = vspace[vpoint];
            vpoint++;
            ytwo = vspace[vpoint];
            vpoint++;

            pointslope();
            vpoint = vpoint - 6;
            vspace[vpoint] = slope;
            vpoint++;
            vspace[vpoint] = yinter;
            vpoint = vpoint + 5;
        }
        else
        {
            vpoint++;           //wind it till you find it.
        }
    }
}

/* This copies the contents of the array "pixbuf" to "pixline" for the video
 output driver to display. */

void vbuffercopy()
{
    pixpoint = 0;
    while ((pixpoint != 7))
    {
        pixline[pixpoint] = pixbuf[pixpoint];
        pixbuf[pixpoint] = 0;
        pixpoint++;
    }
}

/* This takes a single point on the y axes and calculates what information needs
 to go into the array "pixbuf" and performs an inclusive OR with it. */

void drawpoint()
{
    newytmp = newy + 1;
    
    if ((newytmp <= 16))
    {
        pixpoint = 0;
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
    }
    
    else if ((newytmp <= 32))
    {
        pixpoint = 1;
        newytmp = newytmp - 16;
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
    }
    
    else if ((newytmp <= 48))
    {
        pixpoint = 2;
        newytmp = newytmp - 32;
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
    }
    
    else if ((newytmp <= 64))
    {
        pixpoint = 3;
        newytmp = newytmp - 48;
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
    }
    
    else if ((newytmp <= 80))
    {
        pixpoint = 4;
        newytmp = newytmp - 64;
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
    }
    
    else if ((newytmp <= 96))
    {
        pixpoint = 5;
        newytmp = newytmp - 80;
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
    }
    
    else if ((newytmp <= 112))
    {
        pixpoint = 6;
        newytmp = newytmp - 96;
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
    }

    /*
    pixpoint = 0;
    while ((pixpoint != 7))
    {
        if ((newytmp <= 16 ))
        {
        pixbuf[pixpoint] = pixbuf[pixpoint] | bittable[newytmp];
        pixpoint = 7;
        }
        else
        {
            pixpoint++;
            newytmp = newytmp - 16;
        }
    }*/
}



#endif

