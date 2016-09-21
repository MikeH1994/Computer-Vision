#ifndef _H_FILTERS_H_
#define _H_FILTERS_H_

class Filter{
protected:
	int _width;
	int _height;
	long _size;
	float* _tempArray;
	bool* _indexNotOutOfBounds;
public:
	void runMeanFilter(unsigned char* array,int kernelSize = 1);
	void runMedianFilter(unsigned char* array, int kernelSize = 1);
	bool notOutOfBounds(int &x,int &y);
	long getIndex(int &x, int &y,int &nChannel);
	void generateIndexNotOutOfBoundsArray();
	Filter(int width, int height);
	~Filter();
};

#endif
