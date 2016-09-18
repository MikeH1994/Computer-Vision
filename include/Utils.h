#ifndef H_UTILS_H
#define H_UTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Utils{
protected:
  float _rWeight = 0.2126f;
  float _gWeight = 0.7152f;
  float _bWeight = 0.0722f;
  void writeImageToFile(const  unsigned char* data,int width, int height,std::string path);
  float getTimeDifference(std::clock_t start, std::clock_t end);
  void copyArray(unsigned char* src,  unsigned char* dst, long size);
  const std::string currentDateTime();
  bool directoryExists(std::string filepath);
public:
};

#endif
