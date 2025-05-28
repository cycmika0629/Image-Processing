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

using namespace std;


/*int main(int argc, char *argv[]){
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/lena.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_X_Server();
    //img1->Display_CMD();
    

    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/4-1.jpg");
    img2->DumpImage("img2.jpg");
    img2->Display_X_Server();
    //img2->Display_CMD();

    GrayImage* gimg = dynamic_cast<GrayImage*>(img1);
    if (gimg) {
        int gray_filters = FILTER_FLIP; 
        ApplyFiltersGray(gimg->get_pixels(), gimg->get_width(), gimg->get_height(), gray_filters);
        gimg->DumpImage("img1_filtered.jpg");
    }
    
    RGBImage* rimg = dynamic_cast<RGBImage*>(img2);
    if (rimg) {
        int rgb_filters = FILTER_LAPLACIAN;
        ApplyFiltersRGB(rimg->get_pixels(), rimg->get_width(), rimg->get_height(), rgb_filters);
        rimg->DumpImage("img2_laplacian.jpg");
        rimg->Display_X_Server(); 
    }
    
    delete img1;
    delete img2;
    
    return 0;
}*/


int main(/*int argc, char *argv[]*/) {
  // === ��ܹϤ������P�ɮצW�� ===
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
  cout << "Which type of image do you want?(Enter 1(Gray)/2(RGB)): ";
  cin >> img_type;
  cout << "Enter the name of the image in Image-Folder (.jpg/.png): ";
  cin >> img_name;
  img_name = "Image-Folder/" + img_name;

  Image* img = nullptr;
  switch (img_type) {
    case 1:
      img = new GrayImage();
      break;
    case 2:
      img = new RGBImage();
      break;
    default:
      cerr << "Invalid image type!" << endl;
      return 1;
  }

  if (!img->LoadImage(img_name)) {
    cerr << "Failed to load image: " << img_name << endl;
    delete img;
    return 1;
  }
  
  if (!img) {
  std::cerr << "Image pointer is null!" << std::endl;
  return 1;
  }
  img->DumpImage("img.jpg");
  img->Display_X_Server();
  
  if(img_type == 2){
    int fun;
    cout << "Choose which functions you want to use(1: Filter, 2: Encryption): ";
    cin >> fun;
    if(fun == 2){
      int num;
      cout << "Enter 1 for encryption, 2 for decryption: ";
      cin >> num;
      cin.ignore();
      RGBImage* rimg = dynamic_cast<RGBImage*>(img);
      ImageEncryption crypto;
      if(num == 1){
        string msg;
        cout << "Enter the message: ";   
        getline(cin, msg); 
        RGBImage* encrypted = crypto.Encrypt(msg, rimg);
        encrypted->DumpImage("Image-Folder/encrypted_img.png");
        encrypted->Display_X_Server();
        delete encrypted;
        delete img;
        return 0;
      }
      else if(num == 2){
        string msg = crypto.Decrypt(rimg);
        cout << "Decrypted message: " << msg << endl;
        delete img;
        return 0;
      }
    }
  }
  // === ����o������ ===
  int filter_type;
  cout << "Choose the filter number:\n"
    << "1: Flip\n2: Mosaic\n3: Gaussian\n4: Laplacian\n5: FishEye\n"
    << "6: Swirl\n7: Cartoon Effect\n8: Custom Sequence\n";
  cin >> filter_type;

  switch (filter_type) {
    case 1: // Flip
    if (img_type == 1)
      ApplyGrayHorizontalFlip(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    else
      ApplyRGBHorizontalFlip(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    break;

    case 2: {
    cout << "Enter mosaic block size (e.g., 8): ";
    int b;
    cin >> b;
    if (img_type == 1)
      ApplyGrayMosaic(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height(), b);
    else
      ApplyRGBMosaic(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height(), b);
    break;
    }

    case 3: {
    cout << "Enter Gaussian sigma (e.g., 1.2): ";
    float sigma;
    cin >> sigma;
    if (img_type == 1)
      ApplyGrayGaussian(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);
    else
      ApplyRGBGaussian(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);
    break;
    }

    case 4:
    if (img_type == 1)
      ApplyGrayLaplacian(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    else
      ApplyRGBLaplacian(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    break;

    case 5:
    if (img_type == 1)
      ApplyGrayFisheye(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    else
      ApplyRGBFisheye(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    break;

    case 6:
    if (img_type == 1)
      ApplyGraySwirl(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    else
      ApplyRGBSwirl(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    break;

    case 7:
    if (img_type == 1)
      ApplyGrayCartoon(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    else
      ApplyRGBCartoon(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
    break;

    case 8: {
      cin.ignore();
      string sequence;
      cout << "Enter filter sequence (e.g. flip|mosaic:8|gaussian:1.2): ";
      getline(cin, sequence);

      vector<string> filters;
      stringstream ss(sequence);
      string token;
      while (getline(ss, token, '|')) filters.push_back(token);

      for (const string& f : filters) {
        string name, param;
        size_t colon = f.find(':');
        if (colon != string::npos) {
          name = f.substr(0, colon);
          param = f.substr(colon + 1);
        } else {
        name = f;
        }
      if (filter_map.find(name) == filter_map.end()) {
      cerr << "Unsupported filter: " << name << endl;
      continue;
      }

      FilterType type = filter_map[name];

      if (img_type == 1) {
        GrayImage* gimg = dynamic_cast<GrayImage*>(img);
        switch (type) {
          case FILTER_FLIP:      ApplyGrayHorizontalFlip(gimg->get_pixels(), gimg->get_width(), gimg->get_height()); break;
          case FILTER_MOSAIC:    ApplyGrayMosaic(gimg->get_pixels(), gimg->get_width(), gimg->get_height(), stoi(param)); break;
          case FILTER_GAUSSIAN:  ApplyGrayGaussian(gimg->get_pixels(), gimg->get_width(), gimg->get_height(), stof(param)); break;
          case FILTER_LAPLACIAN: ApplyGrayLaplacian(gimg->get_pixels(), gimg->get_width(), gimg->get_height()); break;
          case FILTER_FISHEYE:   ApplyGrayFisheye(gimg->get_pixels(), gimg->get_width(), gimg->get_height()); break;
          case FILTER_SWIRL:     ApplyGraySwirl(gimg->get_pixels(), gimg->get_width(), gimg->get_height()); break;
          case FILTER_CARTOON:   ApplyGrayCartoon(gimg->get_pixels(), gimg->get_width(), gimg->get_height()); break;
          default: break;
        }
      } else if (img_type == 2) {
        RGBImage* rimg = dynamic_cast<RGBImage*>(img);
        switch (type) {
        case FILTER_FLIP:      ApplyRGBHorizontalFlip(rimg->get_pixels(), rimg->get_width(), rimg->get_height()); break;
        case FILTER_MOSAIC:    ApplyRGBMosaic(rimg->get_pixels(), rimg->get_width(), rimg->get_height(), stoi(param)); break;
        case FILTER_GAUSSIAN:  ApplyRGBGaussian(rimg->get_pixels(), rimg->get_width(), rimg->get_height(), stof(param)); break;
        case FILTER_LAPLACIAN: ApplyRGBLaplacian(rimg->get_pixels(), rimg->get_width(), rimg->get_height()); break;
        case FILTER_FISHEYE:   ApplyRGBFisheye(rimg->get_pixels(), rimg->get_width(), rimg->get_height()); break;
        case FILTER_SWIRL:     ApplyRGBSwirl(rimg->get_pixels(), rimg->get_width(), rimg->get_height()); break;
        case FILTER_CARTOON:   ApplyRGBCartoon(rimg->get_pixels(), rimg->get_width(), rimg->get_height()); break;
        default: break;
        }
      }
      }
      img->DumpImage("img_filtered_custom.jpg");
      img->Display_X_Server();
      delete img;
      return 0;
    }
    default:
    cerr << "Invalid filter number!" << endl;
    delete img;
    return 1;
    }

  img->DumpImage("img_filtered_custom.jpg");
  img->Display_X_Server();

  delete img;
  return 0;
}
