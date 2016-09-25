#include"../include/Filter.h"

Filter::Filter(int width,int height):_width(width),_height(height),_size(width*height*3){
	_tempArray = new float[_size];
	_medianArray = new unsigned char[_medianArraySize];
}

Filter::~Filter(){
	delete _tempArray;
	delete _medianArray;
}

void Filter::runMedianFilter(unsigned char* array, int kernelSize){
	long index = 0;
	int x=0,y=0,dx=0,dy=0,newX=0,newY=0,channel=0,n=0;
	int area = (2*kernelSize + 1)*(2*kernelSize + 1);
	if (area!=_medianArraySize){
		resizeMedianArray(area);
	}
	for(x=kernelSize; x<_width-kernelSize; x++){
		for(y=kernelSize;y<_height-kernelSize; y++){
			index = getIndex(x,y);
			for(channel = 0; channel<3; channel++){
				n=0;
				for(dx=-kernelSize;dx<kernelSize+1;dx++){
					newX = x+dx;
					for(dy=-kernelSize;dy<kernelSize+1;dy++){
						newY = y+dy;
						_medianArray[n] = array[getIndex(newX,newY,channel)];
						n++;
					}
				}
				heapSort(_medianArray,_medianArraySize);
				_tempArray[index+channel] = getMedian();
			}
		}
	}
	for (index = 0; index<_size; index++){
		array[index] = _tempArray[index];
	}
}

void Filter::runMeanFilter(unsigned char* array, int kernelSize){
	//pass the rgb image held in array through a mean filter.
	//kernelSize here is the number of pixels to the left/right/up/down that the
	//filter looks. kernelSize = 1 looks only one to left and forms 3x3 kernel
	long index = 0;
	int x=0,y=0,dx=0,dy=0,newX=0,newY=0,channel=0,nTerms=0;
	int area = (2*kernelSize + 1)*(2*kernelSize + 1);
	float sum[3] = {0,0,0};
	for(index = 0; index<_size; index++){
		_tempArray[index] = 0;
	}
	//start the filtering at kernelSize. This removes the need to check if a
	//pixel is out of bounds and improves the computational time
	for(x=kernelSize; x<_width-kernelSize; x++){
		for(y=kernelSize;y<_height-kernelSize; y++){
			index = getIndex(x,y);
			sum[0] = 0;
			sum[1] = 0;
			sum[2] = 0;
			for(dx=-kernelSize;dx<kernelSize+1;dx++){
				newX = x+dx;
				for(dy=-kernelSize;dy<kernelSize+1;dy++){
					newY = y+dy;
					for(channel=0;channel<3;channel++){
						sum[channel]+=array[getIndex(newX,newY,channel)];
					}
				}
				for(channel = 0; channel<3; channel++){
					_tempArray[index+channel]= (unsigned char) (sum[channel]/area);
				}
			}
		}
	}
	for(index = 0; index<_size; index++){
		array[index] = _tempArray[index];
	}
}

bool Filter::notOutOfBounds(int &x,int &y){
	//checks if the point x,y is not out of bound for the image with dimensions
	//_width,_height
	if (x<0 || x>_width-1){
		return false;
	}
	if (y<0 || y>_height-1){
		return false;
	}
	return true;
}

void Filter::resizeMedianArray(int newSize){
	//create new array if the user has selected a kernel size needing a different
	//length median array
	delete _medianArray;
	_medianArray = new unsigned char[newSize];
	_medianArraySize = newSize;
}

long Filter::getIndex(int &x, int &y,int nChannel){
	 return (y*_width + x)*3 + nChannel;
}

unsigned char Filter::getMedian(){
	int index = _medianArraySize/2;
	if (_medianArraySize%2 == 1){
		return _medianArray[index];
	}
	else{
		return (_medianArray[index] + _medianArray[index+1])/2;
	}
}

void Filter::test(){
	for(int i = 0; i<_medianArraySize; i++){
		_medianArray[i] = rand()%255;
		std::cout<<(int) _medianArray[i]<<std::endl;
	}
	heapSort(_medianArray,_medianArraySize);
	std::cout<<"============="<<std::endl;
	for(int i = 0; i<_medianArraySize; i++){
		std::cout<<(int) _medianArray[i]<<std::endl;
	}
}
