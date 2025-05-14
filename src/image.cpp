#include <Image.h>

Image::Image(int width, int height):w(width), h(height){}

Image::~Image(){}

int Image::get_width(){
  return w;
}
int Image::get_height(){
  return h;
}