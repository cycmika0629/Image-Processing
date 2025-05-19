#ifndef _GRAY_IMAGE_H_
#define _GRAY_IMAGE_H_

#include "image.h"
using namespace std;

class GrayImage : public Image{
private:
  int **pixels;
public:
  GrayImage();
  GrayImage(int width, int height, int **pixels);
  ~GrayImage();
  bool LoadImage(string filename);
  void DumpImage(string filename);
  void Displayer_X_Server();
  void Display_ASCII();
  void Display_CMD();
};

#endif