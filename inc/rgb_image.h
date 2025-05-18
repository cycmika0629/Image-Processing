#ifndef RGB_IMAGE_H_
#define RGB_IMAGE_H_

#include "image.h"
#include "data_loader.h"
#include <string>

class RGBImage : public Image{
private:
  int*** pixels;

public:
  RGBImage();
  RGBImage(int width, int height, int ***pixels);
  ~RGBImage();
 
  bool LoadImage(std::string filename) override;
  void DumpImage(std::string filename) override;
  
  void Display_X_Server() override;
  void Display_ASCII() override;
  //void Display_CMD() override;

  int*** get_pixels() const {return pixels;}
};

#endif
