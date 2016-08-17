#ifndef H_FRAMEGRABBER_H
#define H_FRAMEGRABBER_H

#include "Webcam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class FrameGrabber{
private:
  std::string _extension = ".ppm";
  std::string _filename = "file_";
  float _rWeight = 0.2126f;
  float _gWeight = 0.7152f;
  float _bWeight = 0.0722f;
  int _width;
  int _height;
  long _size;
  Webcam* _webcam;
  void writeImageToFile(const  unsigned char* data,long size,std::string path);
  float getTimeDifference(std::clock_t start, std::clock_t end);
  unsigned char* getImageFromWebcam();
  unsigned char* getImageFromWebcam( unsigned char* copyTo);
  int* getPixelArrayFromWebcam();
  int* getPixelArrayFromWebcam(int* pixelArray);
  void copyArray(unsigned char* src,  unsigned char* dst, long size);
  void convertToGrayScale( unsigned char* data, long size);
  void getVideo(float nSeconds);
public:
  FrameGrabber(int width, int height);
  ~FrameGrabber();
};


#endif