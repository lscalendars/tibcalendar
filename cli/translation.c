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
get_yoga_str(unsigned char yoga)
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
        return  _("fire");
        break;
      case EARTH:
        return  _("earth");
        break;
      case IRON:
        return  _("iron");
        break;
      case WATER:
        return  _("water");
        break;
      default:
        return  _("wood");
        break;
    }
}

char *
get_animal_str(unsigned char animal)
{
  switch (animal)
    {
      case RABBIT:
        return  _("rabbit");
        break;
      case DRAGON:
        return  _("dragon");
        break;
      case SNAKE:
        return  _("snake");
        break;
      case HORSE:
        return  _("horse");
        break;
      case SHEEP:
        return  _("sheep");
        break;
      case MONKEY:
        return  _("monkey");
        break;
      case BIRD:
        return  _("bird");
        break;
      case DOG:
        return  _("dog");
        break;
      case PIG:
        return  _("pig");
        break;
      case MOUSE:
        return  _("mouse");
        break;
      case OX:
        return  _("ox");
        break;
      default:
        return  _("tiger");
        break;
    }
}
