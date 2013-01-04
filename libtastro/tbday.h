#ifndef TBDAY_H
#define TBDAY_H

#include "system.h" // for epoch  

// the different types of day (ommited part):
#define NORMAL 0 // no omited nor duplicated
#define PREVIOUS_OMMITED 2 // previous lunar day is ommited
#define NEXT_OMMITED 3 // next lunar day is ommited
// other types of day (duplicated part)
#define FIRST_OF_DUPLICATED 4 // first of a duplicated day
#define SECOND_OF_DUPLICATED 5 // second of a duplicated day
// types for the month:
#define FIRST_OF_DOUBLE 1
#define SECOND_OF_DOUBLE 2

typedef struct tib_month
{
    long int year; // the western calendar year
    int rabjung; // the indian cycle the year is in (number 1 is the one starting in 1027)
    unsigned char yor; // the year inside the rabjung, starting with yor 0 for the first
    unsigned char year_animal; // animal, element and gender (ex: 2011 is female iron rabbit)
    unsigned char year_element;
    unsigned char year_gender;
    unsigned char year_sme_ba;
    long int month; // the adjusted month as appearing in the almanach
    unsigned char month_type;
    long int true_month[2]; // the true month
    unsigned char zero_month_flag; // 0 if it is the second time we are on the 0th or 1st month, 1 if it's the first or if we are in another month
    long int start_gd; // the general day for tt=0 in the month
    long int rilcha[2]; // the anomaly
    long int gzadru[6]; // month mean weekday
    long int nyidru[6]; // month mean solar longitude
} tib_month;

typedef struct tib_day
{
  tib_month *month;
  long int gd; // the general day (spi zag)
  long int tt; // the lunar day
  long int ommited; // a type above to know if there is an ommited day around
  long int duplicated; // idem, for duplicated
  long int nyibar[6]; // mean solar longitude
  long int nyidag[6]; // true solar longitude
  long int gzadag[6]; // true weekday
  unsigned char lm_db; // lunar mansion at daybreak
  unsigned char yoga; // yoga, see KTC p.42
  unsigned char karana; // Karanas are numbered from 0 to 7 for the changing karanas, and from 7 to
  unsigned char sideral_day[3]; // the zodiacal sign (month), day and nadi for the mean solar longitude
  // 10 for the fixed ones, see KTC p.43
  // should the planets be separate?
  // the lists are 6 numbers long for intermediate calculations
    long int marmurdag[6]; // the longitude of mars for the day
    long int jupmurdag[6]; // jupiter
    long int satmurdag[6]; // saturn
    long int mermurdag[6]; // mercury
    long int venmurdag[6]; // venus
    long int rahudong[5]; // rahu
} tib_day;

void zla_dag (const epoch *epch, long int y, long int m, long int zd[2]);
void nyi_dag_and_gza_dag ( long int nyibar[6], long int tsebar[6], long int rilcha[2], long int tt, long int nyidag[6], long int gzadag[6]) ;
long int spi_zag (epoch *epch, long int cur_mth, long int tt, long int sd);
int get_tt_data(epoch *epch, long int cur_mth, long int gzadru[6], long int nyidru[6], long int rilcha[6], long int tt, long int nyidag[6], long int gzadag[6], long int nyibar[6]);
void get_day_data(long int jd, tib_day *td, astro_system *sys);
tib_day * new_tib_day();
void get_month_data (epoch *epch, long int zd0, long int rilcha[2], long int nyidru[6], long int gzadru[6]);
void free_tib_day(tib_day *td);
tib_month * new_tib_month();
long int adj_zla (long int tm, long int zd[2], 	unsigned char *zeromthfg, astro_system *sys);


#endif
