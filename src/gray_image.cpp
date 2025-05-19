  GrayImage();
  GrayImage(int width, int height, int **pixels);
  ~GrayImage();
  bool LoadImage(string filename);
  void DumpImage(string filename);
  void Displayer_X_Server();
  void Display_ASCII();
  void Display_CMD();