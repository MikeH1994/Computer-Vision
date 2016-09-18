#include "../include/ImageHandler.h"
#include <cmath>

ImageHandler::~ImageHandler(){
  delete _webcam;
	delete _image;
	delete _imageDisplay;
}

ImageHandler::ImageHandler(){

	if (!directoryExists(_outdir)){
		mkdir(_outdir.c_str());
	}

  _webcam = new Webcam(_width,_height);
  _image = new cimg_library::CImg<unsigned char>(_width,_height,1,3,0);
  _imageDisplay = new cimg_library::CImgDisplay(*_image,"Cam_001");
  _imageDisplay->show();
}

void ImageHandler::update(){
	/*
	run by _updateThread instance. 
	*/
  while(_autoUpdate){
    nextFrame();
  }
}

void ImageHandler::nextFrame(){
  /*
  update the _pixelValue array with new values from webcam
  */
  _pixelLock.lock();
  _pixelValue = _webcam->getArrayFromWebcam();
  processNextFrame();
  updateImageDisplay();
  _pixelLock.unlock();
}
