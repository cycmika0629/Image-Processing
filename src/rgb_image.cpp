#include "rgb_image.h"

RGBImage:: RGBImage() : Image(0,0), pixels(nullptr){}

RGBImage:: RGBImage(int w, int h, int*** p) : Image(w,h), pixels(p){}

RGBImage:: ~RGBImage(){
  if(pixels){
    for(int i = 0; i < height; ++i){
      for(int j = 0; j < width; ++j)
        delete[] pixels[i][j];
      delete[] pixels[i];
    }
    delete[] pixels;
  }
}

bool RGBImage::LoadImage(std::string filename){
  Data_Loader loader;
  pixels = loader.Load_RGB(filename, &width, &height);
  if(!pixels) return false;
  return true;
}

void RGBImage::DumpImage(std::string filename){
  Data_Loader loader;
  loader.Dump_RGB(width, height, pixels, filename);
}

void RGBImage::Display_X_Server(){
  Data_Loader loader;
  loader.Display_RGB_X_Server(width, height, pixels);
}

void RGBImage::Display_ASCII(){
  Data_Loader loader;
  loader.Display_RGB_ASCII(width, height, pixels);
}

/*void RGBImage::Display_CMD(){
  Data_Loader loader;
  loader.Display_RGB_CMD(width, height, pixels);
}*/

