#include "tbstructures.h"
#include "system.h"
#include "stdio.h"
#include "translation.h"

void print_year_info(FILE *f, tib_year *year)
 {
    if (!year->astro_data)
      get_year_astro_data(year);
    fprintf (f, "New Year: %ld, %s-%s-%s\n", year->year,
                                  get_element_5_str(year->astro_data->element),
                                  get_gender_str(year->astro_data->gender),
                                  get_animal_str(year->astro_data->animal));
 }
 
void print_month_info(FILE *f, tib_month *month, astro_system *asys)
 {
           fprintf (f, "Tibetan Lunar Month: %ld", month->month);
  switch(month->type)
   {
   case FIRST_OF_DOUBLE:
          fprintf(f, " (Intercalary)");
          break;
    case SECOND_OF_DOUBLE: // TODO: there was a reference to epch !=2, see why
          fprintf(f, " (Delayed)");
          break;
    default:
          break;
    }
    fprintf(f, " - %s-%s-%s",
          get_element_5_str(month->astro_data->element),
          get_gender_str(month->astro_data->gender), get_animal_str(month->astro_data->animal));
 }
 
void print_day_info(FILE *f, tib_day *day, astro_system *asys)
 {
   
 }
