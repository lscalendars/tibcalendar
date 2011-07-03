#ifndef TBDAY_H
#define TBDAY_H

typedef struct _epoch
{
    long int year; // the western calendar year
    long int jd;   // the julian day corresponding to the epoch
    long int eyr_a;  // Intercalation index
    long int ril_a;
    long int ril_b;
    long int spz_c;
    long int spz_b;
    long int spz_f;
    long int spz_j;
    long int rahupart; // 230 fraction for Rahu cycle
    long int dragkadd; // This is added for "drag po'i rkang 'dzin".
    long int meradd; // Figures to be added for planets for the
    long int venadd; // nyin zhag dal ba. These are days of cycle.
    long int maradd;
    long int jupadd;
    long int satadd;
    long int zlasho; // index of the intercalary months, see KTC p.51. we always consider that the second index is the first + 1, and this long int is the first
    int zlapure;
    int tsurlug;
    long int gzad[6]; // mean weekday at new moon at the epoch: see KTC p.18
    long int nyid[6]; // mean sun of the epoch's month: see KTC p.21
} epoch;

static epoch phugpa_epch= // we take -1000 as a reference
{
    -1000,
    0, // TODO
    7,
    6,
    61,
    10,
    26,
    5,
    1355847,
    93,
    6663418,
    2080,
    277,
    4,
    511,
    2995,
    48,
    0,
    0,
    { 5, 35, 36, 4, 160, 0 },
    { 26, 45, 53, 4, 26, 0 },
};
  
// the different types of day (ommited part):
#define NORMAL 0 // no omited nor duplicated
#define PREVIOUS_OMMITED 2 // previous lunar day is ommited
#define NEXT_OMMITED 3 // next lunar day is ommited
// other types of day (duplicated part)
#define FIRST_OF_DUPLICATED 4 // first of a duplicated day
#define SECOND_OF_DUPLICATED 5 // second of a duplicated day

typedef struct tib_month
{
    long int year; // the western calendar year
    long int month; // the non-adjusted month
    long int adjusted_month; // the adjusted month as appearing in the almanach
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
  // should the planets be separate?
  // the lists are 6 numbers long for intermediate calculations
    long int marmurdag[6]; // the longitude of mars for the day
    long int jupmurdag[6]; // jupiter
    long int satmurdag[6]; // saturn
    long int mermurdag[6]; // mercury
    long int venmurdag[6]; // venus
    long int rahudong[5]; // rahu
} tib_day;

void rilchafp (const epoch epch, long int zd0, long int rilcha[2]);
void nyi_drup (epoch epch, long int zd0, long int nyidru[6] );
void gza_drup (epoch epch, long int zd0, long int gzadru[6]);
void nyi_lonp ( long int ld, long int nyilon[6]);
void tse_drup ( long int ld, long int tsedru[6]);
void nyi_dagp_and_gza_dagp ( long int nyibar[6], long int tsebar[6], long int rilcha[2], long int tt, long int nyidag[6], long int gzadag[6]) ;
long int spi_zagfp (const epoch epch, long int cur_mth, long int tt, long int sd);
int get_tt_data(epoch epch, long int cur_mth, long int gzadru[6], long int nyidru[6], long int rilcha[6], long int tt, long int nyidag[6], long int gzadag[6], long int nyibar[6]);
void get_day_data(long int jd, tib_day *td);
tib_day * new_tib_day();
void get_month_data (const epoch epch, long int zd0, long int rilcha[2], long int nyidru[6], long int gzadru[6]);
void free_tib_day(tib_day *td);

#endif
