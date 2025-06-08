#ifndef BONUS_FILTERS_H
#define BONUS_FILTERS_H
// Fisheye Filter
void ApplyGrayFisheye(int **pixels, int width, int height);
void ApplyRGBFisheye(int ***pixels, int width, int height);
// Swirl Filter
void ApplyGraySwirl(int **pixels, int width, int height, float extent);
void ApplyRGBSwirl(int ***pixels, int width, int height, float extent);
// Cartoon Effect Filter
void ApplyGrayCartoon(int **pixels, int width, int height);
void ApplyRGBCartoon(int ***pixels, int width, int height);
#endif