#include "filters/basic_filters.h"

// Accepts int** from GrayImage directly (no conversion needed)
void ConvertAndApplyGrayGaussian(int** pixels, int width, int height, float sigma) {
  ApplyGrayGaussian(pixels, width, height, sigma);
}
