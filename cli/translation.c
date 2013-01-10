/*********************************************************************************
License for Tibastro - Tibetan astronomical and astrological calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

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

#include "translation.h"
#include<jd.h> // for MONDAY, etc.
#include<astrology.h> // for the definitions of elements, etc.

char * get_weekday_str(long int weekday)
{
  switch(weekday)
    {
      case MONDAY:
        return _("monday");
        break;
      case TUESDAY:
        return  _("tuesday");
        break;
      case WEDNESDAY:
        return  _("wednesday");
        break;
      case THURSDAY:
        return  _("thursday");
        break;
      case FRIDAY:
        return  _("friday");
        break;
      case SATURDAY:
        return  _("saturday");
        break;
      default:
        return  _("sunday");
        break;
    }
}

char * get_zodiac_western_str(unsigned char zodiac_sign)
{
  switch(zodiac_sign)
    {
      case 0:
        return  _("Aries");
        break;
      case 1:
        return  _("Taurus");
        break;
      case 2:
        return  _("Gemini");
        break;
      case 3:
        return  _("Cancer");
        break;
      case 4:
        return  _("Leo");
        break;
      case 5:
        return  _("Virgo");
        break;
      case 6:
        return  _("Libra");
        break;
      case 7:
        return  _("Scorpio");
        break;
      case 8:
        return  _("Sagittarius");
        break;
      case 9:
        return  _("Capricorn");
        break;
      case 10:
        return  _("Aquarius");
        break;
      default:
        return  _("Pisces");
        break;
}
    }

char * get_zodiac_str(unsigned char zodiac_sign)
{
  switch(zodiac_sign)
    {
      case 0:
        return  _("Caitra");
        break;
      case 1:
        return  _("Vaishâkha");
        break;
      case 2:
        return  _("Jyeshtha");
        break;
      case 3:
        return  _("Âshâdha");
        break;
      case 4:
        return  _("Shrâvana");
        break;
      case 5:
        return  _("Bhâdrapada");
        break;
      case 6:
        return  _("Âshvina");
        break;
      case 7:
        return  _("Kârtikka");
        break;
      case 8:
        return  _("Mârghashîrsha");
        break;
      case 9:
        return  _("Pausha");
        break;
      case 10:
        return  _("Mâgha");
        break;
      default:
        return  _("Phâlguna");
        break;
    }
}

char *
get_gender_str(unsigned char gender)
{
  switch (gender)
    {
      case FEMALE:
        return  _("female");
        break;
      default:
        return  _("male");
        break;
    }
}

char *yogas[27] = { "Vishkambha", "Prîti", "Ayushmat", "Saubhâgya", "Shobhana",
                  "Atiganda", "Sukarman", "Dhriti", "Shûla", "Ganda",
                  "Vriddhi", "Dhruva", "Vyâghâta", "Harshana", "Vajra",
                  "Siddhi", "Vyatipâta", "Varîyas", "Parigha", "Shiva",
                  "Siddha", "Sâdhya", "Shubha", "Shukla", "Brahman",
                  "Indra", "Vaidhriti" };

char *
get_yoga_str(long int yoga)
{
  return yogas[yoga];
}                  

static char *karanas[11] = { "Vava", "Vâlava", "Kaulava", "Taitila", "Gara", "Vanija",
                  "Vishti", "Kintughna", "Shakuni", "Catushpada", "Nâga" };
char *
get_karana_str(unsigned char karana)
{
  return karanas[karana];
}

char *
get_element_5_str(unsigned char element)
{
  switch (element)
    {
      case FIRE:
        return  _("Fire");
        break;
      case EARTH:
        return  _("Earth");
        break;
      case IRON:
        return  _("Iron");
        break;
      case WATER:
        return  _("Water");
        break;
      default: // case WOOD
        return  _("Wood");
        break;
    }
}

char *
get_element_4_str(unsigned char element)
{
  switch (element)
    {
      case FOUR_FIRE:
        return  _("Fire");
        break;
      case FOUR_EARTH:
        return  _("Earth");
        break;
      case FOUR_WIND:
        return  _("Wind");
        break;
      default: //case FOUR_WATER:
        return  _("Water");
        break;
    }
}

char *
get_animal_str(unsigned char animal)
{
  switch (animal)
    {
      case RABBIT:
        return  _("Rabbit");
        break;
      case DRAGON:
        return  _("Dragon");
        break;
      case SNAKE:
        return  _("Snake");
        break;
      case HORSE:
        return  _("Horse");
        break;
      case SHEEP:
        return  _("Sheep");
        break;
      case MONKEY:
        return  _("Monkey");
        break;
      case BIRD:
        return  _("Bird");
        break;
      case DOG:
        return  _("Dog");
        break;
      case PIG:
        return  _("Pig");
        break;
      case MOUSE:
        return  _("Mouse");
        break;
      case OX:
        return  _("Ox");
        break;
      default:
        return  _("Tiger");
        break;
    }
}

// chinese lunar mansion
static char *c_lunar_mansions[28] = { "Jiao", "Kang", "Di", "Fang", "Xin", "Wei", "Ji", "Dou",
                    "Niu", "Nu", "Xu", "Wei", "Shi", "Bi", "Kui", "Lou",
                    "Wei", "Mao", "Bi", "Zui", "Can", "Jing", "Gui", "Liu",
                    "Xing", "Zhang", "Yi", "Zhen" };

char *
get_c_lm_str(unsigned char c_lm)
{
  return c_lunar_mansions[c_lm];
}

// western month
static char *western_month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                      "Sep", "Oct", "Nov", "Dec" };

static char *trigrams[8] = { "zon", "li", "khon", "dwa", "khen", "kham", "gin", "zin"};
char *
get_trigram_str(unsigned char trigram)
{
  return trigrams[trigram];
}
