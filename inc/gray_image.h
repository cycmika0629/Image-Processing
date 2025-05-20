#ifndef GRAY_IMAGE_H_
#define GRAY_IMAGE_H_

#include "image.h"
#include "data_loader.h"
#include <string>
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
  void Display_X_Server()override;
  void Display_ASCII()override;
  //void Display_CMD() override;
  int** get_pixels() const {return pixels;}
};

#endif