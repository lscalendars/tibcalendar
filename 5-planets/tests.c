/*********************************************************************************
License for Kcal - Kalacakra calendar software

Copyright (c) 2011 Edward Henning

Permission is hereby granted, free of charge, to any person  obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so, subject
to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
************************************************************************************/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include "jd.h"

double HLTest, HBTest, HRTest;  // General variables
double HXTest, HYTest, HZTest;
double HL, HB, HR; // heliocentric long, lat, radius.
double HX, HY, HZ; // heliocentric x, y, z.
double HX2, HY2, HZ2; // heliocentric x, y, z.
double maxdif = 0.0;

extern double MerHL, MerHB, MerHR; // Mercury, heliocentric long, lat, radius.
extern double MerHX, MerHY, MerHZ; // Mercury, heliocentric x, y, z.
extern double MerHX2, MerHY2, MerHZ2; // Mercury, heliocentric x, y, z.

extern double VenHL, VenHB, VenHR; // Venus, heliocentric long, lat, radius.
extern double VenHX, VenHY, VenHZ; // Venus, heliocentric x, y, z.
extern double VenHX2, VenHY2, VenHZ2; // Venus, heliocentric x, y, z.

extern double EarHL, EarHB, EarHR; // Earth, heliocentric long, lat, radius.
extern double EarHX, EarHY, EarHZ; // Earth, heliocentric x, y, z.
//extern double EarHX2, EarHY2, EarHZ2; // Earth, heliocentric long, lat,
                                      // radius.

extern double MarHL, MarHB, MarHR; // Mars, heliocentric long, lat, radius.
extern double MarHX, MarHY, MarHZ; // Mars, heliocentric x, y, z.
extern double MarHX2, MarHY2, MarHZ2; // Mars, heliocentric x, y, z.

extern double JupHL, JupHB, JupHR; // Jupiter, heliocentric long, lat, radius.
extern double JupHX, JupHY, JupHZ; // Jupiter, heliocentric x, y, z.
extern double JupHX2, JupHY2, JupHZ2; // Jupiter, heliocentric x, y, z.

extern double SatHL, SatHB, SatHR; // Saturn, heliocentric long, lat, radius.
extern double SatHX, SatHY, SatHZ; // Saturn, heliocentric x, y, z.
extern double SatHX2, SatHY2, SatHZ2; // Saturn, heliocentric x, y,

extern double T;
// In this file:
void dis_test ( void );
void test_mercury ( void );
void test_venus ( void );
void test_earth ( void );
void test_mars ( void );
void test_jupiter ( void );
void test_saturn ( void );
void run_heliotests ( void );

// External:
double set_time_data ( double );
void UTET ( int, int, int, int, int, int, double *, double * );
void clc_merlon ( double );
void clc_merrct ( double );
void clc_venlon ( double );
void clc_venrct ( double );
void clc_earlon ( double );
void clc_earrct ( double );
void clc_marlon ( double );
void clc_marrct ( double );
void clc_juplon ( double );
void clc_juprct ( double );
void clc_satlon ( double );
void clc_satrct ( double );

void run_heliotests ( void )
  {
    test_earth ();
    test_mercury ();
    test_venus ();
    test_mars ();
    test_jupiter ();
    test_saturn ();
  }

void dis_test ( void )
  {
    double DX, DY, DZ;
//  goto testbit;
    printf ( "Heliocentric longitude = %.10f\n", HL );
    printf ( "Correct answer = %.10f, Error = %.10f\n",
              HLTest, HL - HLTest );
    printf ( "Heliocentric latitude = %.10f\n", HB );
    printf ( "Correct answer = %.10f, Error = %.10f\n",
              HBTest, HB - HBTest );
    printf ( "Radius vector = %.10f\n", HR );
    printf ( "Correct answer = %.10f, Error = %.10f\n",
              HRTest, HR - HRTest );

    printf ( "Rectangular X = %.10f\n", HX );
    printf ( "Correct answer = %.10f, Error = %.10f\n",
              HXTest, HX - HXTest );
    printf ( "Rectangular Y = %.10f\n", HY );
    printf ( "Correct answer = %.10f, Error = %.10f\n",
              HYTest, HY - HYTest );
    printf ( "Rectangular Z = %.10f\n", HZ );
    printf ( "Correct answer = %.10f, Error = %.10f\n",
              HZTest, HZ - HZTest );

    // Make rectangular:

//  testbit:

    HX2 = HR * cos ( HB ) * cos ( HL );
    HY2 = HR * cos ( HB ) * sin ( HL );
    HZ2 = HR * sin ( HB );

    DX = HX2 - HX;
    DY = HY2 - HY;
    DZ = HZ2 - HZ;

    printf ( "X1 = %.10f, X2 = %.10f, Dif = %.10f\n",
              HX, HX2, HX2 - HX );
    printf ( "Y1 = %.10f, Y2 = %.10f, Dif = %.10f\n",
              HY, HY2, HY2 - HY );
    printf ( "Z1 = %.10f, Z2 = %.10f, Dif = %.10f\n",
              HZ, HZ2, HZ2 - HZ );
    if ( DX < 0.0 )
      DX = -DX;
    if ( DY < 0.0 )
      DY = -DY;
    if ( DZ < 0.0 )
      DZ = -DZ;

    if ( DX > maxdif )
      maxdif = DX;
    if ( DY > maxdif )
      maxdif = DY;
    if ( DZ > maxdif )
      maxdif = DZ;
  } // END - dis_test ()


void test_earth ( void )
  {
    double JD, ET;
//---------------------------------------------------------
    printf ( "\nEARTH:\n\n");
    UTET ( 1099, 12, 19, 12, 0, 0, &JD, &ET ); // ALL OK

    HLTest = 1.6367193623;
    HBTest = -0.0000031292;
    HRTest = 0.9830331815;

    HXTest = -0.0647576071;
    HYTest = 0.9808979021;
    HZTest = -0.0000030761;

// THESE CALCULATION USE BARYCENTRIC DYNAMICAL TIME. DO NOT NEED CONVERSION.

    set_time_data ( JD );  // MUST BE FIRST!!
//  set_time_data ( ET );  // MUST BE FIRST!!

// T is in Julian centuries before 2000.0, ie:

    T = ( JD - 2451545.0 ) / 36525.0;

    clc_earlon ( T / 10.0 );
    HL = EarHL; HB = EarHB; HR = EarHR;
    clc_earrct ( T / 10.0 );
    HX = EarHX; HY = EarHY; HZ = EarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nEARTH:\n\n");
    UTET ( 1199, 12, 19, 12, 0, 0, &JD, &ET );
    HLTest = 1.6495143197;
    HBTest = -0.0000013003;
    HRTest = 0.9830440397;
    HXTest = -0.0773033683;
    HYTest = 0.9799998884;
    HZTest = -0.0000012777;

//  set_time_data ( JD );  // MUST BE FIRST!!
    set_time_data ( ET );  // MUST BE FIRST!!

    clc_earlon ( T / 10.0 );
    HL = EarHL; HB = EarHB; HR = EarHR;
    clc_earrct ( T / 10.0 );
    HX = EarHX; HY = EarHY; HZ = EarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nEARTH:\n\n");
    UTET ( 1299, 12, 19, 12, 0, 0, &JD, &ET );
    HLTest = 1.6622048657;
    HBTest = 0.0000015133;
    HRTest = 0.9830942385;
    HXTest = -0.0897381298;
    HYTest = 0.9789899623;
    HZTest = 0.0000014875;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_earlon ( T / 10.0 );
    HL = EarHL; HB = EarHB; HR = EarHR;
    clc_earrct ( T / 10.0 );
    HX = EarHX; HY = EarHY; HZ = EarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nEARTH:\n\n");
    UTET ( 1599, 12, 29, 12, 0, 0, &JD, &ET );
    HLTest = 1.7006065938;
    HBTest = -0.0000016359;
    HRTest = 0.9831254376;
    HXTest = -0.1272616591;
    HYTest = 0.9748538870;
    HZTest = -0.0000016090;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_earlon ( T / 10.0 );
    HL = EarHL; HB = EarHB; HR = EarHR;
    clc_earrct ( T / 10.0 );
    HX = EarHX; HY = EarHY; HZ = EarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
//  UTET ( 1699, 12, 29, 12, 0, 0, &JD, &ET );
//---------------------------------------------------------
    printf ( "\nEARTH:\n\n");
    UTET ( 1799, 12, 30, 12, 0, 0, &JD, &ET );
    HLTest = 1.7262638916;
    HBTest = 0.0000002083;
    HRTest = 0.9832274321;
    HXTest = -0.1522449491;
    HYTest = 0.9713689619;
    HZTest = 0.0000002038;
    set_time_data ( JD );  // MUST BE FIRST!!

    clc_earlon ( T / 10.0 );
    HL = EarHL; HB = EarHB; HR = EarHR;
    clc_earrct ( T / 10.0 );
    HX = EarHX; HY = EarHY; HZ = EarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
//  UTET ( 1899, 12, 31, 12, 0, 0, &JD, &ET );
//---------------------------------------------------------
    printf ( "\nEARTH:\n\n");
    UTET ( 2000, 1, 1, 12, 0, 0, &JD, &ET );   // ALL OK
    HLTest = 1.7519238681;
    HBTest = -0.0000039656;
    HRTest = 0.9833276819;
    HXTest = -0.1771354615;
    HYTest = 0.9672416229;
    HZTest = -0.0000039000;
    set_time_data ( JD );  // MUST BE FIRST!!

    clc_earlon ( T / 10.0 );
    HL = EarHL; HB = EarHB; HR = EarHR;
    clc_earrct ( T / 10.0 );
    HX = EarHX; HY = EarHY; HZ = EarHZ;
    dis_test ();
    getch ();
 } // END - test_earth ()

void test_mercury ( void )
  {
    double JD, ET;
//---------------------------------------------------------
    printf ( "\nMERCURY:\n\n");
    UTET ( 1099, 12, 19, 12, 0, 0, &JD, &ET ); // ALL OK

    HLTest = 5.0128397764;
    HBTest = -0.1143275808;
    HRTest = 0.4352063237;
    HXTest = 0.1279588350;
    HYTest = -0.4129965856;
    HZTest = -0.0496477652;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_merlon ( T / 10.0 );
    HL = MerHL; HB = MerHB; HR = MerHR;
    clc_merrct ( T / 10.0 );
    HX = MerHX; HY = MerHY; HZ = MerHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMERCURY:\n\n");
    UTET ( 1199, 12, 19, 12, 0, 0, &JD, &ET );
    HLTest = 6.2819826060;
    HBTest = -0.0768697084;
    HRTest = 0.3414354250;
    HXTest = 0.3404269126;
    HYTest = -0.0004094321;
    HZTest = -0.0262202012;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_merlon ( T / 10.0 );
    HL = MerHL; HB = MerHB; HR = MerHR;
    clc_merrct ( T / 10.0 );
    HX = MerHX; HY = MerHY; HZ = MerHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMERCURY:\n\n");
    UTET ( 1299, 12, 19, 12, 0, 0, &JD, &ET );
    HLTest = 1.8738888759;
    HBTest = 0.1126774697;
    HRTest = 0.3209366232;
    HXTest = -0.0951835365;
    HYTest = 0.3043652763;
    HZTest = 0.0360858544;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_merlon ( T / 10.0 );
    HL = MerHL; HB = MerHB; HR = MerHR;
    clc_merrct ( T / 10.0 );
    HX = MerHX; HY = MerHY; HZ = MerHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMERCURY:\n\n");
    UTET ( 1599, 12, 29, 12, 0, 0, &JD, &ET );
    HLTest = 5.1836421820;
    HBTest = -0.1170914848;
    HRTest = 0.4326517759;
    HXTest = 0.1950803107;
    HYTest = -0.3828531309;
    HZTest = -0.0505441569;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_merlon ( T / 10.0 );
    HL = MerHL; HB = MerHB; HR = MerHR;
    clc_merrct ( T / 10.0 );
    HX = MerHX; HY = MerHY; HZ = MerHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
//  UTET ( 1699, 12, 29, 12, 0, 0, &JD, &ET );
//---------------------------------------------------------
    printf ( "\nMERCURY:\n\n");
    UTET ( 1799, 12, 30, 12, 0, 0, &JD, &ET );
    HLTest = 2.0737894888;
    HBTest = 0.1168184804;
    HRTest = 0.3233909533;
    HXTest = -0.1548281758;
    HYTest = 0.2814058364;
    HZTest = 0.0376921749;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_merlon ( T / 10.0 );
    HL = MerHL; HB = MerHB; HR = MerHR;
    clc_merrct ( T / 10.0 );
    HX = MerHX; HY = MerHY; HZ = MerHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
//  UTET ( 1899, 12, 31, 12, 0, 0, &JD, &ET );
//---------------------------------------------------------
    printf ( "\nMERCURY:\n\n");
    UTET ( 2000, 1, 1, 12, 0, 0, &JD, &ET );   // ALL OK
    HLTest = 4.4293481036;
    HBTest = -0.0527573409;
    HRTest = 0.4664714751;
    HXTest = -0.1300934112;
    HYTest = -0.4472876717;
    HZTest = -0.0245983802;
    set_time_data ( JD );    // MUST BE FIRST!!
    clc_merlon ( T / 10.0 );
    HL = MerHL; HB = MerHB; HR = MerHR;
    clc_merrct ( T / 10.0 );
    HX = MerHX; HY = MerHY; HZ = MerHZ;
    dis_test ();
    getch ();
 } // END - test_mercury ()

void test_venus ( void )
  {
    double JD, ET;
//---------------------------------------------------------
    printf ( "\nVENUS:\n\n");
    UTET ( 1099, 12, 19, 12, 0, 0, &JD, &ET ); // ALL OK
    HLTest = 3.3145399295;
    HBTest = 0.0505016053;
    HRTest = 0.7215819783;
    HXTest = -0.7099110735;
    HYTest = -0.1240161268;
    HZTest = 0.0364255617;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_venlon ( T / 10.0 );
    HL = VenHL; HB = VenHB; HR = VenHR;
    clc_venrct ( T / 10.0 );
    HX = VenHX; HY = VenHY; HZ = VenHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nVENUS:\n\n");
    UTET ( 1199, 12, 19, 12, 0, 0, &JD, &ET );
    HLTest = 0.4804699931;
    HBTest = -0.0395505250;
    HRTest = 0.7235430458;
    HXTest = 0.6411201421;
    HYTest = 0.3341571871;
    HZTest = -0.0286090464;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_venlon ( T / 10.0 );
    HL = VenHL; HB = VenHB; HR = VenHR;
    clc_venrct ( T / 10.0 );
    HX = VenHX; HY = VenHY; HZ = VenHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nVENUS:\n\n");
    UTET ( 1299, 12, 19, 12, 0, 0, &JD, &ET );
    HLTest = 3.9850309909;
    HBTest = 0.0222342485;
    HRTest = 0.7247441174;
    HXTest = -0.4817622080;
    HYTest = -0.5412019869;
    HZTest = 0.0161128116;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_venlon ( T / 10.0 );
    HL = VenHL; HB = VenHB; HR = VenHR;
    clc_venrct ( T / 10.0 );
    HX = VenHX; HY = VenHY; HZ = VenHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nVENUS:\n\n");
    UTET ( 1599, 12, 29, 12, 0, 0, &JD, &ET );
    HLTest = 1.8291359617;
    HBTest = 0.0311394084;
    HRTest = 0.7186375037;
    HXTest = -0.1835053718;
    HYTest = 0.6944530427;
    HZTest = 0.0223743314;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_venlon ( T / 10.0 );
    HL = VenHL; HB = VenHB; HR = VenHR;
    clc_venrct ( T / 10.0 );
    HX = VenHX; HY = VenHY; HZ = VenHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
//  UTET ( 1699, 12, 29, 12, 0, 0, &JD, &ET );
//---------------------------------------------------------
    printf ( "\nVENUS:\n\n");
    UTET ( 1799, 12, 30, 12, 0, 0, &JD, &ET );
    HLTest = 2.5083656668;
    HBTest = 0.0552309407;
    HRTest = 0.7185473298;
    HXTest = -0.5783536698;
    HYTest = 0.4245514325;
    HZTest = 0.0396658692;

    set_time_data ( JD );  // MUST BE FIRST!!

    clc_venlon ( T / 10.0 );
    HL = VenHL; HB = VenHB; HR = VenHR;
    clc_venrct ( T / 10.0 );
    HX = VenHX; HY = VenHY; HZ = VenHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
//  UTET ( 1899, 12, 31, 12, 0, 0, &JD, &ET );
//---------------------------------------------------------
    printf ( "\nVENUS:\n\n");
    UTET ( 2000, 1, 1, 12, 0, 0, &JD, &ET );   // ALL OK
    HLTest = 3.1870221833;
    HBTest = 0.0569782849;
    HRTest = 0.7202129253;
    HXTest = -0.7183022790;
    HYTest = -0.0326545995;
    HZTest = 0.0410142975;

    set_time_data ( JD );    // MUST BE FIRST!!
    clc_venlon ( T / 10.0 );
    HL = VenHL; HB = VenHB; HR = VenHR;
    clc_venrct ( T / 10.0 );
    HX = VenHX; HY = VenHY; HZ = VenHZ;
    dis_test ();
    getch ();
 } // END - test_venus ()

void test_mars ( void )
  {
    double JD, ET;
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1099, 12, 19, 12, 0, 0, &JD, &ET ); // ALL OK
    HLTest = 2.7367104344;
    HBTest = 0.0295522719;
    HRTest = 1.6571002307;
    HXTest = -1.5224566563;
    HYTest = 0.6524641789;
    HZTest = 0.0489639473;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1199, 12, 19, 12, 0, 0, &JD, &ET );

    HLTest = 3.6939294875;
    HBTest = 0.0065885509;
    HRTest = 1.5593802008;
    HXTest = -1.3274727561;
    HYTest = -0.8181547191;
    HZTest = 0.0102739788;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1299, 12, 19, 12, 0, 0, &JD, &ET );

    HLTest = 4.8193924948;
    HBTest = -0.0255031923;
    HRTest = 1.4208707215;
    HXTest = 0.1516988677;
    HYTest = -1.4122847590;
    HZTest = -0.0362328071;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1399, 12, 19, 12, 0, 0, &JD, &ET );

    HLTest = 6.0979760762;
    HBTest = -0.0266794243;
    HRTest = 1.3925964529;
    HXTest = 1.3682927953;
    HYTest = -0.2563584217;
    HZTest = -0.0371492613;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1499, 12, 19, 12, 0, 0, &JD, &ET ); // ALL OK

    HLTest = 1.0050966939;
    HBTest = 0.0066676098;
    HRTest = 1.5123622690;
    HXTest = 0.8106181241;
    HYTest = 1.2767287234;
    HZTest = 0.0100837679;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1599, 12, 29, 12, 0, 0, &JD, &ET );

    HLTest = 2.0058210394;
    HBTest = 0.0300702181;
    HRTest = 1.6371997207;
    HXTest = -0.6896577496;
    HYTest = 1.4840391395;
    HZTest = 0.0492235328;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1699, 12, 29, 12, 0, 0, &JD, &ET );

    HLTest = 2.9166648690;
    HBTest = 0.0280268149;
    HRTest = 1.6584697082;
    HXTest = -1.6160583003;
    HYTest = 0.3697531114;
    HZTest = 0.0464755338;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1799, 12, 30, 12, 0, 0, &JD, &ET );

    HLTest = 3.8711855478;
    HBTest = 0.0034969939;
    HRTest = 1.5615140011;
    HXTest = -1.1640169843;
    HYTest = -1.0408461240;
    HZTest = 0.0054605962;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 1899, 12, 31, 12, 0, 0, &JD, &ET );

    HLTest = 4.9942005211;
    HBTest = -0.0271965869;
    HRTest = 1.4218777705;
    HXTest = 0.3952725706;
    HYTest = -1.3652842040;
    HZTest = -0.0386654530;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nMARS:\n\n");
    UTET ( 2000, 1, 1, 12, 0, 0, &JD, &ET );

    HLTest = 6.2735389983;
    HBTest = -0.0247779824;
    HRTest = 1.3912076925;
    HXTest = 1.3907159218;
    HYTest = -0.0134157060;
    HZTest = -0.0344677967;

    set_time_data ( JD );    // MUST BE FIRST!!
    clc_marlon ( T / 10.0 );
    HL = MarHL; HB = MarHB; HR = MarHR;
    clc_marrct ( T / 10.0 );
    HX = MarHX; HY = MarHY; HZ = MarHZ;
    dis_test ();
    getch ();
 } // END - test_mars ()

void test_jupiter ( void )
  {
    double JD, ET;
//---------------------------------------------------------
    printf ( "\nJUPITER:\n\n");
    UTET ( 1099, 12, 19, 12, 0, 0, &JD, &ET ); // ALL OK
    HLTest = 1.2695066546;
    HBTest = -0.0075335740;
    HRTest = 5.1193587362;
    HXTest = 1.5191368970;
    HYTest = 4.8886161890;
    HZTest = -0.0385666565;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_juplon ( T / 10.0 );
    HL = JupHL; HB = JupHB; HR = JupHR;
    clc_juprct ( T / 10.0 );
    HX = JupHX; HY = JupHY; HZ = JupHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nJUPITER:\n\n");
    UTET ( 1399, 12, 19, 12, 0, 0, &JD, &ET );

    HLTest = 3.0889515350;
    HBTest = 0.0231157947;
    HRTest = 5.4491570191;
    HXTest = -5.4401549180;
    HYTest = 0.2866406676;
    HZTest = 0.1259503903;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_juplon ( T / 10.0 );
    HL = JupHL; HB = JupHB; HR = JupHR;
    clc_juprct ( T / 10.0 );
    HX = JupHX; HY = JupHY; HZ = JupHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nJUPITER:\n\n");
    UTET ( 1599, 12, 29, 12, 0, 0, &JD, &ET );

    HLTest = 2.3348832684;
    HBTest = 0.0140523907;
    HRTest = 5.3439455032;
    HXTest = -3.6969935264;
    HYTest = 3.8580245750;
    HZTest = 0.0750927480;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_juplon ( T / 10.0 );
    HL = JupHL; HB = JupHB; HR = JupHR;
    clc_juprct ( T / 10.0 );
    HX = JupHX; HY = JupHY; HZ = JupHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nJUPITER:\n\n");
    UTET ( 1799, 12, 30, 12, 0, 0, &JD, &ET );

    HLTest = 1.5255696771;
    HBTest = -0.0043606936;
    HRTest = 5.1318457604;
    HXTest = 0.2320147906;
    HYTest = 5.1265494302;
    HZTest = -0.0223783436;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_juplon ( T / 10.0 );
    HL = JupHL; HB = JupHB; HR = JupHR;
    clc_juprct ( T / 10.0 );
    HX = JupHX; HY = JupHY; HZ = JupHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nJUPITER:\n\n");
    UTET ( 2000, 1, 1, 12, 0, 0, &JD, &ET );

    HLTest = 0.6334614186;
    HBTest = -0.0205001039;
    HRTest = 4.9653813154;
    HXTest = 4.0011739729;
    HYTest = 2.9385810260;
    HZTest = -0.1017837501;

    set_time_data ( JD );    // MUST BE FIRST!!
    clc_juplon ( T / 10.0 );
    HL = JupHL; HB = JupHB; HR = JupHR;
    clc_juprct ( T / 10.0 );
    HX = JupHX; HY = JupHY; HZ = JupHZ;
    dis_test ();
    getch ();
 } // END - test_jupiter ()

void test_saturn ( void )
  {
    double JD, ET;
//---------------------------------------------------------
    printf ( "\nSATURN:\n\n");
    UTET ( 1099, 12, 19, 12, 0, 0, &JD, &ET ); // ALL OK

    HLTest = 3.5570108069;
    HBTest = 0.0435371139;
    HRTest = 9.8669939498;

    HXTest = -9.0192283867;
    HYTest = -3.9782745625;
    HZTest = 0.4294447827;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_satlon ( T / 10.0 );
    HL = SatHL; HB = SatHB; HR = SatHR;
    clc_satrct ( T / 10.0 );
    HX = SatHX; HY = SatHY; HZ = SatHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nSATURN:\n\n");
    UTET ( 1399, 12, 19, 12, 0, 0, &JD, &ET );

    HLTest = 4.6913199264;
    HBTest = 0.0146771898;
    HRTest = 10.1065692994;
    HXTest = -0.2128973849;
    HYTest = -10.1032378666;
    HZTest = 0.1483307262;
    set_time_data ( JD );  // MUST BE FIRST!!
    clc_satlon ( T / 10.0 );
    HL = SatHL; HB = SatHB; HR = SatHR;
    clc_satrct ( T / 10.0 );
    HX = SatHX; HY = SatHY; HZ = SatHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nSATURN:\n\n");
    UTET ( 1599, 12, 29, 12, 0, 0, &JD, &ET ); // ALL OK

    HLTest = 3.5217555199;
    HBTest = 0.0437035058;
    HRTest = 9.7571035629;
    HXTest = -9.0518360159;
    HYTest = -3.6171280351;
    HZTest = 0.4262838732;

    set_time_data ( JD );  // MUST BE FIRST!!
    clc_satlon ( T / 10.0 );
    HL = SatHL; HB = SatHB; HR = SatHR;
    clc_satrct ( T / 10.0 );
    HX = SatHX; HY = SatHY; HZ = SatHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nSATURN:\n\n");
    UTET ( 1799, 12, 30, 12, 0, 0, &JD, &ET );

    HLTest = 2.1956677359;
    HBTest = 0.0104156566;
    HRTest = 9.1043068639;
    HXTest = -5.3256669097;
    HYTest = 7.3835415374;
    HZTest = 0.0948256372;
    set_time_data ( JD );  // MUST BE FIRST!!
    clc_satlon ( T / 10.0 );
    HL = SatHL; HB = SatHB; HR = SatHR;
    clc_satrct ( T / 10.0 );
    HX = SatHX; HY = SatHY; HZ = SatHZ;
    dis_test ();
    getch ();
//---------------------------------------------------------
    printf ( "\nSATURN:\n\n");
    UTET ( 2000, 1, 1, 12, 0, 0, &JD, &ET );   // ALL OK

    HLTest = 0.7980038761;
    HBTest = -0.0401984149;
    HRTest = 9.1838483715;
    HXTest = 6.4064067820;
    HYTest = 6.5699928470;
    HZTest = -0.3690768029;

    set_time_data ( JD );    // MUST BE FIRST!!
    clc_satlon ( T / 10.0 );
    HL = SatHL; HB = SatHB; HR = SatHR;
    clc_satrct ( T / 10.0 );
    HX = SatHX; HY = SatHY; HZ = SatHZ;
    dis_test ();
    getch ();
 } // END - test_saturn ()
                 