#ifndef H_IMAGEHANDLER_H
#define H_IMAGEHANDLER_H

#include <chrono>
#include <thread>
#include <algorithm>
#include "Webcam.h"

class ImageHandler:public Utils{
protected:
  bool _autoUpdate = true;
  bool _autoSave = true;
  unsigned char* _pixelValue;

  int _nFramesTaken = 1;
  int _width = 640;
  int _height = 480;
  long _size = _width*_height*3;
  float _nSecondsPerUpdate = 0.5f;
  std::string _outdir = "images/grab_";
  Webcam* _webcam;
  std::recursive_mutex _pixelLock;
  std::thread _updateThread;
  void update();
  void nextFrame();
  void initialise();
  //======================
  // overwritten functions
  virtual void processNextFrame() = 0;
  virtual void initialFrame() = 0;
  virtual void saveScreenGrab(std::string) = 0;  
  //======================
public:
  ImageHandler();
  ~ImageHandler();
};
#endif
