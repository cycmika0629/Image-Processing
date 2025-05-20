#ifndef _IMAGE_ENCRYPTION_H_
#define _IMAGE_ENCRYPTION_H_
#include <string>
#inclide "rgb_image.h"

class ImageEncryption{
  public:
    RGBImage* Encrypt(const string message, RGBImage* img);
    std::string Decrypt(RGBImage* img);
};

#endif