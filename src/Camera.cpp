#include"../include/Camera.h"

Camera::Camera():ImageHandler(){

}

void Camera::updateImageDisplay(){
	//updates the CImg windows displaying camera output
	int x;
	int y;
	
	for(long i = 0; i<_width*_height; i+=1){
		x = i%_width;
		y = i/_width;
		(*_image)(x,y,0,0) = _pixelValue[i*3];
		(*_image)(x,y,0,1) = _pixelValue[i*3 +1];
		(*_image)(x,y,0,2) = _pixelValue[i*3 + 2];
	}
	_image->display(*_imageDisplay);
}

void Camera::saveScreenGrab(std::string partName){
	//saves screengrab to folder
  std::string time = currentDateTime();
  std::replace(time.begin(),time.end(),':','_');
  std::replace(time.begin(),time.end(),'.','_');
  std::string path = _outdir + "/grab_" + partName + time + ".ppm";
  std::cout<<"Image written to "<<path<<std::endl;
  writeImageToFile(_pixelValue,_width,_height,path);
}

void Camera::initialFrame(){
	return;
}

void Camera::processNextFrame(){

	filter();
}

