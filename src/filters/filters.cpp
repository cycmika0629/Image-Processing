#include "filters.h"
#include <iostream>

void ApplyFiltersGray(int** pixels, int width, int height, int filter_flags){
  if(filter_flags & FILTER_FLIP){
    ApplyGrayHorizontalFlip(pixels, width, height);
  }

  if(filter_flags & FILTER_MOSAIC){
    ApplyGrayMosaic(pixels, width, height);
  }

  if(filter_flags & FILTER_GAUSSIAN){
    ApplyGrayGaussian(pixels, width, height);
  }

  if(filter_flags & FILTER_LAPLACIAN){
    ApplyGrayLaplacian(pixels, width, height);
  }
  
  if(filter_flags & FILTER_FISHEYE){
    ApplyGrayFisheye(pixels, width, height);
  }
}

void ApplyFiltersRGB(int*** pixels, int width, int height, int filter_flags){
  if(filter_flags & FILTER_FLIP){
    ApplyRGBHorizontalFlip(pixels, width, height);
  }

  if(filter_flags & FILTER_MOSAIC){
    ApplyRGBMosaic(pixels, width, height);
  }

  if(filter_flags & FILTER_GAUSSIAN){
    ApplyRGBGaussian(pixels, width, height);
  }

  if(filter_flags & FILTER_LAPLACIAN){
    ApplyRGBLaplacian(pixels, width, height);
  }
  
  if(filter_flags & FILTER_FISHEYE){
    ApplyRGBFisheye(pixels, width, height);
  }
}
