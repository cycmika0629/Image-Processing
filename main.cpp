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
#include <fstream>

using namespace std;

void LogFilterUse(const string& image_name, const string& filter_info, const string& output_name) {
  ofstream log("log.txt", ios::app);
  auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
  log << "[" << ctime(&now);
  log << "] Image: " << image_name << endl;
  log << "→ Applied: " << filter_info << endl;
  log << "→ Output: " << output_name << endl;
  log << "---------------------------------------" << endl;
  log.close();
}

void GaussianInteractivePreview(Image* img, int img_type, const string& original_name) {
  if (!img) return;

  string output_name = "preview_gaussian.jpg";
  while (true) {
    float sigma;
    cout << "[GAUSSIAN] Enter sigma value (0 to exit): ";
    cin >> sigma;
    if (sigma == 0) break;

    if (img_type == 1)
      ApplyGrayGaussian(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);
    else
      ApplyRGBGaussian(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);

    img->DumpImage(output_name);
    img->Display_X_Server();

    cout << "[INFO] Applied Gaussian(sigma=" << sigma << ") → " << output_name << endl;
    LogFilterUse(original_name, "Gaussian(sigma=" + to_string(sigma) + ")", output_name);
  }
}

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

  while (true) {
    string img_name;
    int img_type;
    cout << "[INPUT] Image type? (1: Gray / 2: RGB / 0: Exit): ";
    cin >> img_type;
    if (img_type == 0) break;

    cout << "[INPUT] Image file name (.jpg/.png in Image-Folder): ";
    cin >> img_name;
    string full_path = "Image-Folder/" + img_name;

    Image* img = nullptr;
    if (img_type == 1) img = new GrayImage();
    else if (img_type == 2) img = new RGBImage();
    else {
      cerr << "[ERROR] Invalid image type!\n";
      continue;
    }

    if (!img->LoadImage(full_path)) {
      cerr << "[ERROR] Failed to load image: " << full_path << endl;
      delete img;
      continue;
    }

    cout << "[INFO] Loaded: " << full_path << ", size: " << img->get_width() << "x" << img->get_height() << endl;
    img->DumpImage("img.jpg");
    img->Display_X_Server();

    // encryption option if RGB
    if (img_type == 2) {
      int choice;
      cout << "[INPUT] Do you want to use (1) Filter or (2) Encryption? ";
      cin >> choice;
      if (choice == 2) {
        RGBImage* rimg = dynamic_cast<RGBImage*>(img);
        ImageEncryption crypto;
        int op;
        cout << "[INPUT] Enter 1 to encrypt or 2 to decrypt: ";
        cin >> op;
        cin.ignore();
        if (op == 1) {
          string msg;
          cout << "[INPUT] Enter message to encrypt: ";
          getline(cin, msg);
          RGBImage* encrypted = crypto.Encrypt(msg, rimg);
          encrypted->DumpImage("Image-Folder/encrypted_img.png");
          encrypted->Display_X_Server();
          LogFilterUse(img_name, "Encrypt(message)", "encrypted_img.png");
          delete encrypted;
        } else if (op == 2) {
          string msg = crypto.Decrypt(rimg);
          cout << "[INFO] Decrypted message: " << msg << endl;
          LogFilterUse(img_name, "Decrypt(message)", "<from current image>");
        }
        delete img;
        continue;
      }
    }

    int filter_type;
    cout << "[INPUT] Choose filter:\n"
         << "1: Flip\n2: Mosaic\n3: Gaussian\n4: Laplacian\n5: FishEye\n"
         << "6: Swirl\n7: Cartoon\n8: Gaussian Interactive Preview\n9: Custom Sequence\n>> ";
    cin >> filter_type;

    string output_name = "img_filtered.jpg";
    string log_info;

    switch (filter_type) {
      case 1:
        if (img_type == 1) ApplyGrayHorizontalFlip(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        else ApplyRGBHorizontalFlip(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        log_info = "Flip";
        break;
      case 2: {
        int b; cout << "[INPUT] Mosaic block size: "; cin >> b;
        if (img_type == 1) ApplyGrayMosaic(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height(), b);
        else ApplyRGBMosaic(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height(), b);
        log_info = "Mosaic(block=" + to_string(b) + ")";
        break;
      }
      case 3: {
        float sigma; cout << "[INPUT] Gaussian sigma: "; cin >> sigma;
        if (img_type == 1) ApplyGrayGaussian(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);
        else ApplyRGBGaussian(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height(), sigma);
        log_info = "Gaussian(sigma=" + to_string(sigma) + ")";
        break;
      }
      case 4:
        if (img_type == 1) ApplyGrayLaplacian(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        else ApplyRGBLaplacian(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        log_info = "Laplacian";
        break;
      case 5:
        if (img_type == 1) ApplyGrayFisheye(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        else ApplyRGBFisheye(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        log_info = "Fisheye";
        break;
      case 6:
        if (img_type == 1) ApplyGraySwirl(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        else ApplyRGBSwirl(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        log_info = "Swirl";
        break;
      case 7:
        if (img_type == 1) ApplyGrayCartoon(dynamic_cast<GrayImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        else ApplyRGBCartoon(dynamic_cast<RGBImage*>(img)->get_pixels(), img->get_width(), img->get_height());
        log_info = "Cartoon";
        break;
      case 8:
        GaussianInteractivePreview(img, img_type, img_name);
        delete img;
        continue;
      case 9: {
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
        log_info = "Custom Sequence: " + sequence;
        break;
      }
      default:
        cerr << "[ERROR] Invalid filter number." << endl;
        delete img;
        continue;
    }

    img->DumpImage(output_name);
    img->Display_X_Server();
    cout << "[INFO] Output saved to: " << output_name << endl;
    LogFilterUse(img_name, log_info, output_name);
    delete img;
  }

  return 0;
}
