all:
	g++ -std=c++11 \
  run.cpp \
  core/V4L2Wrapper.cpp core/Webcam.cpp core/ImageHandler.cpp core/Utils.cpp \
  background_subtraction/BackgroundSubtraction.cpp \
  -o run -lpthread