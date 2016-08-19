#include "ImageHandler.h"
#include <cmath>

ImageHandler::~ImageHandler(){
  delete _webcam;
}

ImageHandler::ImageHandler(){
  std::cout<<"ImageHandler constructor"<<std::endl;
  _webcam = new Webcam(_width,_height);
  _pixelValue = new unsigned char[_size];
  _pixelAverage = new float[_size];
  _pixelStandDev = new float[_size];
  _pixelForeground = new float[_size];
  initialFrame();
}

void ImageHandler::initialFrame(){
  /*
  set the initial frame values
  */
  _pixelValue = _webcam->getPixelArrayFromWebcam(_pixelValue);
  for (long i = 0; i<_size; i++){
    _pixelAverage[i] = _pixelValue[i];
    _pixelStandDev[i] = 1;
  }
}

void ImageHandler::nextFrame(){
  /*
  get pixel averages & compute running gaussian averages, then compute background subtraction
  */
  _pixelValue = _webcam->getPixelArrayFromWebcam(_pixelValue);
  float d;
  for (long i = 0; i<_size; i++){
    _pixelAverage[i] = _alpha*_pixelValue[i] + (1-_alpha)*_pixelAverage[i];
    d = abs(_pixelAverage[i]-_pixelValue[i]);
    _pixelStandDev[i] = sqrt(d*d*_alpha + (1-_alpha)*_pixelStandDev[i]*_pixelStandDev[i]);
    d = abs(_pixelValue[i] - _pixelAverage[i]);
    // if pixel is within k standard deviations
    if (d<=_k*_pixelStandDev[i]){
    //pixel is background
      _pixelForeground[i] = 0;
    }
    else{
    //pixel is foreground
      _pixelForeground[i] = 250;
    }
  }
}

unsigned char* ImageHandler::convertPixelsToRGBFormat(unsigned char* array){
  unsigned char* data = new unsigned char[_size*3];
  for (long i = 0; i<_size; i++){
    for (int j = 0; j<3; j++){
      data[i*3 + j] = array[i];
    }
  }
  return data;
}

unsigned char* ImageHandler::convertPixelsToRGBFormat(float* array){
  unsigned char* data = new unsigned char[_size*3];
  for (long i = 0; i<_size; i++){
    for (int j = 0; j<3; j++){
      data[i*3 + j] = (unsigned char) array[i];
    }
  }
  return data;
}

unsigned char ImageHandler::getPixel(int x, int y){
  long index = _width*y + x;
  return _pixelValue[index];
}

void ImageHandler::setPixel(int x, int y, unsigned char value){
  _pixelValue[_width*y + x] = value;
}

float ImageHandler::getPixel(int x, int y, float* array){
  long index = _width*y + x;
  return array[index];
}

void ImageHandler::run(){
  for (int i = 0; i<100;i++){
    nextFrame();
    if (i%5==0){
      std::cout<<i<<"/100"<<std::endl;
    }
  }
  _webcam->writeImageToFile("images/before.ppm");
  unsigned char* dataAverage = convertPixelsToRGBFormat(_pixelAverage);
  std::cout<<"Press enter to take background subtraction"<<std::endl;
  std::string str;
  std::getline(std::cin,str);
  //TODO =========>std::cin?
  nextFrame();
  _webcam->writeImageToFile("images/after.ppm");
  unsigned char* dataForeground = convertPixelsToRGBFormat(_pixelForeground);
  unsigned char* dataPixelValue = convertPixelsToRGBFormat(_pixelValue);
  unsigned char* dataPixelAverage = convertPixelsToRGBFormat(_pixelAverage);
  

  _webcam->writeImageToFile(dataPixelAverage,_size*3,"images/average.ppm");
  _webcam->writeImageToFile(dataForeground,_size*3,"images/foreground.ppm");
}