#ifndef TRANSLATION_H
#define TRANSLATION_H

#ifndef ENABLE_NLS
#define ENABLE_NLS 0
#endif
#if ENABLE_NLS == 1
    #include "gettext.h"
    #define _(str) gettext(str)
    #define N_(str) str
    #define ngt_(str, strtwo, count) ngettext(str, strtwo, count)
#else
    #define _(str) str
    #define N_(str) str
    #define ngt_(str, strtwo, count) str
#endif

char * get_weekday_str(long int weekday);
char * get_gender_str(unsigned char gender);
char * get_element_5_str(unsigned char element);
char * get_element_4_str(unsigned char element);
char * get_animal_str(unsigned char animal);
char * get_yoga_str(unsigned char yoga);
char * get_karana_str(unsigned char karana);
char * get_zodiac_str(unsigned char zodiac_sign);
char * get_zodiac_western_str(unsigned char zodiac_sign);

#endif
