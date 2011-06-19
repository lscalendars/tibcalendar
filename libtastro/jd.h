/*
** jd.h
**
** Headers and function defs for julian date routines
**
*/  
  
#if !defined __JD_H
#define __JD_H

#define SATURDAY 0
#define SUNDAY 1
#define MONDAY 2
#define TUESDAY 3
#define WEDNESDAY 4
#define THURSDAY 5
#define FRIDAY 6
 
void jd_to_wd (long int jd, int *day, int *month, int *year, int *dow);

long int wd_to_jd (int day, int month, int year);

 
#endif	/* __JD_H */
