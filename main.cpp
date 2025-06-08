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
#include <functional>

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

int main() {
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

    int display_mode;
    cout << "[INPUT] Choose display mode: 1: GUI (X_Server) / 2: ASCII Terminal Display: ";
    cin >> display_mode;

    if (display_mode == 1) {
      img->Display_X_Server();
      LogFilterUse(img_name, "Initial GUI display", "img.jpg");
    } else if (display_mode == 2) {
      img->Display_ASCII();
      LogFilterUse(img_name, "Initial ASCII display", "img.jpg");
    }

    int filter_flags;
    cout << "[INPUT] Enter filter flags as sum (e.g., Flip(1)+Gaussian(4)=5):\n"
         << "1: Flip | 2: Mosaic | 4: Gaussian | 8: Laplacian | 16: FishEye | 32: Swirl | 64: Cartoon\n>> ";
    cin >> filter_flags;

    string output_name = "img_filtered.jpg";
    string log_info = "BitField Filters: ";

    map<FilterType, function<void()>> filter_handlers;

    if (img_type == 1) {
      auto g = dynamic_cast<GrayImage*>(img);
      filter_handlers = {
        {FILTER_FLIP,      [&]() { ApplyGrayHorizontalFlip(g->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_MOSAIC,    [&]() {
          int b; cout << "  [Mosaic] block size? "; cin >> b;
          ApplyGrayMosaic(g->get_pixels(), img->get_width(), img->get_height(), b);
        }},
        {FILTER_GAUSSIAN,  [&]() {
          float s; cout << "  [Gaussian] sigma? "; cin >> s;
          ApplyGrayGaussian(g->get_pixels(), img->get_width(), img->get_height(), s);

        }},
        {FILTER_LAPLACIAN, [&]() { ApplyGrayLaplacian(g->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_FISHEYE,   [&]() { ApplyGrayFisheye(g->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_SWIRL,     [&]() { ApplyGraySwirl(g->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_CARTOON,   [&]() { ApplyGrayCartoon(g->get_pixels(), img->get_width(), img->get_height()); }}
      };
    } else {
      auto r = dynamic_cast<RGBImage*>(img);
      filter_handlers = {
        {FILTER_FLIP,      [&]() { ApplyRGBHorizontalFlip(r->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_MOSAIC,    [&]() {
          int b; cout << "  [Mosaic] block size? "; cin >> b;
          ApplyRGBMosaic(r->get_pixels(), img->get_width(), img->get_height(), b);
        }},
        {FILTER_GAUSSIAN,  [&]() {
          float s; cout << "  [Gaussian] sigma? "; cin >> s;
          ApplyRGBGaussian(r->get_pixels(), img->get_width(), img->get_height(), s);
        }},
        {FILTER_LAPLACIAN, [&]() { ApplyRGBLaplacian(r->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_FISHEYE,   [&]() { ApplyRGBFisheye(r->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_SWIRL,     [&]() { ApplyRGBSwirl(r->get_pixels(), img->get_width(), img->get_height()); }},
        {FILTER_CARTOON,   [&]() { ApplyRGBCartoon(r->get_pixels(), img->get_width(), img->get_height()); }}
      };
    }

    for (auto& [flag, func] : filter_handlers) {
      if (filter_flags & flag) {
        func();
        switch (flag) {
          case FILTER_FLIP:      log_info += "Flip "; break;
          case FILTER_MOSAIC:    log_info += "Mosaic "; break;
          case FILTER_GAUSSIAN:  log_info += "Gaussian "; break;
          case FILTER_LAPLACIAN: log_info += "Laplacian "; break;
          case FILTER_FISHEYE:   log_info += "Fisheye "; break;
          case FILTER_SWIRL:     log_info += "Swirl "; break;
          case FILTER_CARTOON:   log_info += "Cartoon "; break;
          default: break;
        }
      }
    }

    img->DumpImage(output_name);
    if (display_mode == 1) img->Display_X_Server();
    else if (display_mode == 2) img->Display_ASCII();
    cout << "[INFO] Output saved to: " << output_name << endl;
    LogFilterUse(img_name, log_info, output_name);
    delete img;
  }

  return 0;
}
