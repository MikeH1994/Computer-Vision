all:
	g++ -std=c++11 run.cpp V4L2Wrapper.cpp Webcam.cpp ImageHandler.cpp Utils.cpp -o run -lpthread