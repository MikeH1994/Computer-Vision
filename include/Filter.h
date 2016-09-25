#ifndef _H_FILTERS_H_
#define _H_FILTERS_H_
#include "Utils.h"
#include<iostream>

class Filter:Utils{
protected:
	int _width;
	int _height;
	int _medianArraySize = 9;
	long _size;
	
	float* _tempArray;
	bool* _indexNotOutOfBounds;
	unsigned char* _medianArray;
	
	long getIndex(int &x, int &y,int nChannel = 0);
	void resizeMedianArray(int newSize);
	unsigned char getMedian();
	bool notOutOfBounds(int &x,int &y);
public:
	void runMeanFilter(unsigned char* array,int kernelSize = 1);
	void runMedianFilter(unsigned char* array, int kernelSize = 1);
	void test();
	Filter(int width, int height);
	~Filter();
};

#endif
