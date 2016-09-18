#ifndef H_IMAGEHANDLER_H
#define H_IMAGEHANDLER_H

#include"CImg.h"

#include <chrono>
#include <thread>
#include <algorithm>
#include "Webcam.h"
#include <unistd.h>

class ImageHandler:public Utils{
protected:
  bool _autoUpdate = true;
  unsigned char* _pixelValue;
  int _nFramesTaken = 1;
  int _width = 640;
  int _height = 480;
  long _size = _width*_height*3;
  float _nSecondsPerUpdate = 0.5f;
  std::string _outdir = "images";
  Webcam* _webcam;
  std::recursive_mutex _pixelLock;
  std::thread _updateThread;
  cimg_library::CImg<unsigned char> *_image;
  cimg_library::CImgDisplay *_imageDisplay;
  void update();
  void nextFrame();
  void initialise();
  //======================
  // overwritten functions
  virtual void processNextFrame() = 0;
  virtual void initialFrame() = 0;
  virtual void saveScreenGrab(std::string) = 0;  
  virtual void updateImageDisplay() = 0;
  //======================
public:
  ImageHandler();
  ~ImageHandler();
};
#endif
