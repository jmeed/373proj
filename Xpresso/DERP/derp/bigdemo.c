/****************************************************************************************
*                                                                                       *
*  Goldelox 4D Serial Sample                                                            *
*                                                                                       *
*  Date:        21 December 2012                                                        *
*                                                                                       *
*  Description: Demonstrates Pretty much every Goldelox 4D Serial command.              *
*               This has been written as a console application.                         *
*                                                                                       *
*               The following file needs to be openned with Graphics Composer(GC) and   *
*               'saved' to offset 0 of a uSD card.                                      *
*               PoGa Intro\GCI_Demo1\Images\GCI_DEMO1                                   *
*                                                                                       *
*  This program was originally compiled using the Libarty C Compiler (LCC)              *
*  http://www.cs.virginia.edu/~lcc-win32/                                               *
*                                                                                       *
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <windows.h>
#include <bigdemo.h>

#include "Goldelox_const4D.h"			// defines for 4dgl constants, generated by conversion of 4DGL constants to target language
#include "Goldelox_Serial_4DLibrary.h"

unsigned char *atoz = {"abcdefghijklmnopqrstuvwxyz"} ;

// globals for this program
int fmediatests ;

void Usage(char *programName)
{
	fprintf(stderr,"Runs all Serial commands against a display\n\n");
	fprintf(stderr,"%s ComPort [speed]\n\n",programName);
	fprintf(stderr," ComPort    Comms port to which display is attached\n") ;
	fprintf(stderr," speed      Speed at which to begin (default 9600)\n\n") ;
    fprintf(stderr,"A uSD with GCI_DEMO1 file saved at offset 0\n") ;
}


int trymount(void)
{
#define retries 20
	int i ;
	int j ;
	i = media_Init() ;
	j = 0 ;
	if (!i)
	{
		printf("Please insert the uSD card") ;
		while (   (!i)
		       && (j < retries) )
		{
			printf(".") ;
			i = media_Init() ;
			j++ ;
		}
	}
	if (j == retries)
		return FALSE ;
	else
		return TRUE ;
}

void gfx_Part1(void)
{
	int i ;
	gfx_Cls() ;
	txt_BGcolour(LIGHTGOLD) ;           // to ensure text goesn look odd
	txt_FGcolour(RED) ;
	putstr("gfx_A - gfx_L") ;
	printf("gfx_A to gfx_L\n") ;
	txt_FGcolour(LIME) ;            // reset
	gfx_ChangeColour(RED, LIME) ;
	gfx_Circle(30,20,10,BLUE) ;
	gfx_CircleFilled(60,20,10,BLUE) ;
	gfx_Rectangle(40,50,80,60,RED) ;  // draw a rectange to show where we are clipping
	gfx_ClipWindow(40,50,80,60) ;
	gfx_Clipping(ON) ;                  // turn clipping on but just use it for text
	gfx_MoveTo(20,51) ;
	putstr("1234567890asdfghjkl") ;     // this is clipped
	gfx_Clipping(OFF) ;
	sleep(1000) ;
	printf("Display off\n") ;
	gfx_Contrast(0) ;
	sleep(1000) ;
	printf("Display on + Brightness\n") ;
	for (i = 1; i <= 16; i++)
	{
		gfx_Contrast(i) ;
		sleep(100) ;
	}
//	gfx_FrameDelay(6) ;
	printf("X Res= %d  Y Res= %d\n",peekB(GFX_XMAX)+1, peekB(GFX_YMAX)+1) ;
	printf("Pixel at 60,20 is %4.4x\n", gfx_GetPixel(60, 20)) ;
	gfx_Line(0,0,100,200,BLUE) ;
	gfx_LinePattern(0x00aa) ;
	gfx_Set(OBJECT_COLOUR, WHITE);
	gfx_LineTo(90,40) ;
	gfx_LinePattern(0) ;            // reset
	gfx_BGcolour(BLACK) ;           // reset
	txt_BGcolour(BLACK) ;           // reset
	gfx_Contrast(8) ;
}

void gfx_Part2(void)
{
	int i ;
	int k ;
	int l ;
	WORD orbitx ;
	WORD orbity ;
	WORD vx[20] ;    // : array[0..19] of word ;
	WORD vy[20] ; //     : array[0..19] of word ;
	gfx_Cls() ;
	putstr("gfx_M - gfx_O") ;
	printf("gfx_M to gfx_T\n") ;
	k = 46 ;
	l = 32 ;
	gfx_MoveTo(k, l);
	gfx_CircleFilled(k,l,5,BLUE) ;
	i = -90;   // 12 o'clock position
	while (i<270)
	{
		gfx_Orbit(i, 25, &orbitx, &orbity);
		k = 3;
		if ((i % 90) == 0 )
			k = 5;
		gfx_Circle(orbitx , orbity, k, BLUE);
		i += 30;   // each 30 degreees
	}

	gfx_OutlineColour(YELLOW) ;
	gfx_RectangleFilled(80,10,95,60,LIME) ;
	gfx_OutlineColour(0) ;                    // turn outline off

	sleep(1000) ;
	gfx_Cls() ;
	putstr("gfx_P - gfx_P") ;
	printf("gfx_P to gfx_P\n") ;
	vx[0] = 18;   vy[0] = 55;
	vx[1] = 18;   vy[1] = 40;
	vx[2] = 25;   vy[2] = 40;
	vx[3] = 25;   vy[3] = 55;
	vx[4] = 46;   vy[4] = 63;
	vx[5] = 55;   vy[5] = 39;
	vx[6] = 64;   vy[6] = 63;
	vx[7] = 46;   vy[7] = 29;
	vx[8] = 55;   vy[8] = 35;
	vx[9] = 64;   vy[9] = 29;
	vx[10] = 80; vy[10] = 25;
	vx[11] = 80; vy[11] = 39;
	vx[12] = 70; vy[12] = 49;
	vx[13] = 90; vy[13] = 49;
	vx[14] = 80; vy[14] = 39;
	vx[15] = 71; vy[15] = 29;
	vx[16] = 80; vy[16] = 35;
	vx[17] = 89; vy[17] = 29;
	// house
	gfx_Rectangle(3,25,33,55,RED);             // frame
	gfx_Triangle(3,25,18,4,33,25,YELLOW);       // roof
	gfx_Polyline(4, vx, vy, CYAN);            // door
	gfx_PutPixel(20, 47, LIME) ;          // door knob
	// man
	gfx_Circle(55, 13, 10, BLUE);               // head
	gfx_Line(55, 25, 55, 39, BLUE);             // body
	gfx_Polyline(3, &vx[4], &vy[4], CYAN);      // legs
	gfx_Polyline(3, &vx[7], &vy[7], BLUE);      // arms
	// woman
	gfx_Circle(80, 13, 10, PINK);              // head
	gfx_Polyline(5, &vx[10], &vy[10], BROWN);   // dress
	gfx_Line(74, 63, 76, 49, PINK);          // left arm
	gfx_Line(82, 49, 86, 63, PINK);          // right arm
	gfx_Polyline(3, &vx[15], &vy[15], SALMON);  // dress

	sleep(1000) ;
	gfx_Cls() ;
	putstr("gfx_P - gfx_T") ;
	printf("gfx_P to gfx_T\n") ;
	vx[0] = 45; vy[0] = 20;
	vx[1] = 55; vy[1] = 10;
	vx[2] = 95; vy[2] = 20;
	vx[3] = 60; vy[3] = 25;
	vx[4] = 95; vy[4] = 30;
	vx[5] = 55; vy[5] = 40;
	vx[6] = 45; vy[6] = 30;
	gfx_Polygon(7, vx, vy, RED);

	gfx_Rectangle(0,10, 10,60, AQUA) ;
	gfx_RectangleFilled(20,10, 40,60, ORANGE) ;
//	gfx_ScreenMode(LANDSCAPE) ;
//	gfx_ScreenMode(PORTRAIT) ;
	gfx_Transparency(ON) ;
	gfx_TransparentColour(YELLOW) ;  // how do we 'test' this?
	gfx_Triangle(50,60, 70,40, 90,60,YELLOW);
}

void Joystick_Tests(void)
{
	int i ;
 	int oldi ;
	gfx_Cls() ;
	printf("Joystick Tests\n") ;
	putstr("Joystick Tests\nPress Fire to\nExit") ;
	txt_FGcolour(RED) ;
	i = -1 ;
	while (i != FIRE)
	{
		oldi = i ;
		i = joystick() ;
		if (i != oldi)
		{
			txt_MoveCursor(5,3) ;
			switch (i)
		    {
				case RELEASED : putstr("Released") ;
					break ;
				case UP       : putstr("   Up   ") ;
					break ;
				case LEFT     : putstr("  Left  ") ;
					break ;
				case DOWN     : putstr("  Down  ") ;
					break ;
				case RIGHT    : putstr("  Right ") ;
					break ;
				case FIRE     : putstr("  Fire  ") ;
					break ;
			}
		}
	}

	txt_FGcolour(LIME) ;
}

void text_Tests(void)
{
	gfx_Cls() ;
	printf("Text Tests\n") ;
	putstr("Text Tests") ;

	txt_Attributes(BOLD + INVERSE + ITALIC + UNDERLINED) ;
	txt_Xgap(3) ;
	txt_Ygap(3) ;
	txt_BGcolour(YELLOW) ;
	txt_FGcolour(WHITE) ;
	txt_FontID(SYSTEM) ;
	txt_MoveCursor(2, 0) ;
	putstr("Hello There") ;

	txt_MoveCursor(4, 1) ;
	txt_Height(2) ;
	txt_Width(1) ;
	txt_Inverse(OFF) ;
	txt_Italic(OFF) ;
	txt_Opacity(TRANSPARENT) ;
	txt_Set(TEXT_COLOUR, LIME) ;
	txt_Underline(ON) ;
	txt_Bold(OFF) ;
	putstr("Hello There") ;
	txt_Height(1) ;
	txt_Width(1) ;
	putCH('z') ;
	printf("Char height= %d Width= %d\n", charheight('w'), charwidth('w') ) ;
	txt_BGcolour(BLACK) ;
	txt_FGcolour(LIME) ;
	txt_FontID(SYSTEM) ;
	txt_MoveCursor(0,0) ;      // reset
}

void Media_Tests(void)
{
	int i ;
	int j ;
	int k ;
	int l ;
	int m , handle ;
	gfx_Cls() ;
	putstr("Media Tests") ;
	printf("Media Tests\n") ;
	media_SetAdd(3, 0x600) ;     // using addresses from .dat file
	media_Image(20,20) ;
	sleep(1000) ;

	media_SetAdd(0, 0x3800) ;    // ditto
	media_Video(0,0) ;

	media_SetSector(1, 0) ;     // pic a sector not used by the 'above'
	media_WriteByte(0x11) ;
	media_WriteWord(0x2233) ;
	media_Flush() ;            // should write 0xFF over the rest of the sector
	media_SetSector(1, 0) ;
	printf("\n%2.2x %4.4x %4.4x\n",media_ReadByte(), media_ReadWord(), media_ReadWord());
}


int Callback(int ErrCode, unsigned char Errbyte)
{
	printf("Serial 4D Library reports error %s", Error4DText[ErrCode]) ;
	if (ErrCode == Err4D_NAK)
		printf(" returned data= %x\n", Errbyte) ;
	else
		printf("\n") ;
	exit(2) ;
	return 0 ; // to make compiler happy
}

int main(int argc,char *argv[])
{
	int comspeed ;
	int i, rc ;
	char comport[20] ;
	char wks[20] ;
	TimeLimit4D   = 5000 ; // 2 second timeout on all commands
	Callback4D = Callback ; // NULL ;
 	//Error_Abort4D = TRUE ; // abort on detected 4D Serial error
	if (argc == 1) {
		/* If no arguments we call the Usage routine and exit */
		Usage(argv[0]);
		return 1;
	}
    strcpy(comport, argv[1] ) ;
    strupr(comport) ;
	if (argc <= 2)
		comspeed = BAUD_9600 ;
	else
	{
		for (i = 0; i < maxrates; i++)
		{
			itoa(baudrates[i], wks, 10) ;
			if (!strcmp(wks,argv[2]))
				break ;
		}
		if (i == maxrates)
		{
			printf("Invalid baud rate %s",argv[2]) ;
			return 1;
		}
	    comspeed = i ;
	}
//	printf("%s %d\n", comport, comspeed) ;
	rc = OpenComm(comport, comspeed) ;
 	if (rc != 0)
  	{
    	printf("Error %d Opening %s", /*syserrormessage(*/rc/*)*/, comport) ;
    	return 2 ;
	}

	gfx_Cls() ;
	fmediatests = trymount() ;
	sys_GetModel(wks) ; // length is also returned, but we don't need that here
	printf("Display model: %s\n", wks) ;
	putstr("Model:\n") ;
    putstr(wks) ;
	i = strlen(wks) ;
	printf("SPE2 Version: %4.4x\n", sys_GetVersion()) ;
	printf("PmmC Version: %4.4x\n", sys_GetPmmC()) ;
	if (fmediatests)
  		printf("Media tests will be done\n") ;
  	else
 	    printf("Media tests cannot be done, missing uSD card\n") ;

	sleep(5000) ;

	gfx_Part1() ; // GFX Part 1 tests
	sleep(5000) ;

	gfx_Part2() ; // GFX Part 2 tests
	sleep(5000) ;

	text_Tests() ; // text tests
	sleep(5000) ;

	Joystick_Tests() ;
	sleep(5000) ;

	gfx_Cls() ;
	blitComtoDisplay(0, 0, Image[1], Image[3], &Image[6]) ;
 	sleep(5000) ;

	if (fmediatests)
	{
		Media_Tests() ;
		sleep(5000) ;
	}

//  SSTimeout(1000) ;
//  SSSpeed(1) ;
//  SSMode(0) ;

	BeeP(40,2000) ;

	setbaudWait(BAUD_115200) ;
	putstr("Hi at 115200\n") ;
	setbaudWait(BAUD_9600) ;
	putstr("Back to 9600\n") ;

	return 0;
}
