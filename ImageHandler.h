#ifndef H_IMAGEHANDLER_H
#define H_IMAGEHANDLER_H

#include <chrono>
#include <thread>
#include "Webcam.h"

class ImageHandler:public Utils{
private:

  bool _autoUpdate = true;
  bool _autoSave = true;
  unsigned char* _pixelValue;
  float* _pixelAverage;
  float* _pixelStandDev;
  unsigned char* _pixelForeground;
  
  int _width = 640;
  int _height = 480;
  float _nSecondsPerUpdate = 0.5;
  int _nInitialFramesToGenerateBackground = 15;
  long _size = _width*_height;
  float _alpha = 0.01f;
  float _k = 2.5f;
  float _foregroundCutoff = 0.01f;
  std::string _outdir = "images/grab_";
  Webcam* _webcam;
  std::recursive_mutex _pixelLock;
  std::thread _updateThread;
  
  void update();
  void nextFrame();
  void initialFrame();
  void saveScreenGrab();
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