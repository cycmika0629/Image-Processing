#include "filters/basic_filters.h"
#include <algorithm>
#include <cmath>

// ========== Gray Filters =========
void ApplyGrayHorizontalFlip(int** pixels, int width, int height){
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width/2; ++j){
      std::swap(pixels[i][j], pixels[i][width-j-1]);
    }
  }
}

void ApplyGrayMosaic(int** pixels, int width, int height){
  int b = std::max(2, std::min(width, height) / 40);
  for (int i = 0; i < height; i += b) {
    for (int j = 0; j < width; j += b) {
      int sum = 0;
      int count = 0;
      for (int p = 0; p < b; ++p) {
        for (int q = 0; q < b; ++q) {
          int y = i + p;
          int x = j + q;
          if (y < height && x < width) {
            sum += pixels[y][x];
            ++count;
          }
        }
      }

      int avg = (count > 0) ? sum / count : 0;
      for (int p = 0; p < b; ++p) {
        for (int q = 0; q < b; ++q) {
          int y = i + p;
          int x = j + q;
          if (y < height && x < width) {
            pixels[y][x] = avg;
          }
        }
      }
    }
  }
}

template<typename T>
T clamp(T val, T lo, T hi) {
  return std::min(std::max(val, lo), hi);
}

void ApplyGrayGaussian(int** pixels, int width, int height){
  float sigma = 0.8;
  int k = std::ceil(3 * sigma);
  int size = 2 * k + 1;
  float **kernel = new float*[size];
  for (int i = 0; i < size; ++i){
    kernel[i] = new float[size];
  }
  float sum = 0.0;
  for (int i = -k; i <= k ; ++i){
    for (int j = -k; j <= k; ++j){
      float g = std::exp(-(i * i + j * j) / (sigma * sigma));
      g /= (2 * M_PI * sigma * sigma);
      kernel[i+k][j+k] = g;
      sum += g;
    }
  }
  for (int i = 0; i < size; ++i){
    for (int j = 0; j < size; ++j){
      kernel[i][j] /= sum;
    }
  }

  int ** output = new int*[height];
  for (int i = 0; i < height; ++i){
    output[i] = new int[width];
  }
  for (int y = 0; y < height; ++y){
    for (int x = 0; x < width; ++x){
      float val = 0.0;
      for (int i = -k; i < k; ++i){
        for (int j = -k; j < k; ++j){
          int yy = clamp((y + i), 0, height - 1);
          int xx = clamp((x + j), 0, width - 1);
          val += pixels[yy][xx] * kernel[i+k][j+k];
        }
      }
      output[y][x] = clamp((int)(val + 0.5f), 0, 255);
    }
  }

  for (int i = 0; i < height; ++i){
    for (int j = 0; j < width; ++j){
      pixels[i][j] = output[i][j];
    }
  }

  for (int i = 0; i < size; ++i){
    delete[] kernel[i];
  }
  delete[] kernel;

  for (int i = 0; i < height; ++i){
    delete[] output[i];
  }
  delete[] output;
}

void ApplyGrayLaplacian(int** pixels, int width, int height){
  // matrix of laplacian
  int K[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};
  
  // create a new matrix to store new value
  int **output = new int*[height];
  for(int i = 0; i < height; ++i)
    output[i] = new int[width];
  
  // calculate new value
  for(int i = 1; i < height-1; ++i){
    for(int j = 1; j < width-1; ++j){
      int sum = 0;
      for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y)
	        sum += K[x+1][y+1] * pixels[i+x][j+y];
      }
      sum = std::min(255,std::max(0, sum));
      output[i][j] = sum;
    }
  }

  // change the value
  for(int i = 1; i < height-1; ++i){
    for(int j = 1; j < width-1; ++j)
      pixels[i][j] = output[i][j];
  }

  // free
  for(int i = 0; i < height; ++i)
    delete[] output[i];
  delete[] output;
}

// ========== RGB Filters ==========
void ApplyRGBHorizontalFlip(int*** pixels, int width, int height){
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width/2; ++j){
      for(int c = 0; c < 3; ++c)
        std::swap(pixels[i][j][c], pixels[i][width-j-1][c]);
    }
  }
}

void ApplyRGBMosaic(int*** pixels, int width, int height){
  int b = std::max(2, std::min(width, height) / 40);
  for (int i = 0; i < height; i += b){
    for (int j = 0; j < width; j += b){
      int sum[3] = {0, 0, 0};
      int count = 0;
      for (int p = 0; p < b; ++p){
        for (int q = 0; q < b; ++q){
          int y = i + p;
          int x = j + q;
          if (x < width && y < height){
            for (int c = 0; c < 3; ++c){
              sum[c] += pixels[y][x][c];
            }
            count++;
          }
        }
      }
      int avg[3];
      if (count > 0){
        avg[0] = sum[0] / count;
        avg[1] = sum[1] / count;
        avg[2] = sum[2] / count;
      }
      else{
        avg[0] = 0;  avg[1] = 0;  avg[2] = 0;
      }
        for (int p = 0; p < b; ++p){
        for (int q = 0; q < b; ++q){
          int y = i + p;
          int x = j + q;
          for (int c = 0; c < 3; ++c){
            if (x < width && y < height){
              pixels[y][x][c] = avg[c];
            }
          }
        }
      }
    }
  }
}

void ApplyRGBGaussian(int*** pixels, int width, int height){
  float sigma = 0.8;
  int k = std::ceil(3 * sigma);
  int size = 2 * k + 1;
  float ***kernel = new float**[size];
  for (int i = 0; i < size; ++i){
    kernel[i] = new float*[size];
    for (int j = 0; j < size; ++j){
      kernel[i][j] = new float[3];
    }
  }
  float sum = 0.0;
  for (int i = -k; i <= k ; ++i){
    for (int j = -k; j <= k; ++j){
      float g = std::exp(-(i * i + j * j) / (sigma * sigma));
      g /= (2 * M_PI * sigma * sigma);
      for (int c = 0; c < 3; ++c){
        kernel[i+k][j+k][c] = g;
      }
      sum += g;
    }
  }
  for (int i = 0; i < size; ++i){
    for (int j = 0; j < size; ++j){
      for (int c = 0; c < 3; ++c){
        kernel[i][j][c] /= sum;
      }
    }
  }

  int *** output = new int**[height];
  for (int i = 0; i < height; ++i){
    output[i] = new int*[width];
    for (int j = 0; j < width; ++j){
      output[i][j] = new int[3];
    }
  }
  for (int y = 0; y < height; ++y){
    for (int x = 0; x < width; ++x){
      for (int c = 0; c < 3; ++c){
        float val = 0.0;
        for (int i = -k; i < k; ++i){
          for (int j = -k; j < k; ++j){
            int yy = clamp((y + i), 0, height - 1);
            int xx = clamp((x + j), 0, width - 1);
            val += pixels[yy][xx][c] * kernel[i+k][j+k][c];
          }
        }
        output[y][x][c] = clamp((int)(val + 0.5f), 0, 255);
      }
    }
  }

  for (int i = 0; i < height; ++i){
    for (int j = 0; j < width; ++j){
      for (int c = 0; c < 3; ++c){
        pixels[i][j][c] = output[i][j][c];
      }
    }
  }

  for (int i = 0; i < size; ++i){
    for (int j = 0; j < size; ++j){
      delete[] kernel[i][j];
    }
    delete[] kernel[i];
  }
  delete[] kernel;

  for (int i = 0; i < height; ++i){
    for (int j = 0; j < width; ++j){
      delete[] output[i][j];
    }
    delete[] output[i];
  }
  delete[] output;
}

void ApplyRGBLaplacian(int*** pixels, int width, int height){
  // matrix of laplacian
  int K[3][3] = {{0,-1,0},{-1,5,1},{0,-1,0}};
  
  // create a new matrix to store new value
  int ***output = new int**[height];
  for(int i = 0; i < height; ++i){
    output[i] = new int* [width];
    for(int j = 0; j < width; ++j)
      output[i][j] = new int[3];
  }
  
  // calculate new value
  for(int i = 1; i < height-1; ++i){
    for(int j = 1; j < width-1; ++j){
      for(int c = 0; c < 3; ++c){
        int sum = 0;
        for(int x = -1; x <= 1; ++x){
          for(int y = -1; y <= 1; ++y)
	          sum += K[x+1][y+1] * pixels[i+x][j+y][c];
        }
        sum = std::min(255,std::max(0, sum));
        output[i][j][c] = sum;
      }
    }
  }

  // change the value
  for(int i = 1; i < height-1; ++i){
    for(int j = 1; j < width-1; ++j)
      for(int c = 0; c < 3; ++c)
        pixels[i][j][c] = output[i][j][c];
  }

  // free
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j)
      delete[] output[i][j];
    delete[] output[i];
  }
  delete[] output;
}
