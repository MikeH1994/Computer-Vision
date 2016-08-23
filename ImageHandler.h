#ifndef H_IMAGEHANDLER_H
#define H_IMAGEHANDLER_H

#include <chrono>
#include <thread>
#include <algorithm>
#include "Webcam.h"

class ImageHandler:public Utils{
private:

  bool _autoUpdate = true;
  bool _autoSave = true;
  unsigned char* _pixelValue;
  unsigned char* _pixelForeground;
  unsigned char* _rgbTransferArray;
  float* _pixelAverage;
  float* _pixelStandDev;

  int _nFramesTaken = 1;
  int _width = 640;
  int _height = 480;
  int _nInitialFramesToGenerateBackground = 30;
  float _nSecondsPerUpdate = 1.0f;
  long _size = _width*_height*3;
  float _alpha = 0.01f;
  float _k = 3.0f;
  float _foregroundCutoff = 0.02f;
  std::string _outdir = "images/grab_";
  Webcam* _webcam;
  std::recursive_mutex _pixelLock;
  std::thread _updateThread;
  
  void update();
  void nextFrame();
  void initialFrame();
  void saveScreenGrab(std::string);
public:
  ImageHandler();
  ~ImageHandler();
  void runMotionTracking();
};
#endif