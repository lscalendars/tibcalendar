#ifndef SYSTEM_H
#define SYSTEM_H

// first the different systems
#define PHUGPA 1
#define TSURPHU 2
#define SHERAB_LING 3
#define ERROR_CORRECTION 4
#define MODERN_KALACAKRA 5

// the epoch structure for traditional systems
typedef struct epoch
{
    long int year; // the western calendar year
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
    long int gzad[6]; // mean weekday at new moon at the epoch: see KTC p.18
    long int nyid[6]; // mean sun of the epoch's month: see KTC p.21
} epoch;

// the system structure
typedef struct astro_system
{
    unsigned char type; // PHUGPA, TSURPHU, etc.
    epoch *epoch;
} astro_system;

astro_system *get_astro_system(unsigned char type);
astro_system *new_system();

#endif
