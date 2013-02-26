/*
License for Tibastro - Tibetan almanach calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
char * get_yoga_str(long int yoga);
char * get_karana_str(unsigned char karana);
char * get_zodiac_str(unsigned char zodiac_sign);
char * get_zodiac_western_str(unsigned char zodiac_sign);

#endif
