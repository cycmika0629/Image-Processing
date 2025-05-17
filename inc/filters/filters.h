#ifndef FILTERS_H
#define FILTERS_H

#include "basic_filters.h"
#include "bonus_filters.h"
#include "../bit_field_filter.h"

void ApplyFiltersGray(int** pixels, int width, int height, int filter_flags);
void ApplyFiltersRGB(int*** pixels, int width, int height, int filter_flags);

#endif
