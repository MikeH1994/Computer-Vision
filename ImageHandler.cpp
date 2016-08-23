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
  _rgbTransferArray = new unsigned char[_size];
  _pixelAverage = new float[_size];
  _pixelStandDev = new float[_size];
  initialFrame();
}

void ImageHandler::update(){
  int t = (int) (1000*_nSecondsPerUpdate);
  while(_autoUpdate){
    nextFrame();
    std::this_thread::sleep_for(std::chrono::milliseconds( 1000 ) );
  }
}

void ImageHandler::initialFrame(){
  /*
  set the initial frame values
  */
  _pixelLock.lock();  
  _webcam->getArrayFromWebcam(_pixelValue);
  for (long i = 0; i<_size; i++){
    _pixelAverage[i] = _pixelValue[i];
    _pixelStandDev[i] = 2; 
  }
  float tempAlpha = _alpha;
  _alpha = 0.05f;
  std::cout<<"Forming background image..."<<std::endl;
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
  _webcam->getArrayFromWebcam(_pixelValue);
  float dist;
  float standDev;
  int nForeground = 0;
  for (long i = 0; i<_size; i+=3){
  	dist = 0;
  	for (int j = 0; j<3; j++){
  		//calculate new average intensity for pixel
  		_pixelAverage[i+j] = _alpha*_pixelValue[i+j] + (1-_alpha)*_pixelAverage[i+j];
  		dist+=(_pixelAverage[i+j]-_pixelValue[i+j])*(_pixelAverage[i+j]-_pixelValue[i+j]);
  	}  
  	//average distance between pixel value and pixel average for 3 channels
  	dist = sqrt(dist);
  	//calculate standard deviation (as we are taking the magnitude of the distance for all 3 channels,
  	//each one will have same standard deviation)
  	standDev = sqrt(dist*dist*_alpha + (1-_alpha)*_pixelStandDev[i]*_pixelStandDev[i]);
  	for (int j = 0; j<3; j++){
  		_pixelStandDev[i+j] = standDev;
  	}
  	  
    if (_nFramesTaken>_nInitialFramesToGenerateBackground){   
    	if (dist>_k*_pixelStandDev[i]){
    		//pixel is foreground/motion detected		
    		for (int j = 0; j<3; j++){
    			_pixelForeground[i+j] = 250;
    			nForeground++;
    		}
    	}
    	else{
    		//pixel is background/stationary	
    		for (int j = 0; j<3; j++){
    			_pixelForeground[i+j] = 0;
    		}
    	}
    }
  }
  std::cout<<"n: "<<nForeground<<"|"<<_width*_height*_foregroundCutoff<<std::endl;
  if (_nFramesTaken>_nInitialFramesToGenerateBackground && nForeground*1.0f/(_width*_height)>_foregroundCutoff){
  	std::cout<<"Motion Detected at "<<currentDateTime().c_str()<<std::endl;
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
	
	writeImageToFile(_pixelValue,_width,_height,path);
	path = _outdir + partName + time + "_foreground.ppm";
	writeImageToFile(_pixelForeground,_width,_height,path);
}


void ImageHandler::runMotionTracking(){
  _autoUpdate = true;
  _updateThread = std::thread(&ImageHandler::update,this);
  _updateThread.detach();


  std::string str = " ";
  std::cout<<"Press 'Enter' to quit"<<std::endl;
  std::getline(std::cin,str);  
  return;
  
}