#include "Utils.h"

void Utils::writeImageToFile(const  unsigned char* data,int width, int height,std::string path){
  long size = width*height*3;
  std::ofstream image;
  image.open(path.c_str());
  image << "P6\n" << width << " " << height << " 255\n";
  image.write((char *) data, size);
  image.close();
}

float Utils::getTimeDifference(std::clock_t start, std::clock_t end){
  return ((float) end - (float) start)/(float) CLOCKS_PER_SEC;
}

void Utils::copyArray(unsigned char* src,  unsigned char* dst, long size){
  for (long i = 0; i<size; i++){
	  dst[i] = src[i];
  }
}

const std::string Utils::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%X", &tstruct);
    return buf;
}