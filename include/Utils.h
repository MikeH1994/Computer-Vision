#ifndef H_UTILS_H
#define H_UTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Utils{
public:
protected:
  float _rWeight = 0.2126f;
  float _gWeight = 0.7152f;
  float _bWeight = 0.0722f;
  long _size;
  
  void writeImageToFile(const  unsigned char* data,int width, int height,std::string path);
  float getTimeDifference(std::clock_t start, std::clock_t end);
  void copyArray(unsigned char* src,  unsigned char* dst, long size);
  void heapSort(unsigned char* array, int n);
  void siftDown(unsigned char*, int root, int bottom);
  const std::string currentDateTime();
  bool directoryExists(std::string filepath);


	
};

#endif
