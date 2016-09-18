#ifndef _H_BACKGROUNDSUBTRACTION_H_
#define _H_BACKGROUNDSUBTRACTION_H_
#include "ImageHandler.h"
#include <cmath>

class BackgroundSubtraction:public ImageHandler{
protected:
  cimg_library::CImg<unsigned char> *_imageForeground;
  cimg_library::CImgDisplay *_imageForegroundDisplay;
	bool _saveGrabs = false;
  unsigned char* _pixelForeground;
  float* _pixelAverage;
  float* _pixelStandDev;
  float _alpha = 0.005f;
  float _k = 3.5f;
  float _foregroundCutoff = 0.09f;
  int _nInitialFramesToGenerateBackground = 50;
  void processNextFrame() override;
  void initialFrame() override;
  void updateImageDisplay() override;
public: 
  void runMotionTracking();
  void saveScreenGrab(std::string) override;
  BackgroundSubtraction();
  ~BackgroundSubtraction();
  void checkPixelForMovement(float dist,long pixelIndex, long &nForeground);
  void setPixelAverageAndSD(long pixelIndex,float dist, long &nForeground);
};


#endif
