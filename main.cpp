#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"
#include "filters/filters.h"
#include <string>
#include <iostream>

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

#include <iostream>
#include <string>
#include "gray_image.h"
#include "rgb_image.h"
#include "filters/filters.h"
#include "bit_field_filter.h"

using namespace std;

int main(/*int argc, char *argv[]*/) {
  // === 選擇圖片類型與檔案名稱 ===
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
  // === 選擇濾鏡類型 ===
  int filter_type;
  cout << "Choose the filter number(1: Flip, 2: Mosaic, 3: Gaussian, 4: Laplacian, 5: FishEye): ";
  cin >> filter_type;

  int filter = 0;
  switch (filter_type) {
    case 1: filter = FILTER_FLIP; break;
    case 2: filter = FILTER_MOSAIC; break;
    case 3: filter = FILTER_GAUSSIAN; break;
    case 4: filter = FILTER_LAPLACIAN; break;
    case 5: filter = FILTER_FISHEYE; break;
    default:
      cerr << "Invalid filter number!" << endl;
      delete img;
      return 1;
  }

  // === 應用濾鏡 ===
  switch (img_type) {
    case 1: {
      GrayImage* gimg = dynamic_cast<GrayImage*>(img);
      if (!gimg) {
      cerr << "dynamic_cast to GrayImage failed!" << std::endl;
      break;
      }
      ApplyFiltersGray(gimg->get_pixels(), gimg->get_width(), gimg->get_height(), filter);
      gimg->DumpImage("img_filtered.jpg");
      gimg->Display_X_Server();
      break;
    }
    case 2: {
      RGBImage* rimg = dynamic_cast<RGBImage*>(img);
      if (!rimg) {
      cerr << "dynamic_cast to RGBImage failed!" << std::endl;
      break;
      }
      ApplyFiltersRGB(rimg->get_pixels(), rimg->get_width(), rimg->get_height(), filter);
      rimg->DumpImage("img_filtered.jpg");
      rimg->Display_X_Server();
      break;
    }
  }

  delete img;
  return 0;
}
