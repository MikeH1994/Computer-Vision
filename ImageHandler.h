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
  float _nSecondsPerUpdate = 0.5;
  int _nInitialFramesToGenerateBackground = 40;
  long _size = _width*_height;
  float _alpha = 0.01f;
  float _k = 3.2f;
  float _foregroundCutoff = 0.08f;
  std::string _outdir = "images/grab_";
  Webcam* _webcam;
  std::recursive_mutex _pixelLock;
  std::thread _updateThread;
  
  void update();
  void nextFrame();
  void initialFrame();
  void saveScreenGrab(std::string);
  unsigned char* convertPixelsToRGBFormat(unsigned char*);
  unsigned char* convertPixelsToRGBFormat(float*);
public:
  ImageHandler();
  ~ImageHandler();
  unsigned char getPixel(int,int);
  void setPixel(int,int,unsigned char);
  float getPixel(int,int, float*);
  void runMotionTracking();
};
#endif