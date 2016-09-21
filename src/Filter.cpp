#include"../include/Filter.h"

Filter::Filter(int width,int height):_width(width),_height(height),_size(width*height*3){
	_tempArray = new float[_size];
	_indexNotOutOfBounds = new bool[_size];
	generateIndexNotOutOfBoundsArray();
}

Filter::~Filter(){
	delete _tempArray;
	delete _indexNotOutOfBounds;
}

void Filter::runMedianFilter(unsigned char* array, int kernelSize){
	//TODO

}

void Filter::runMeanFilter(unsigned char* array, int kernelSize){
	long index = 0;
	int x=0,y=0,dx=0,dy=0,newX=0,newY=0,channel=0,nTerms=0;
	
	float sum[3] = {0,0,0};
	for(index = 0; index<_size; index++){
		_tempArray[index] = 0;
	}
	for(index = 0; index<_size; index+=3){
		x = (index/3)%_width;
		y = index/3/_width;
		
		nTerms=0;
		sum[0] = 0;
		sum[1] = 0;
		sum[2] = 0;
		for(dx=-kernelSize;dx<kernelSize+1;dx++){
			for(dy=-kernelSize;dy<kernelSize+1;dy++){
				newX = x+dx;
				newY = y+dy;
				if (_indexNotOutOfBounds[index]){
					
					nTerms++;
					for(channel=0;channel<3;channel++){
						sum[channel]+=array[getIndex(newX,newY,channel)];
					}
				}
			}
			for(channel = 0; channel<3; channel++){
				_tempArray[index+channel]= (unsigned char) (sum[channel]/nTerms);
			}
		}
	}
	for(index = 0; index<_size; index++){
		array[index] = _tempArray[index];
	}
}

bool Filter::notOutOfBounds(int &x,int &y){
	if (x<0){
		return false;
	}
	if (x>_width-1){
		return false;
	}
	if (y<0){
		return false;
	}
	if (y>_height-1){
		return false;
	}
	return true;
}

long Filter::getIndex(int &x, int &y,int &nChannel){
	 return (y*_width + x)*3 + nChannel;
}

void Filter::generateIndexNotOutOfBoundsArray(){
	int x,y;
	for (long index = 0; index<_size; index++){
		x = (index/3)%_width;
		y = index/3/_width;
		_indexNotOutOfBounds[index] = notOutOfBounds(x,y);
	}
}


