#include "image_encryption.h"
#include <string>
#include <vector>

using namespace std;

// Change string to bits
vector<bool> StringToBits(const string& msg){
  vector<bool> bits;
  int len = msg.size();
  // store string length at first 32 bit (string length can at most 2^32)
  for(int i = 31; i >= 0; --i)
    bits.push_back((len>>i)&1);
  for(char c : msg){
    for(int i = 7; i >= 0; --i){
      bits.push_back((c>>i)&1);
    }
  }
  return bits;
}

/*string BitsToString()

RGBImage* ImageEncryption::Encrypt(string message, RGBImage* img){
  int h = img->get_height();
  int w = img->get_width();
  int*** pixels = img->get_pixels();
  
  int*** p = new int** [h];
  for(int i = 0; i < h; ++i){
    p[i] = new int* [w];
    for(int j = 0; j < w; ++j){
      p[i][j] = new int[3];
      for(int c = 0; c < 3; ++c)
        p[i][j][c] =  pixels[i][j][c];
    }
  }
  
  vector<bool> bits = StringToBits(message);
  
} */