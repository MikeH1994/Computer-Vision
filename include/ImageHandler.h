#ifndef H_IMAGEHANDLER_H
#define H_IMAGEHANDLER_H

#include"CImg.h"
#include"Filter.h"

#include <chrono>
#include <thread>
#include <algorithm>
#include "Webcam.h"
#include <unistd.h>

class ImageHandler:public Utils{
protected:
  bool _autoUpdate = true;
  bool _applyFilter = false;
  bool _saveGrabs = true;
  unsigned char* _pixelValue;
  int _nFramesTaken = 1;
  int _width = 640;
  int _height = 480;
  long _size = _width*_height*3;
  float _nSecondsPerUpdate = 0.5f;
  const unsigned char _colorBlack[3] = {0,0,0};
  const unsigned char _colorWhite[3] = {255,255,255};
  std::string _outdir = "images";
  Webcam* _webcam;
  Filter* _filter;
  std::recursive_mutex _pixelLock;
  std::thread _updateThread;
  cimg_library::CImg<unsigned char> *_image;
  cimg_library::CImgDisplay *_imageDisplay;
  
  void update();
  void nextFrame();
  void initialise();
  void filter();
  void filter(unsigned char* array);
  bool pixelHasNonZeroNeighbour(unsigned char* array, long &index);
  bool pixelHasNonZeroNeighbour(cimg_library::CImg<unsigned char> *image, int &x, int &y);
  //======================
  // overwritten functions
  virtual void processNextFrame() = 0;
  virtual void initialFrame() = 0;
  virtual void saveScreenGrab(std::string) = 0;  
  virtual void updateImageDisplay() = 0;
  //======================
public:
	void run();
	ImageHandler();
  ~ImageHandler();
};
#endif
