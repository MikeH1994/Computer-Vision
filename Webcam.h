#ifndef H_FRAMEGRABBER_H
#define H_FRAMEGRABBER_H

#include "V4L2Wrapper.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>

class Webcam:public Utils{
private:
  int _width;
  int _height;
  long _size;
  V4L2Wrapper* _wrapper;
  std::recursive_mutex _webcamLock;
public:
  Webcam(int width, int height);
  ~Webcam();
  void saveImage(std::string path);
  unsigned char* getPixelArrayFromWebcam();
  unsigned char* getPixelArrayFromWebcam(unsigned char* pixelArray);
  unsigned char* getImageFromWebcam();
  unsigned char* getImageFromWebcam( unsigned char* copyTo);
};
#endif
