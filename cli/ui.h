#if defined(WIN32)
#include <conio.h>
#define ui_getchar getch
#else
#define ui_getchar() getchar()
#endif 

#define ui_print printf

#include<tbday.h>
#include<stdio.h>

void init_ui();
void end_ui();
void ui_print_lst(long int l[5], unsigned char length);
void print_tib_day(tib_day *td);
void print_tib_month(tib_month *tm);
void print_tib_day_r(tib_day *td);
void print_tib_planet_data(tib_planet_data *pd);
void print_tib_month_astro_data(tib_month_astro *tma);
void print_tib_year(tib_year *ty);

