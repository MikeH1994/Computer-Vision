#ifndef H_FRAMEGRABBER_H
#define H_FRAMEGRABBER_H

#include "V4L2Wrapper.h"
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <ctime>

class Webcam{
private:
  float _rWeight = 0.2126f;
  float _gWeight = 0.7152f;
  float _bWeight = 0.0722f;
  int _width;
  int _height;
  long _size;
  V4L2Wrapper* _wrapper;
  std::recursive_mutex _webcamLock;
  float getTimeDifference(std::clock_t start, std::clock_t end);
  void copyArray(unsigned char* src,  unsigned char* dst, long size);
public:
  
  Webcam(int width, int height);
  ~Webcam();
  void writeImageToFile(std::string path);
  void writeImageToFile(const  unsigned char* data,long size,std::string path);
  unsigned char* getPixelArrayFromWebcam();
  unsigned char* getPixelArrayFromWebcam(unsigned char* pixelArray);
  unsigned char* getImageFromWebcam();
  unsigned char* getImageFromWebcam( unsigned char* copyTo);
  void convertToGrayScale( unsigned char* data, long size);
};
#endif