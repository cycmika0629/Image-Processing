#ifndef BASIC_FILTERS_H
#define BASIC_FILTERS_H

// Gray Filters
void ApplyGrayHorizontalFlip(int** pixels, int width, int height);
void ApplyGrayMosaic(int** pixels, int width, int height, int b);
void ApplyGrayGaussian(int** pixels, int width, int height, float sigma);
void ApplyGrayLaplacian(int** pixels, int width, int height);

// RGB Filters
void ApplyRGBHorizontalFlip(int*** pixels, int width, int height);
void ApplyRGBMosaic(int*** pixels, int width, int height, int b);
void ApplyRGBGaussian(int*** pixels, int width, int height, float sigma);
void ApplyRGBLaplacian(int*** pixels, int width, int height);

#endif