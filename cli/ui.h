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
