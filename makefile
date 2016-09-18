all:
	g++ -std=c++11 \
  run.cpp \
  src/V4L2Wrapper.cpp src/Webcam.cpp src/ImageHandler.cpp src/Utils.cpp \
  src/BackgroundSubtraction.cpp \
  -o run -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
