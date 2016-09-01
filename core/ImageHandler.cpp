#include "ImageHandler.h"
#include <cmath>

ImageHandler::~ImageHandler(){
  delete _webcam;
}

ImageHandler::ImageHandler(){
  _webcam = new Webcam(_width,_height);
}

void ImageHandler::update(){
  int t = (int) (1000*_nSecondsPerUpdate);
  while(_autoUpdate){
    nextFrame();
    std::this_thread::sleep_for(std::chrono::milliseconds( t ) );
  }
}

void ImageHandler::nextFrame(){
  /*
  update the _pixelValue array with new values from webcam
  */
  _pixelLock.lock();
  _pixelValue = _webcam->getArrayFromWebcam();
  processNextFrame();
  _pixelLock.unlock();
}
