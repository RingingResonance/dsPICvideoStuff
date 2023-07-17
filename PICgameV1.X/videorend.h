/*
 * File:   videorend.h
 * Author: Jarrett Cigainero
 *
 * Created on June 9, 2017, 2:55 PM
 */

#ifndef video_system
#define	video_system


void calcupdate();
void vbuffercopy();
void drawpoint();
void slineupdate();
void lincalc();
void line();

/*vector calc vars */
float vspace[50];               //vector storage data
float floatline;
float youtput;
unsigned int pixline[14];                //pixel chunk data.
unsigned int vpoint;                    //pointer for vspace
unsigned int workline;
unsigned int pixbuf[14];
unsigned int pixpoint;
unsigned int pixnumb;
unsigned int drawmode;
unsigned int setnum;
unsigned int workskip;
unsigned char buffcopy;
unsigned char calcup;
unsigned char pointstart;
unsigned char simup;
signed int xinput;
signed int newy;
signed int oldy;
signed int newytmp;
signed int oldytmp;

const unsigned int bittable[] =
{0,32768,16384,8192,4096,2048,
1024,512,256,128,64,32,16,8,4,2,1};

#endif

