#ifndef IMAGE_H
#define IMAGE_H
#include "data_loader.h"
#include <string>

using std::string;

class Image{

protected:
  int width; //Imagewidth
  int height; //Imageheight
  Data_Loader data;
  
public:
  Image(int width, int height);
  virtual ~Image();
  virtual bool LoadImage(string filename) = 0;
  virtual void DumpImage(string filename) = 0;
  virtual void Display_X_Server() = 0;
  virtual void Display_ASCII() = 0;
  //virtual void Display_CMD() = 0;
  int get_width();
  int get_height();

};
#endif