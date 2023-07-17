/* 
 * File:   spivideogen.h
 * Author: Jarrett Cigainero
 *
 * Created on June 9, 2017, 10:34 PM
 */

#ifndef SPIVIDEOGEN_H
#define	SPIVIDEOGEN_H

void vidinit();
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt (void);
void __attribute__((interrupt, no_auto_psv)) _SPI1Interrupt (void);

/* VIDEO output CONSTANTS */
const unsigned int hsync = 188;                      //horizontal sync 94 * 20 mips is 4.7uS. for 20 mips
const unsigned int vsync = 2040;                      //vertical sync 520 for 20 mips
const unsigned int vhsync = 94;                     //some other sync stuff?? 47 for 20 mips
const unsigned int bporch = 320;                     //back porch. 94 * 20 mips is 4.7uS. for 20 mips
const unsigned int pixels = 2020;                     //pixel timing. magic number Close to 52.?? mS for 20 mips
/* VIDEO output VARs*/
unsigned int vstep;                      //vertical step stuff.
unsigned int pxstep;                     //which step of the video are we on.
unsigned int pixline[14];                //pixel chunk data.
unsigned int pchunk;                     //pixel chunk pointer. Used by video driver.
unsigned int scanline;                   //scan line number.

#endif	/* SPIVIDEOGEN_H */

