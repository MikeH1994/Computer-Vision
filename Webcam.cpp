#include "Webcam.h"

void Webcam::writeImageToFile(const  unsigned char* data,long size,std::string path){
  std::ofstream image;
  image.open(path.c_str());
  image << "P6\n" << _width << " " << _height << " 255\n";
  image.write((char *) data, size);
  image.close();
}

void Webcam::writeImageToFile(std::string path){
  std::cout<<"Size: "<<_size<<std::endl;
  std::ofstream image;
  image.open(path.c_str());
  image << "P6\n" << _width << " " << _height << " 255\n";
  image.write((char *) getImageFromWebcam(), _size);
  image.close();
}

float Webcam::getTimeDifference(std::clock_t start, std::clock_t end){
  return ((float) end - (float) start)/(float) CLOCKS_PER_SEC;
}

unsigned char* Webcam::getImageFromWebcam(){
  _webcamLock.lock();
  return _wrapper->frame().data;
  _webcamLock.unlock();
  
}

unsigned char* Webcam::getImageFromWebcam( unsigned char* copyTo){
  copyArray(getImageFromWebcam(),copyTo,_size);
  return copyTo;
}

unsigned char* Webcam::getPixelArrayFromWebcam(){
  unsigned char* pixelArray = new unsigned char[_width*_height];
  return getPixelArrayFromWebcam(pixelArray);
}

unsigned char* Webcam::getPixelArrayFromWebcam(unsigned char* pixelArray){
  unsigned char* data = _wrapper->frame().data;
  float temp;
  for (long i = 0; i<_width*_height;i++){
    temp = _rWeight*data[i*3] + _gWeight*data[i*3 + 1]+ _bWeight*data[i*3 + 2];
    pixelArray[i] = (unsigned char) temp;
  }
  return pixelArray;
}

void Webcam::copyArray(unsigned char* src,  unsigned char* dst, long size){
  for (long i = 0; i<size; i++){
	  dst[i] = src[i];
  }
}

void Webcam::convertToGrayScale( unsigned char* data, long size){
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

Webcam::Webcam(int width, int height){
  _width = width;
  _height = height;
  _size = _width*_height*3;
  _wrapper = new V4L2Wrapper("/dev/video0", _width, _height);
  std::cout<<"Dimensions: "<<_width<<", "<<_height<<" Size: "<<_size<<std::endl;
}

Webcam::~Webcam(){
  delete _wrapper;
}
