#ifndef _IMAGE_ENCRYPTION_H_
#define _IMAGE_ENCRYPTION_H_
#include "rgb_image.h"
#include <string>

using namespace std;

class ImageEncryption{
  public:
    RGBImage* Encrypt(string message, RGBImage* source_image);
    string Decrypt(RGBImage* encrypted_image);
};

#endif