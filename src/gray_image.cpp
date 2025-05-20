#include "gray_image.h"
using namespace std;

  GrayImage::GrayImage(): Image(0, 0), pixels(nullptr){}
  GrayImage::GrayImage(int w, int h, int **p): Image(w, h), pixels(p){}
  GrayImage::~GrayImage(){
    if (pixels){
      for (int i = 0; i < width; i++){
        delete[] pixels[i];
      }
      delete[] pixels;
    }
  }
  bool GrayImage::LoadImage(string filename){
    Data_Loader loader;
    pixels = loader.Load_Gray(filename, &width, &height);
    if (pixels) return true;
    else return false;
  }
  void GrayImage::DumpImage(string filename){
    Data_Loader dumper;
    dumper.Dump_Gray(width, height, pixels, filename);
    return;
  }
  void GrayImage::Displayer_X_Server(){
    Data_Loader displayer;
    displayer.Display_Gray_X_Server(width, height, pixels);
    return;
  }
  void GrayImage::Display_ASCII(){
    Data_Loader display;
    display.Display_Gray_ASCII(width, height, pixels);
    return;
  }
  /*void GrayImage::Display_CMD();*/