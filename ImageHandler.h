#ifndef H_IMAGEHANDLER_H
#define H_IMAGEHANDLER_H

#include "FrameGrabber.h"

class ImageHandler{
private:
  FrameGrabber* _frameGrabber;
  int* _pixelValue;
  float* _pixelAverage;
  float* _pixelStandDev;
  float* _pixelSubtraction;
  
  long _size = _width*_height;
  int _width = 640;
  int _height = 480;
  float _alpha = 0.01f;
  float _k = 2.5f;
  
  ~ImageHandler();
  void nextFrame();
public:
  ImageHandler();
};

#endif