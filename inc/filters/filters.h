#ifndef FILTERS_H
#define FILTERS_H

#include "basic_filters.h"
#include "bonus_filters.h"
#include "../bit_field_filter.h"

void ApplyGrayMosaic(int** pixels, int width, int height, int b);
void ApplyGrayGaussian(int** pixels, int width, int height, float sigma);

#endif
