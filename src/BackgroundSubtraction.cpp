#include "../include/BackgroundSubtraction.h"

BackgroundSubtraction::BackgroundSubtraction():ImageHandler(){
  _pixelForeground = new unsigned char[_size];
  _pixelAverage = new float[_size];
  _pixelStandDev = new float[_size];
  _imageForeground = new cimg_library::CImg<unsigned char>(_width,_height,1,1,0);
    _imageForegroundDisplay = new cimg_library::CImgDisplay(*_imageForeground,
     "Cam_001 Motion");
  _imageForegroundDisplay->show();
  initialFrame();
}

BackgroundSubtraction::~BackgroundSubtraction(){
  delete _pixelForeground;
  delete _pixelAverage;
  delete _pixelStandDev;
	delete _imageForeground;
  delete _imageForegroundDisplay;
}

void BackgroundSubtraction::runMotionTracking(){
	/*
	runs motion tracking. Creates a thread to continously call on 'nextFrame()'
	*/
  _autoUpdate = true;
  _updateThread = std::thread(&BackgroundSubtraction::update,this);
  _updateThread.detach();
  
  while(!_imageDisplay->is_closed() && !_imageForegroundDisplay->is_closed()){
		std::this_thread::sleep_for(std::chrono::milliseconds( 100 ) );
	}
	_pixelLock.lock();
  return;
}

void BackgroundSubtraction::updateImageDisplay(){
	//updates the CImg windows displaying camera output
	int x;
	int y;
	for(long i = 0; i<_width*_height; i+=1){
		x = i%_width;
		y = i/_width;
		(*_image)(x,y,0,0) = _pixelValue[i*3];
		(*_image)(x,y,0,1) = _pixelValue[i*3 +1];
		(*_image)(x,y,0,2) = _pixelValue[i*3 + 2];
		(*_imageForeground)(x,y,0,0) = _pixelForeground[i*3];
	}
	_image->display(*_imageDisplay);
	_imageForeground->display(*_imageForegroundDisplay);
}

void BackgroundSubtraction::saveScreenGrab(std::string partName){
	//saves screengrab to folder
  std::string time = currentDateTime();
  std::replace(time.begin(),time.end(),':','_');
  std::replace(time.begin(),time.end(),'.','_');
  std::string path = _outdir + "/grab_" + partName + time + ".ppm";
  std::cout<<"Image written to "<<path<<std::endl;
  writeImageToFile(_pixelValue,_width,_height,path);
  path = _outdir + partName + time + "_foreground.ppm";
  writeImageToFile(_pixelForeground,_width,_height,path);
}

void BackgroundSubtraction::checkPixelForMovement(float dist, long pixelIndex,long &nForeground){ 
  /*
  for a given pixel, check if movement has been detected, if so set the value of
  corresponding element in _pixelForeground 
  */
  if (_nFramesTaken<_nInitialFramesToGenerateBackground){
    //if the background image is still being generated, don't bother with foreground pixels
    return;
  }

  if (dist>_k*_pixelStandDev[pixelIndex]){
    //pixel is foreground/motion detected		
    for (int j = 0; j<3; j++){
      _pixelForeground[pixelIndex+j] = 250;
      nForeground++;
    }
  }
  else{
    //pixel is background/stationary	
    for (int j = 0; j<3; j++){
      _pixelForeground[pixelIndex+j] = 0;
    }
  }
}

void BackgroundSubtraction::processNextFrame(){
	/*
	called by 'nextFrame()'; using the new _pixelValue elements, generate the 
	new _pixelAverage and _pixelStandDev
	*/
  float dist, standDev;
  long nForeground = 0;
  int j = 0;
  for (long i = 0; i<_size; i+=3){
  	dist = 0;
  	for (j = 0; j<3; j++){
  		//calculate new average intensity for pixel
  		_pixelAverage[i+j] = _alpha*_pixelValue[i+j] + (1-_alpha)*_pixelAverage[i+j];
  		dist+=(_pixelAverage[i+j]-_pixelValue[i+j])*(_pixelAverage[i+j]-_pixelValue[i+j]);
  	}  
  	//average distance between pixel value and pixel average for 3 channels
  	dist = sqrt(dist);
  	/*
  	calculate standard deviation (as we are taking the magnitude of the 
  	distance for all 3 channels,	each channel will have same standard deviation)
  	*/
  	standDev = sqrt(dist*dist*_alpha + (1-_alpha)*_pixelStandDev[i]*_pixelStandDev[i]);
  	for (j = 0; j<3; j++){
	  _pixelStandDev[i+j] = standDev;
  	}
  	 //check if the pixel is moving
     checkPixelForMovement(dist,i,nForeground);
  }
  
  if (_nFramesTaken>_nInitialFramesToGenerateBackground && 
  				nForeground*1.0f/(_width*_height)>_foregroundCutoff){
  				
  	std::cout<<"Motion Detected at "<<currentDateTime().c_str()<<std::endl;
  	if (_saveGrabs){
			saveScreenGrab("image_");
		}
  }
  _nFramesTaken++;
}


void BackgroundSubtraction::initialFrame(){
  /*
  called when instance is created. Sets default values for arrays and
  forms background image
  */
  _pixelLock.lock();  
  _pixelValue = _webcam->getArrayFromWebcam();
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
  if (_saveGrabs){
  	saveScreenGrab("original_");
  }
  std::cout<<"Background image complete"<<std::endl;
  _pixelLock.unlock();
}
