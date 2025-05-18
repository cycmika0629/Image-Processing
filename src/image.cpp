#include "image.h"

Image::Image(int w, int h):width(w), height(h){}

Image::~Image(){}

int Image::get_width(){
  return width;
}
int Image::get_height(){
  return height;
}