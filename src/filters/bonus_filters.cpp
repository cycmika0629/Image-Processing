#include "filters/basic_filters.h"
#include <algorithm>
#include <cmath>

using namespace std;

// === ¦Ç¶¥ Fisheye ===
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

// === ±m¦â Fisheye ===
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
