#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"
#include "filters/filters.h"


int main(int argc, char *argv[]){
    /*Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/lena.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_X_Server();
    //img1->Display_CMD();*/
    

    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/4-1.jpg");
    img2->DumpImage("img2.jpg");
    img2->Display_X_Server();
    //img2->Display_CMD();

    /*GrayImage* gimg = dynamic_cast<GrayImage*>(img1);
    if (gimg) {
        int gray_filters = FILTER_FLIP; 
        ApplyFiltersGray(gimg->get_pixels(), gimg->get_width(), gimg->get_height(), gray_filters);
        gimg->DumpImage("img1_filtered.jpg");
    }*/
    
    RGBImage* rimg = dynamic_cast<RGBImage*>(img2);
    if (rimg) {
        int rgb_filters = FILTER_LAPLACIAN;
        ApplyFiltersRGB(rimg->get_pixels(), rimg->get_width(), rimg->get_height(), rgb_filters);
        rimg->DumpImage("img2_laplacian.jpg");
        rimg->Display_X_Server(); 
    }
    
    //delete img1;
    delete img2;
    
    return 0;
}
