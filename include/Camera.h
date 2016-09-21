#ifndef _H_CAMERA_H_
#define _H_CAMERA_H_
#include"ImageHandler.h"

class Camera:public ImageHandler{
protected:
	void processNextFrame() override;
  void initialFrame() override;
  void updateImageDisplay() override;
	void saveScreenGrab(std::string) override;
public:
	Camera();	
};

#endif
