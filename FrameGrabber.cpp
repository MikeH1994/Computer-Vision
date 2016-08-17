#include "FrameGrabber.h"

void FrameGrabber::writeImageToFile(const  unsigned char* data,long size,std::string path){
  std::ofstream image;
  image.open(path.c_str());
  image << "P6\n" << _width << " " << _height << " 255\n";
  image.write((char *) data, size);
  image.close();
}
float FrameGrabber::getTimeDifference(std::clock_t start, std::clock_t end){
  return ((float) end - (float) start)/(float) CLOCKS_PER_SEC;
}

unsigned char* FrameGrabber::getImageFromWebcam(){
  return _webcam->frame().data;
}

unsigned char* FrameGrabber::getImageFromWebcam( unsigned char* copyTo){
  copyArray(_webcam->frame().data,copyTo,_size);
  return copyTo;
}

int* FrameGrabber::getPixelArrayFromWebcam(){
  int* pixelArray = new int[_width*_height];
  unsigned char* data = _webcam->frame().data;
  float temp;
  for (int i = 0; i<_width*_height;i++){
    temp = _rWeight*data[i*3] + _gWeight*data[i*3 + 1]+ _bWeight*data[i*3 + 2];
    pixelArray[i] = (int) temp;
  }
  return pixelArray;    
}

int* FrameGrabber::getPixelArrayFromWebcam(int* pixelArray){
  unsigned char* data = _webcam->frame().data;
  float temp;
  for (int i = 0; i<_width*_height;i++){
    temp = _rWeight*data[i*3] + _gWeight*data[i*3 + 1]+ _bWeight*data[i*3 + 2];
    pixelArray[i] = (int) temp;
  }
  return pixelArray;
}

void FrameGrabber::copyArray(unsigned char* src,  unsigned char* dst, long size){
  for (long i = 0; i<size; i++){
	  dst[i] = src[i];
  }
}

void FrameGrabber::convertToGrayScale( unsigned char* data, long size){
  float temp;
  int iTemp;
  for (long i = 0; i<size; i+=3){
	  temp = _rWeight*(int) data[i] + _gWeight*(int)data[i+1]+ _bWeight*(int)data[i+2];
	  iTemp = (int) temp;
	  for (int j = 0; j<3;j++){
		data[i+j] = (unsigned char) iTemp;  
	  }
  }	  
}

void FrameGrabber::getVideo(float nSeconds){
  std::clock_t start = std::clock();
  float runTime = 0;
  std::clock_t next;
  unsigned char* data;
  std::string timeGrab;
  std::string filename;
  while(runTime<nSeconds){
	  next = std::clock();
	  data = getImageFromWebcam();
    convertToGrayScale(data,_size);
	  runTime = getTimeDifference(start,next);
    //std::cout<<"Start: " << (float) start<<", End: "<<(float) next<<std::endl;
	  timeGrab = std::to_string(runTime);
	  filename = _filename + timeGrab + _extension;
	  writeImageToFile(data,_size,filename);
  }
   /*unsigned char* data = getImageFromWebcam();
   writeImageToFile(data,_size,_filename + _extension);
   */
}

FrameGrabber::FrameGrabber(int width, int height){
  int _width = width;
  int _height = height;
  long _size = _width*_height*3;
  _webcam = new Webcam("/dev/video0", _width, _height);
  std::cout<<"Dimensions: "<<_width<<", "<<_height<<std::endl;
}

FrameGrabber::~FrameGrabber(){
  delete _webcam;
}
