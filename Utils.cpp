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

void Utils::convertToGrayScale( unsigned char* data, long size){
  float temp;
  int iTemp;
  for (long i = 0; i<size; i+=3){
	  temp = _rWeight*(int) data[i] + _gWeight*(int)data[i+1]+ _bWeight*(int)data[i+2];
	  iTemp = (int) temp;
	  for (int j = 0; j<3;j++){
		data[i+j] = (unsigned char) iTemp;  
	  }
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