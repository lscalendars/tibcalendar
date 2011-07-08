#include "translation.h"
#include<jd.h> // for MONDAY, etc.
#include<astrology.h> // for the definitions of elements, etc.

char * get_weekday_str(long int weekday)
{
  switch(weekday)
    {
      case MONDAY:
        return "monday";
        break;
      case TUESDAY:
        return "tuesday";
        break;
      case WEDNESDAY:
        return "wednesday";
        break;
      case THURSDAY:
        return "thursday";
        break;
      case FRIDAY:
        return "friday";
        break;
      case SATURDAY:
        return "saturday";
        break;
      default:
        return "sunday";
        break;
    }
}

char * get_zodiac_western_str(unsigned char zodiac_sign)
{
  switch(zodiac_sign)
    {
      case 0:
        return "Aries";
        break;
      case 1:
        return "Taurus";
        break;
      case 2:
        return "Gemini";
        break;
      case 3:
        return "Cancer";
        break;
      case 4:
        return "Leo";
        break;
      case 5:
        return "Virgo";
        break;
      case 6:
        return "Libra";
        break;
      case 7:
        return "Scorpio";
        break;
      case 8:
        return "Sagittarius";
        break;
      case 9:
        return "Capricorn";
        break;
      case 10:
        return "Aquarius";
        break;
      default:
        return "Pisces";
        break;
}
    }



char * get_zodiac_str(unsigned char zodiac_sign)
{
  switch(zodiac_sign)
    {
      case 0:
        return "Caitra";
        break;
      case 1:
        return "Vaishâkha";
        break;
      case 2:
        return "Jyeshtha";
        break;
      case 3:
        return "Âshâdha";
        break;
      case 4:
        return "Shrâvana";
        break;
      case 5:
        return "Bhâdrapada";
        break;
      case 6:
        return "Âshvina";
        break;
      case 7:
        return "Kârtikka";
        break;
      case 8:
        return "Mârghashîrsha";
        break;
      case 9:
        return "Pausha";
        break;
      case 10:
        return "Mâgha";
        break;
      default:
        return "Phâlguna";
        break;
    }
}

char *
get_gender_str(unsigned char gender)
{
  switch (gender)
    {
      case FEMALE:
        return "female";
        break;
      default:
        return "male";
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
        return "fire";
        break;
      case EARTH:
        return "earth";
        break;
      case IRON:
        return "iron";
        break;
      case WATER:
        return "water";
        break;
      default:
        return "wood";
        break;
    }
}

char *
get_animal_str(unsigned char animal)
{
  switch (animal)
    {
      case RABBIT:
        return "rabbit";
        break;
      case DRAGON:
        return "dragon";
        break;
      case SNAKE:
        return "snake";
        break;
      case HORSE:
        return "horse";
        break;
      case SHEEP:
        return "sheep";
        break;
      case MONKEY:
        return "monkey";
        break;
      case BIRD:
        return "bird";
        break;
      case DOG:
        return "dog";
        break;
      case PIG:
        return "pig";
        break;
      case MOUSE:
        return "mouse";
        break;
      case OX:
        return "ox";
        break;
      default:
        return "tiger";
        break;
    }
}
