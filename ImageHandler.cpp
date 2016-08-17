#include "ImageHandler.h"
#include <cmath>
ImageHandler::~ImageHandler(){
  delete _frameGrabber;
}

ImageHandler::ImageHandler(){
  _frameGrabber = new FrameGrabber(_width,_height);
  _pixelValue = new int[_size];
  _pixelAverage = new float[_size];
  _pixelStandDev = new float[_size];
  _pixelSubtraction = new float[_size];
  nextFrame();
}

void ImageHandler::nextFrame(){
  _pixelValue = _frameGrabber->getPixelArrayFromWebcam(_pixelValue);
  float d;
  for (long i = 0; i<_size; i++){
    _pixelAverage[i] = _alpha*_pixelValue[i] + (1-_alpha)*_pixelAverage[i];
    d = abs(_pixelAverage[i]-pixelValue[i])
    _pixelStandDev[i] = sqrt(d*d*_alpha + (1-_alpha)*_pixelStandDev[i]*_pixelStandDev[i]);
    d = abs(_pixelValue[i] - _pixelAverage[i])/_pixelStandDev[i];
    if (d<=k){
      _pixelSubtraction[i] = 0;
    }
    else{
      _pixelSubtraction[i] = d;
    }
  }
}