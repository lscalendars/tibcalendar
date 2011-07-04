#include "translation.h"
#include<jd.h> // for MONDAY, etc.
#include<astrology.h> // for the definitions of elements, etc.

char * get_weekday(long int weekday)
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

char *
get_gender(unsigned char gender)
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

char *
get_element_5(unsigned char element)
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
get_animal(unsigned char animal)
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
