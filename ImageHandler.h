#ifndef H_IMAGEHANDLER_H
#define H_IMAGEHANDLER_H

#include "Webcam.h"
class ImageHandler{
private:
  unsigned char* _pixelValue;
  float* _pixelAverage;
  float* _pixelStandDev;
  float* _pixelForeground;
  
  int _width = 640;
  int _height = 480;
  long _size = _width*_height;
  float _alpha = 0.01f;
  float _k = 2.5f;
  Webcam* _webcam;
  
  void nextFrame();
  void initialFrame();
  unsigned char* convertPixelsToRGBFormat(unsigned char*);
  unsigned char* convertPixelsToRGBFormat(float*);
public:
  ImageHandler();
  ~ImageHandler();
  unsigned char getPixel(int,int);
  void setPixel(int,int,unsigned char);
  float getPixel(int,int, float*);
  void run();
};
#endif