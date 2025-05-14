#ifndef IMAGE_H
#define IMAGE_H

class Image{

protected:
  int w; //Imagewidth
  int h; //Imageheight
  Data_Loader data;
  
public:
  Image(int width, int height);
  ~Image();
  virtual bool LoadImage(string filename) = 0;
  virtual void DumpImage(string filename) = 0;
  virtual void Display_X_Server() = 0;
  virtual void Display_ASCII() = 0;
  virtual void Display_CMD() = 0;
  int get_width();
  int get_height();

};
#endif