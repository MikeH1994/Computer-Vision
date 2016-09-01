#include "BackgroundSubtraction.h"

unsigned char BackgroundSubtraction::getForegroundValue(float dist){
  /*
	get a suitable pixel intensity for the _pixelForeground array based on
	the magnitude of pixel intensity from average (dist)
  */
  if (_variableForeground){
	return std::min((unsigned char)254,(unsigned char) dist); 
  }
  else{
	return 250;
  }
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
  long i = pixelIndex;

  if (dist>_k*_pixelStandDev[i]){
    //pixel is foreground/motion detected		
    for (int j = 0; j<3; j++){
      _pixelForeground[i+j] = getForegroundValue(dist);
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

void BackgroundSubtraction::processNextFrame(){
	/*
	called by 'nextFrame()'; using the new _pixelValue elements, generate the 
	new _pixelAverage and _pixelStandDev
	*/
  float dist;
  float standDev;
  long nForeground = 0;
  for (long i = 0; i<_size; i+=3){
  	dist = 0;
  	for (int j = 0; j<3; j++){
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
  	for (int j = 0; j<3; j++){
	  _pixelStandDev[i+j] = standDev;
  	}
  	 //check if the pixel is moving
     checkPixelForMovement(dist,i,nForeground);
   
  }
  
  if (_nFramesTaken>_nInitialFramesToGenerateBackground && 
  				nForeground*1.0f/(_width*_height)>_foregroundCutoff){
  				
  	std::cout<<"Motion Detected at "<<currentDateTime().c_str()<<std::endl;
    std::cout<<"n: "<<nForeground<<"|"<<_width*_height*_foregroundCutoff<<std::endl;
  	saveScreenGrab("image_");
  	
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
  saveScreenGrab("original_");
  _pixelLock.unlock();  
}

BackgroundSubtraction::BackgroundSubtraction():ImageHandler(){
  _pixelForeground = new unsigned char[_size];
  _pixelAverage = new float[_size];
  _pixelStandDev = new float[_size];
  initialFrame();
}

BackgroundSubtraction::~BackgroundSubtraction(){
  delete _pixelForeground;
  delete _pixelAverage;
  delete _pixelStandDev;
}

void BackgroundSubtraction::runMotionTracking(){
	/*
	runs motion tracking. Creates a thread to continously call on 'nextFrame()'
	*/
  _autoUpdate = true;
  _updateThread = std::thread(&BackgroundSubtraction::update,this);
  _updateThread.detach();

  std::string str = " ";
  std::cout<<"Press 'Enter' to quit"<<std::endl;
  std::getline(std::cin,str);
  _pixelLock.lock();
  return;
}

void BackgroundSubtraction::saveScreenGrab(std::string partName){
  std::string time = currentDateTime();
  std::replace(time.begin(),time.end(),':','_');
  std::replace(time.begin(),time.end(),'.','_');
  std::string path = _outdir + partName + time + ".ppm";
  std::cout<<"Image written to "<<path<<std::endl;
  writeImageToFile(_pixelValue,_width,_height,path);
  path = _outdir + partName + time + "_foreground.ppm";
  writeImageToFile(_pixelForeground,_width,_height,path);
}
