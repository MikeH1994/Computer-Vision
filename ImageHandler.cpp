#include "ImageHandler.h"
#include <cmath>

ImageHandler::~ImageHandler(){
  delete _webcam;
}

ImageHandler::ImageHandler(){
  std::cout<<"ImageHandler constructor"<<std::endl;
  _webcam = new Webcam(_width,_height);
  _pixelValue = new unsigned char[_size];
  _pixelForeground = new unsigned char[_size];
  _rgbTransferArray = new unsigned char[_size*3];
  _pixelAverage = new float[_size];
  _pixelStandDev = new float[_size];

  initialFrame();
}

void ImageHandler::update(){
  float n = _nSecondsPerUpdate;
  while(_autoUpdate){
    //std::cout<<"Updating..."<<std::endl;
    nextFrame();
    std::this_thread::sleep_for(std::chrono::milliseconds( (int) (n*1000) ) );
  }
}

void ImageHandler::initialFrame(){
  /*
  set the initial frame values
  */
  _pixelLock.lock();
  _pixelValue = _webcam->getPixelArrayFromWebcam(_pixelValue);
  for (long i = 0; i<_size; i++){
    _pixelAverage[i] = _pixelValue[i];
    _pixelStandDev[i] = 1;//+ sqrt(_pixelValue[i]);
  }
  float tempAlpha = _alpha;
  _alpha = 0.05f;
  std::cout<<"Forming background image"<<std::endl;
  for (int i = 0; i<_nInitialFramesToGenerateBackground; i++){
    nextFrame();
  }
  _alpha = tempAlpha;
  saveScreenGrab("original_");
  _pixelLock.unlock();
}

void ImageHandler::nextFrame(){
  /*
  get pixel averages & compute running gaussian averages, then compute background subtraction
  */
  _pixelLock.lock();
  _pixelValue = _webcam->getPixelArrayFromWebcam(_pixelValue);
  float d;
  int nForeground = 0;
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
      nForeground++;
    }
  }
  if (_nFramesTaken>_nInitialFramesToGenerateBackground && nForeground*1.0f/_size>_foregroundCutoff){
	  std::cout<<"Motion Detected at "<<currentDateTime().c_str()<<std::endl;
    std::cout<<nForeground<<std::endl;
    saveScreenGrab("image_");
  }
  _nFramesTaken++;
  _pixelLock.unlock();
}

void ImageHandler::saveScreenGrab(std::string partName){
	std::string time = currentDateTime();
  std::replace(time.begin(),time.end(),':','_');
	std::replace(time.begin(),time.end(),'.','_');
  std::string path = _outdir + partName + time + ".ppm";
	
  
  writeImageToFile(convertPixelsToRGBFormat(_pixelValue),_width,_height,path);
	path = _outdir + "foreground_" + time + ".ppm";
	writeImageToFile(convertPixelsToRGBFormat(_pixelForeground),_width,_height,path);
}

unsigned char* ImageHandler::convertPixelsToRGBFormat(unsigned char* array){
  for (long i = 0; i<_size; i++){
    for (int j = 0; j<3; j++){
      _rgbTransferArray[i*3 + j] = array[i];
    }
  }
  return _rgbTransferArray;
}

unsigned char* ImageHandler::convertPixelsToRGBFormat(float* array){
  for (long i = 0; i<_size; i++){
    for (int j = 0; j<3; j++){
      _rgbTransferArray[i*3 + j] = (unsigned char) array[i];
    }
  }
  return _rgbTransferArray;
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

void ImageHandler::runMotionTracking(){
  if (_autoUpdate){
    _updateThread = std::thread(&ImageHandler::update,this);
    _updateThread.detach();
  }
  std::cout<<"alpha: "<<_alpha<<std::endl;
  std::string str = " ";
  int number = 0;
  std::string name;
  while(str!="EXIT"){
      std::cout<<"Enter 'EXIT' to quit"<<std::endl;
      std::getline(std::cin,str);  
      if (str=="EXIT"){
        _pixelLock.lock();
        return;
        _pixelLock.unlock();
      }
  }
}