#include "Webcam.h"

void Webcam::saveImage(std::string path){
  unsigned char* data = getImageFromWebcam();
  writeImageToFile(data,_width,_height,path);
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
