#ifndef _H_BACKGROUNDSUBTRACTION_H_
#define _H_BACKGROUNDSUBTRACTION_H_
#include "../core/ImageHandler.h"

class BackgroundSubtraction:public ImageHandler{
protected:
  unsigned char* _pixelForeground;
  float* _pixelAverage;
  float* _pixelStandDev;
  float _alpha = 0.005f;
  float _k = 3.5f;
  float _foregroundCutoff = 0.09f;
  int _nInitialFramesToGenerateBackground = 30;
  void processNextFrame();
  void initialFrame();
  void saveScreenGrab(std::string); 
public:
  void runMotionTracking();
  BackgroundSubtraction();
  ~BackgroundSubtraction();
};


#endif