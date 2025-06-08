# 2025_NYCU_OOPFP_Image_Processing

This project implements a modular image processing system in C++ that supports grayscale and RGB images, bit-field based filters, and message encryption/decryption via LSB steganography. The system uses OOP principles, including inheritance, polymorphism, and static member sharing via a custom `Image` class hierarchy.

---

## Features

### Image Type Support
- **Grayscale Images (GrayImage class)**
  - Uses 2D `int**` pixel matrix
  - Pixel values range from 0 (black) to 255 (white)
- **RGB Images (RGBImage class)**
  - Uses 3D `int***` pixel matrix for R, G, B channels

### Image Filters (Bit-field Selectable)
> Enable multiple filters at once using bitwise OR flags.

| Filter         | Flag | Description                             |
|----------------|------|-----------------------------------------|
| Flip           | `1`  | Horizontal flip                         |
| Mosaic         | `2`  | Block pixelation                        |
| Gaussian Blur  | `4`  | Smoothing filter using Gaussian kernel |
| Laplacian      | `8`  | Edge detection / sharpening             |
| FishEye        | `16` | Radial distortion effect                |
| Swirl          | `32` | Rotational distortion from image center|
| Cartoon        | `64` | Stylized smoothing with edge emphasis   |

- Filters work on both **Gray** and **RGB** images
- Filters are modular via a `map<FilterType, function>` structure

### Image Encryption & Decryption
- **Steganographic LSB Encoding** (RGB only)
  - Embeds ASCII text into the **least significant bits** of R/G/B channels
  - Prefixes message length in first 32 bits
- **Robust Decryption**
  - Gracefully handles unencrypted images
  - Outputs a warning if no valid message is found
- **Overlay Message on Image**
  - Uses `CImg::draw_text()` to show decrypted message
  - Outputs `img_decrypted_labeled.png`

### Display Options
- **X11 GUI Display**
  - For visual image inspection on MobaXterm or Linux
- **ASCII Display in Terminal**
  - Approximates image brightness using `. - + # @`
  - Supports both Gray and RGB

### OOP Architecture
- Uses **virtual functions** and **polymorphism**
- Shared base class `Image`:
  - Common methods: `LoadImage`, `DumpImage`, `Display_X_Server`, `Display_ASCII`
- Derived classes:
  - `GrayImage` handles `int** pixels`
  - `RGBImage` handles `int*** pixels`

### Logging & File I/O
- Logs filter usage to `log.txt`
- Saves images to `Image-Folder/` as `.jpg` or `.png`

---

## Build Instructions

### Prerequisites
- g++ with C++11 support
- libX11 and libpng
- MobaXterm / Linux with X11 server
- `CImg.h` library (included)

### Compile
```bash
make              # default build
make VERBOSE=1    # show commands
make check        # run with valgrind (ASCII mode only)
