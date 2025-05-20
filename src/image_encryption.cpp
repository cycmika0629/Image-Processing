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

string BitsToString(const vector<bool> bits){
  int len = 0;
  for(int i = 0; i < 32; ++i){
    len = (len<<1) | bits[i];
  }
  string msg = "";
  for(int i = 0; i < len; ++i){
    char c = 0;
    for(int j = 0; j < 8; ++j){
      c = (c<<1) | bits[32 + i*8 + j];
    }
    msg += c;
  }
  return msg;
}

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
  size_t bit_index = 0;
  
  for(int i = 0; i < h && bit_index < bits.size(); ++i){
    for(int j = 0; j < w && bit_index < bits.size(); ++j){
      for(int c = 0; c < 3 && bit_index < bits.size(); ++c){
        p[i][j][c] &= 0xFE; // remove LSB
        p[i][j][c] |= bits[bit_index++]; // write bit into LSB
      }
    }
  }
  
  if (bit_index < bits.size())
    throw runtime_error("Message too long to fit in image.");
    
  return new RGBImage(w,h,p);
}

string ImageEncryption::Decrypt(RGBImage* img){
  vector<bool> bits;
  int*** p = img->get_pixels();
  int h = img->get_height();
  int w = img->get_width();
  
  for(int i = 0; i < h; ++i){
    for(int j = 0; j < w; ++j){
      for(int c = 0; c < 3; ++c)
        bits.push_back(p[i][j][c] & 1);
    }
  }
  
  return BitsToString(bits);
}

 