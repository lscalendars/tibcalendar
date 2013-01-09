#ifndef TBSTRUCTURES_H
#define TBSTRUCTURES_H

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

// planetary data
typedef struct tib_planet_data
{
  // the lists are 6 numbers, long for intermediate calculations
    long int marmurdag[6]; // the longitude of mars for the day
    long int jupmurdag[6]; // jupiter
    long int satmurdag[6]; // saturn
    long int mermurdag[6]; // mercury
    long int venmurdag[6]; // venus
    long int rahudong[5]; // rahu
} tib_planet_data;

// astrological data for year, month and day
typedef struct tib_year_astro_data
{
    int rabjung; // the indian cycle the year is in (number 1 is the one starting in 1027)
    unsigned char yor; // the year inside the rabjung, starting with yor 0 for the first
    unsigned char animal; // animal, element and gender (ex: 2011 is female iron rabbit)
    unsigned char element;
    unsigned char gender;
    unsigned char sme_ba;
} tib_year_astro_data;

typedef struct tib_month_astro_data
{
    unsigned char c_month; // the chinese month
    unsigned char element;
    unsigned char gender;
    unsigned char animal;
} tib_month_astro_data;

typedef struct tib_day_astro_data
{
  unsigned char element; 
  unsigned char animal;
  unsigned char c_lunar_mansion; // the chinese lunar mansion (phugpa)
  unsigned char s_sme_ba; // the solar day's mewa
  unsigned char l_sme_ba; // the lunar day's mewa
  unsigned char trigram;
  unsigned char l_animal;
  unsigned char lm_db; // lunar mansion at daybreak
  unsigned char yoga; // yoga, see KTC p.42
  unsigned char karana; // Karanas are numbered from 0 to 7 for the changing karanas, and from 7 to
  unsigned char sideral_day[3]; // the zodiacal sign (month), day and nadi for the mean solar longitude
  unsigned char anniversary; // could be better or even thrown out of libtastro, but we'll see... currently it's just one of the values listed below.
  // 10 for the fixed ones, see KTC p.43
  // now the Earth-lords:
  unsigned char yk; // 1 if  "yan kwong" is present, 0 otherwise
  unsigned char zph; // "zin phung"
  unsigned char kbz; // "klu bzlog"
  unsigned char kth; //  "klu thebs"
  unsigned char nn; // "nyi nag", value is 2 if month is 1
} tib_day_astro_data;

typedef struct tib_year
{
    long int year; // the western calendar year
    tib_year_astro_data *astro_data;
} tib_year;

typedef struct tib_month
{
    tib_year *year;
    long int month; // the adjusted month as appearing in the almanach
    unsigned char type;
    long int true_month[2]; // the true month
    unsigned char zero_month_flag; // 0 if it is the second time we are on the 0th or 1st month, 1 if it's the first or if we are in another month // TODO: is it useful? redundant with month_type?
    long int start_gd; // the general day for tt=0 in the month
    long int rilcha[2]; // the anomaly
    long int gzadru[6]; // month mean weekday
    long int nyidru[6]; // month mean solar longitude
    //astrological data
    tib_month_astro_data *astro_data;
} tib_month;

// structure for a lunar date
typedef struct tib_day
{
  tib_month *month;
  long int gd; // the general day (spi zag), in our case it is the julian day
  long int tt; // the lunar day
  long int ommited; // a type above to know if there is an ommited day around
  long int duplicated; // idem, for duplicated
  long int nyibar[6]; // mean solar longitude
  long int nyidag[6]; // true solar longitude
  long int gzadag[6]; // true weekday
  tib_day_astro_data *astro_data;
  tib_planet_data *planet_data;
} tib_day;

tib_day * new_tib_day();
tib_month * new_tib_month();
tib_planet_data * new_tib_planet_data();
tib_year *new_tib_year();

void free_tib_year(tib_year *ty);
void free_tib_month(tib_month *tm);
void free_tib_day(tib_day *td);

tib_month_astro_data *new_tib_month_astro_data();
tib_year_astro_data *new_tib_year_astro_data();
tib_day_astro_data *new_tib_day_astro_data();

// the anniversaries
#define ANN_DEM_MIR 1 // demonstration of miracles
#define ANN_REV_KALACAKRA 2 // revelation of the Kalacakra Tantra
#define ANN_BIRTH 3 // Birth of the Buddha
#define ANN_ENL_PARI 4 // Enlightenment and Parinirvana of the Buddha
#define ANN_TURN_WHEEL 5 // Turning the Wheel of the Dharma
#define ANN_ENTRY_WOMB 6 // Entering the Womb of his mother
#define ANN_DESCENT_REALM 7 // Descent from the Realm of Gods

#endif
