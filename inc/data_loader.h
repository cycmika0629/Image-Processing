#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <iostream>

using namespace std;

class Data_Loader
{

public:
    Data_Loader();
    ~Data_Loader();
    int **Load_Gray(string filename, int *w, int *h);
    int ***Load_RGB(string filename, int *w, int *h);
    void Dump_Gray(int w, int h, int **pixels, string filename);
    void Dump_RGB(int w, int h, int ***pixels, string filename);
    void Display_Gray_X_Server(int w, int h, int **pixels);
    void Display_RGB_X_Server(int w, int h, int ***pixels);
    void Display_Gray_ASCII(int w, int h, int **pixels);
    void Display_RGB_ASCII(int w, int h, int ***pixels);
    void Display_Gray_CMD(int w, int h, int **pixels);
    void Display_RGB_CMD(int w.int h, int ***pixels);
    bool List_Directory(string directoryPath, vector<string> &filesnames);

private:
    bool File_Exists(const string &filenmae);
};