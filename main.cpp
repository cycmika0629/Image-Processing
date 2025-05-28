#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"
#include "filters/filters.h"
#include "filters/bonus_filters.h"
#include "filters/basic_filters.h"
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

int main() {
  map<string, FilterType> filter_map = {
    {"flip", FILTER_FLIP},
    {"mosaic", FILTER_MOSAIC},
    {"gaussian", FILTER_GAUSSIAN},
    {"laplacian", FILTER_LAPLACIAN},
    {"fisheye", FILTER_FISHEYE},
    {"swirl", FILTER_SWIRL},
    {"cartoon", FILTER_CARTOON}
  };

  string img_name;
  int img_type;
  cout << "[INPUT] Image type? (1: Gray / 2: RGB): ";
  cin >> img_type;
  cout << "[INPUT] Image file name (.jpg/.png in Image-Folder): ";
  cin >> img_name;
  img_name = "Image-Folder/" + img_name;

  Image* img = nullptr;
  if (img_type == 1) img = new GrayImage();
  else if (img_type == 2) img = new RGBImage();
  else {
    cerr << "[ERROR] Invalid image type!\n";
    return 1;
  }

  if (!img->LoadImage(img_name)) {
    cerr << "[ERROR] Failed to load image: " << img_name << endl;
    delete img;
    return 1;
  }

  cout << "[INFO] Successfully loaded: " << img_name << endl;
  cout << "[INFO] Image type: " << (img_type == 1 ? "Gray" : "RGB") << ", Size: " << img->get_width() << "x" << img->get_height() << endl;

  img->DumpImage("img.jpg");
  img->Display_X_Server();

  if(img_type == 2){
    int fun;
    cout << "[INPUT] Choose function (1: Filter, 2: Encryption): ";
    cin >> fun;
    if(fun == 2){
      int num;
      cout << "[INPUT] 1: Encrypt, 2: Decrypt: ";
      cin >> num;
      cin.ignore();
      RGBImage* rimg = dynamic_cast<RGBImage*>(img);
      ImageEncryption crypto;
      if(num == 1){
        string msg;
        cout << "[INPUT] Enter message to encrypt: ";   
        getline(cin, msg); 
        RGBImage* encrypted = crypto.Encrypt(msg, rimg);
        encrypted->DumpImage("Image-Folder/encrypted_img.png");
        encrypted->Display_X_Server();
        cout << "[INFO] Message encrypted and image saved.\n";
        delete encrypted;
        delete img;
        return 0;
      }
      else if(num == 2){
        string msg = crypto.Decrypt(rimg);
        cout << "[INFO] Decrypted message: " << msg << endl;
        delete img;
        return 0;
      }
    }
  }

  int filter_type;
  cout << "[INPUT] Choose filter:\n"
       << "1: Flip\n2: Mosaic\n3: Gaussian\n4: Laplacian\n5: FishEye\n"
       << "6: Swirl\n7: Cartoon\n8: Custom Sequence\n>> ";
  cin >> filter_type;

  switch (filter_type) {
    case 1:
      if (img_type == 1) ApplyGrayHorizontalFlip(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      else ApplyRGBHorizontalFlip(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      break;
    case 2: {
      int b; cout << "[INPUT] Mosaic block size: "; cin >> b;
      if (img_type == 1) ApplyGrayMosaic(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height(), b);
      else ApplyRGBMosaic(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height(), b);
      break;
    }
    case 3: {
      float sigma; cout << "[INPUT] Gaussian sigma: "; cin >> sigma;
      if (img_type == 1) ApplyGrayGaussian(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);
      else ApplyRGBGaussian(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);
      break;
    }
    case 4:
      if (img_type == 1) ApplyGrayLaplacian(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      else ApplyRGBLaplacian(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      break;
    case 5:
      if (img_type == 1) ApplyGrayFisheye(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      else ApplyRGBFisheye(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      break;
    case 6:
      if (img_type == 1) ApplyGraySwirl(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      else ApplyRGBSwirl(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      break;
    case 7:
      if (img_type == 1) ApplyGrayCartoon(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      else ApplyRGBCartoon(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
      break;
    case 8: {
      cin.ignore();
      string sequence;
      cout << "[INPUT] Enter filter sequence (e.g. flip|mosaic:8|gaussian:1.2): ";
      getline(cin, sequence);

      vector<string> filters;
      stringstream ss(sequence);
      string token;
      while (getline(ss, token, '|')) {
        filters.push_back(token);
      }

      for (const string& f : filters) {
        string name, param;
        size_t colon = f.find(':');
        if (colon != string::npos) {
          name = f.substr(0, colon);
          param = f.substr(colon + 1);
        } else name = f;

        if (!filter_map.count(name)) {
          cerr << "[WARN] Unsupported filter: " << name << endl;
          continue;
        }

        FilterType type = filter_map[name];
        if (img_type == 1) {
          GrayImage* g = dynamic_cast<GrayImage*>(img);
          switch (type) {
            case FILTER_FLIP: ApplyGrayHorizontalFlip(g->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_MOSAIC: ApplyGrayMosaic(g->get_pixels(), img->get_width(), img->get_height(), stoi(param)); break;
            case FILTER_GAUSSIAN: ApplyGrayGaussian(g->get_pixels(), img->get_width(), img->get_height(), stof(param)); break;
            case FILTER_LAPLACIAN: ApplyGrayLaplacian(g->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_FISHEYE: ApplyGrayFisheye(g->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_SWIRL: ApplyGraySwirl(g->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_CARTOON: ApplyGrayCartoon(g->get_pixels(), img->get_width(), img->get_height()); break;
            default: break;
          }
        } else {
          RGBImage* r = dynamic_cast<RGBImage*>(img);
          switch (type) {
            case FILTER_FLIP: ApplyRGBHorizontalFlip(r->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_MOSAIC: ApplyRGBMosaic(r->get_pixels(), img->get_width(), img->get_height(), stoi(param)); break;
            case FILTER_GAUSSIAN: ApplyRGBGaussian(r->get_pixels(), img->get_width(), img->get_height(), stof(param)); break;
            case FILTER_LAPLACIAN: ApplyRGBLaplacian(r->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_FISHEYE: ApplyRGBFisheye(r->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_SWIRL: ApplyRGBSwirl(r->get_pixels(), img->get_width(), img->get_height()); break;
            case FILTER_CARTOON: ApplyRGBCartoon(r->get_pixels(), img->get_width(), img->get_height()); break;
            default: break;
          }
        }
      }
      break;
    }
    default:
      cerr << "[ERROR] Invalid filter number." << endl;
      delete img;
      return 1;
  }

  img->DumpImage("img_filtered_custom.jpg");
  img->Display_X_Server();

  cout << "[INFO] Filter applied and image saved to img_filtered_custom.jpg" << endl;
  auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
  cout << "[INFO] Finished at: " << ctime(&now);

  delete img;
  return 0;
}
