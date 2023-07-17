/*
 * File:   math.c
 * Author: Jarrett Cigainero
 *
 * Created on June 9, 2017, 2:55 PM
 */

#include "math.h"

#ifndef somemath
#define	somemath

/*This takes two points and calculates the Slope and Y intercept. */

void pointslope()
{
    float sxtemp;
    float sytemp;
    float ytemp;
    sxtemp = xtwo - xone;
    sytemp = ytwo - yone;
    slope = sytemp / sxtemp;
    sxtemp = xone * -1;
    ytemp = sxtemp * slope;
    yinter = ytemp + yone;
}

#endif
