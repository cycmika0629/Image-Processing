#include "filters/basic_filters.h"
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

// ===  Fisheye ===
void ApplyGrayFisheye(int **pixels, int width, int height){
    int cx = width / 2;
    int cy = height / 2;
    int radius = min(cx, cy);

    int** p = new int*[height];
    for(int i = 0; i < height; ++i){
        p[i] = new int[width];
        for(int j = 0; j < width; ++j)
            p[i][j] = 0;
    }

    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            double dx = (x - cx) / static_cast<double>(radius);
            double dy = (y - cy) / static_cast<double>(radius);
            double r = sqrt(dx * dx + dy * dy);

            if(r <= 1){
                double nr = r * r;
                double theta = atan2(dy, dx);
                double nx = nr * cos(theta);
                double ny = nr * sin(theta);
                int src_x = static_cast<int>(nx * radius + cx);
                int src_y = static_cast<int>(ny * radius + cy);
                if(src_x >= 0 && src_x < width && src_y >= 0 && src_y < height)
                    p[y][x] = pixels[src_y][src_x];
            }
        }
    }

    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x)
            pixels[y][x] = p[y][x];

    for(int i = 0; i < height; ++i)
        delete[] p[i];
    delete[] p;
}

void ApplyRGBFisheye(int ***pixels, int width, int height){
    int cx = width / 2;
    int cy = height / 2;
    int radius = min(cx, cy);

    int*** p = new int**[height];
    for(int i = 0; i < height; ++i){
        p[i] = new int*[width];
        for(int j = 0; j < width; ++j){
            p[i][j] = new int[3];
            for(int c = 0; c < 3; ++c)
                p[i][j][c] = 0;
        }
    }

    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            double dx = (x - cx) / static_cast<double>(radius);
            double dy = (y - cy) / static_cast<double>(radius);
            double r = sqrt(dx * dx + dy * dy);

            if(r <= 1){
                double nr = r * r;
                double theta = atan2(dy, dx);
                double nx = nr * cos(theta);
                double ny = nr * sin(theta);
                int src_x = static_cast<int>(nx * radius + cx);
                int src_y = static_cast<int>(ny * radius + cy);
                if(src_x >= 0 && src_x < width && src_y >= 0 && src_y < height){
                    for(int c = 0; c < 3; ++c)
                        p[y][x][c] = pixels[src_y][src_x][c];
                }
            }
        }
    }

    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            for(int c = 0; c < 3; ++c)
                pixels[y][x][c] = p[y][x][c];
            delete[] p[y][x];
        }
        delete[] p[y];
    }
    delete[] p;
}

// ======== Swirl ========
void ApplyGraySwirl(int **pixels, int width, int height, double factor) {
  int cx = width / 2;
  int cy = height / 2;

  // Copy pixels safely to avoid modifying during iteration
  vector<vector<int>> temp(height, vector<int>(width, 0));

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int dx = x - cx;
      int dy = y - cy;
      double r = sqrt(dx * dx + dy * dy);
      double theta = atan2(dy, dx) + r * factor;

      int src_x = static_cast<int>(r * cos(theta) + cx);
      int src_y = static_cast<int>(r * sin(theta) + cy);

      if (src_x >= 0 && src_x < width && src_y >= 0 && src_y < height)
        temp[y][x] = pixels[src_y][src_x];
    }
  }

  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      pixels[y][x] = temp[y][x];
}


void ApplyRGBSwirl(int ***pixels, int width, int height, double factor) {
  int cx = width / 2;
  int cy = height / 2; 

  int ***p = new int**[height];
  for (int i = 0; i < height; ++i) {
    p[i] = new int*[width];
    for (int j = 0; j < width; ++j) {
      p[i][j] = new int[3];
      for (int c = 0; c < 3; ++c)
        p[i][j][c] = 0;
    }
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int dx = x - cx;
      int dy = y - cy;
      double r = std::sqrt(dx * dx + dy * dy);
      double theta = std::atan2(dy, dx) + r * factor;

      int src_x = static_cast<int>(r * std::cos(theta) + cx);
      int src_y = static_cast<int>(r * std::sin(theta) + cy);

      for (int c = 0; c < 3; ++c) {
        if (src_x >= 0 && src_x < width && src_y >= 0 && src_y < height)
          p[y][x][c] = pixels[src_y][src_x][c];
        else
          p[y][x][c] = 0;
      }
    }
  }

  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      for (int c = 0; c < 3; ++c)
        pixels[y][x][c] = p[y][x][c];

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j)
      delete[] p[i][j];
    delete[] p[i];
  }
  delete[] p;
}

// ==========Cartoon Effect=========
void ApplyGrayCartoon(int **pixels, int width, int height){
  const int level = 32; // �ⶥ 
  const int edge = 50;
  
  int** quantized = new int*[height];
  int** edge_map = new int*[height];
  for(int i = 0; i < height; ++i){
    quantized[i] = new int[width];
    edge_map[i] = new int[width];
  }
  
  for(int y = 0; y < height; ++y){
    for(int x = 0; x < width; ++x){
      quantized[y][x] = (pixels[y][x]/level) * level;
    }
  }
  
  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      int gx = 0;
      gx += abs(pixels[y][x] - pixels[y-1][x]);
      gx += abs(pixels[y][x] - pixels[y+1][x]);
      gx += abs(pixels[y][x] - pixels[y][x-1]);
      gx += abs(pixels[y][x] - pixels[y][x+1]);
      edge_map[y][x] = gx;
    }
  }
  
  for(int y = 0; y < height; ++y){
    for(int x = 0; x < width; ++x){
      if(edge_map[y][x] > edge){
        pixels[y][x] = 0;
      }
      else{
        pixels[y][x] = quantized[y][x];
      }
    }
  }
  
  for (int y = 0; y < height; ++y) {
    delete[] quantized[y];
    delete[] edge_map[y];
  }
    delete[] quantized;
    delete[] edge_map;
}

void ApplyRGBCartoon(int ***pixels, int width, int height){
  const int level = 64; // �ⶥ 
  const int edge = 100;
  
  int*** quantized = new int**[height];
  int** edge_map = new int*[height];
  for(int i = 0; i < height; ++i){
    quantized[i] = new int*[width];
    edge_map[i] = new int[width];
    for(int j = 0; j < width; ++j){
      quantized[i][j] = new int[3];
      edge_map[i][j] = 0;
    }
  }
  
  // color quantization
  for(int y = 0; y < height; ++y){
    for(int x = 0; x < width; ++x){
      for(int c = 0; c < 3; ++c)
        quantized[y][x][c] = (pixels[y][x][c]/level) * level;
    }
  }
  
  // edge detection
  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      int gx = 0;
        for (int c = 0; c < 3; ++c) {
          gx += abs(pixels[y][x][c] - pixels[y-1][x][c]);
          gx += abs(pixels[y][x][c] - pixels[y+1][x][c]);
          gx += abs(pixels[y][x][c] - pixels[y][x-1][c]);
          gx += abs(pixels[y][x][c] - pixels[y][x+1][c]);
        }
      edge_map[y][x] = gx;
    }
  }
  
  for(int y = 0; y < height; ++y){
    for(int x = 0; x < width; ++x){
      if(edge_map[y][x] > edge){
        for(int c = 0; c < 3; ++c)
          pixels[y][x][c] = 0;
      }
      else{
        for(int c = 0; c < 3; ++c)
          pixels[y][x][c] = quantized[y][x][c];
      }
    }
  }
  
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      delete[] quantized[y][x];
    }
    delete[] quantized[y];
    delete[] edge_map[y];
  }
  delete[] quantized;
  delete[] edge_map;
}
