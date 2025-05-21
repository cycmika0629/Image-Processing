#ifndef BIT_FIELD_FILTER_H
#define BIT_FIELD_FILTER_H

enum FilterType{
  FILTER_NONE = 0,
  FILTER_FLIP = 1 << 0,      // 0001
  FILTER_MOSAIC = 1 << 1,    // 0010
  FILTER_GAUSSIAN = 1 << 2,  // 0100
  FILTER_LAPLACIAN = 1 << 3, // 1000
  // mabye for bonus filter
  FILTER_FISHEYE = 1 << 4,
};

#endif