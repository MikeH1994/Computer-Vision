#include "../include/ImageHandler.h"
#include <cmath>
#include "../include/Filter.h"

ImageHandler::~ImageHandler(){
  delete _webcam;
  delete _filter;
	delete _image;
	delete _imageDisplay;
}
ImageHandler::ImageHandler(){
	if (!directoryExists(_outdir)){
		mkdir(_outdir.c_str(),0700);
	}

  _webcam = new Webcam(_width,_height);
  _filter = new Filter(_width,_height);
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

void ImageHandler::run(){
	/*
	runs motion tracking. Creates a thread to continously call on 'nextFrame()'.
	*/
  _autoUpdate = true;
  _updateThread = std::thread(&ImageHandler::update,this);
  _updateThread.detach();
  
  while(!_imageDisplay->is_closed()){
		std::this_thread::sleep_for(std::chrono::milliseconds( 100 ) );
	}
	_autoUpdate = false;
	_pixelLock.lock();
  return;
}

bool ImageHandler::pixelHasNonZeroNeighbour(unsigned char* array, long &index){
	int x = (index/3)%_width;
	int y = (index/3)/_width;
	bool up = false,down=false,left=false,right=false;
	if (x>0){
		left = true;
	}
	if (x<_width-1){
		right = true;
	}
	if (y>0){
		up = true;
	}
	if (y<_height-1){
		down = true;
	}
	
	if (up){
		if (array[index-_width*3]!=0){
			//check top
			return true;
		}
		if(left && array[index-(_width-1)*3]!=0){
			//check topleft
			return true;
		}
		if (right && array[index-(_width+1)*3]!=0){
			//check topright
			return true;
		}
	}
	if (left && array[index-3]!=0){
		//check left
		return true;
	}
	if (right && array[index+3]!=0){
		//check right
		return true;
	}
	if (down){
		if (array[index+_width*3]!=0){
			//check bottom
			return true;
		}
		if (left && array[index+(_width-1)*3]!=0){
			//check bottomleft
			return true;
		}	
		if (right && array[index+(_width+1)*3]!=0){
			//check bottomright
			return true;
		}
	}
	return false;
}

bool ImageHandler::pixelHasNonZeroNeighbour(cimg_library::CImg<unsigned char> *image, int &x, int &y){
		bool up = false,down=false,left=false,right=false;
	if (x>0){
		left = true;
	}
	if (x<_width-1){
		right = true;
	}
	if (y>0){
		up = true;
	}
	if (y<_height-1){
		down = true;
	}
	if (up){
		if ((*image)(x,y-1,0)!=0){
			//check top
			return true;
		}
		if(left && (*image)(x-1,y-1,0)!=0){
			//check topleft
			return true;
		}
		if (right && (*image)(x+1,y-1,0)!=0){
			//check topright
			return true;
		}
	}
	if (left && (*image)(x-1,y,0)!=0){
		//check left
		return true;
	}
	if (right && (*image)(x+1,y-1,0)!=0){
		//check right
		return true;
	}
	if (down){
		if ((*image)(x,y+1,0)!=0){
			//check bottom
			return true;
		}
		if (left && (*image)(x-1,y+1,0)!=0){
			//check bottomleft
			return true;
		}	
		if (right && (*image)(x+1,y+1,0)!=0){
			//check bottomright
			return true;
		}
	}
	return false;
}

void ImageHandler::nextFrame(){
  /*
  update the _pixelValue array with new values from webcam
  */
  _pixelLock.lock();
  _pixelValue = _webcam->getArrayFromWebcam();
  if (_applyFilter){
  	filter();
  }
  processNextFrame();
  updateImageDisplay();
  _pixelLock.unlock();
}

void ImageHandler::filter(){
		filter(_pixelValue);
}

void ImageHandler::filter(unsigned char* array){
	_filter->runMeanFilter(array);
}
