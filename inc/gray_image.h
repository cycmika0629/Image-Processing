#ifndef _GRAY_IMAGE_H_
#define _GRAY_IMAGE_H_

#include "image.h"
using namespace std;

class GrayImage : public Image{
private:
  int **pixels;
public:
  GrayImage();
  GrayImage(int w, int h, int **p);
  ~GrayImage();
  bool LoadImage(string filename) override;
  void DumpImage(string filename) override;
  void Displayer_X_Server() override;
  void Display_ASCII() override;
  void Display_CMD() override;
};

#endif