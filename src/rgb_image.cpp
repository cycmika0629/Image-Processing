#include "rgb_image.h"
#include "CImg.h"
using namespace cimg_library;

void RGBImage::DrawTextOnImage(const std::string& text, const std::string& out_filename) {
  // Convert raw int*** pixels to CImg<unsigned char>
  CImg<unsigned char> img(width, height, 1, 3);
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      for (int c = 0; c < 3; ++c)
        img(x, y, 0, c) = static_cast<unsigned char>(pixels[y][x][c]);

  // Draw text
  const unsigned char color[] = { 255, 0, 0 }; // Red
  img.draw_text(10, 10, text.c_str(), color, 0, 1.0f, 32);

  // Save as PNG
  img.save_png(out_filename.c_str());
}

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

