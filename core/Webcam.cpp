#include "Webcam.h"

void Webcam::saveImage(std::string path){
  unsigned char* data = getArrayFromWebcam();
  writeImageToFile(data,_width,_height,path);
}

unsigned char* Webcam::getArrayFromWebcam(){
  _webcamLock.lock();
  unsigned char* data = _wrapper->frame().data;
  _webcamLock.unlock();
  return data;
}

unsigned char* Webcam::getArrayFromWebcam( unsigned char* copyTo){
  copyArray(getArrayFromWebcam(),copyTo,_size);
  return copyTo;
}

unsigned char* Webcam::getGrayscaleArrayFromWebcam(unsigned char* pixelArray){
  unsigned char* data = getArrayFromWebcam();
  float temp;
  for (long i = 0; i<_size;i+=3){
    temp = _rWeight*data[i] + _gWeight*data[i + 1]+ _bWeight*data[i + 2];
    pixelArray[i] = (unsigned char) temp;
  }
  return pixelArray;
}

Webcam::Webcam(int width, int height){
  _width = width;
  _height = height;
  _size = _width*_height*3;
  _wrapper = new V4L2Wrapper("/dev/video0", _width, _height);
}

Webcam::~Webcam(){
  delete _wrapper;
}
