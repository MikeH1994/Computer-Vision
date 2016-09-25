#include "../include/Utils.h"
#include <sys/types.h>
#include <sys/stat.h>

void Utils::writeImageToFile(const  unsigned char* data,int width, int height,std::string path){
  long size = width*height*3;
  std::ofstream image;
  image.open(path.c_str());
  image << "P6\n" << width << " " << height << " 255\n";
  image.write((char *) data, size);
  image.close();
}

float Utils::getTimeDifference(std::clock_t start, std::clock_t end){
  return ((float) end - (float) start)/(float) CLOCKS_PER_SEC;
}

void Utils::copyArray(unsigned char* src,  unsigned char* dst, long size){
  for (long i = 0; i<size; i++){
	  dst[i] = src[i];
  }
}

const std::string Utils::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%X", &tstruct);
    return buf;
}

bool Utils::directoryExists(std::string filepath){
	struct stat info;
	if(stat( filepath.c_str(), &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}


void Utils::heapSort(unsigned char* array, int arraySize) {
  int i, temp;

  for (i = (arraySize / 2)-1; i >= 0; i--)
    siftDown(array, i, arraySize);

  for (i = arraySize-1; i >= 1; i--)
  {
    temp = array[0];
    array[0] = array[i];
    array[i] = temp;
    siftDown(array, 0, i-1);
  }
}


void Utils::siftDown(unsigned char* array,int root, int bottom){
  int done, maxChild, temp;

  done = 0;
  while ((root*2 <= bottom) && (!done))
  {
    if (root*2 == bottom)
      maxChild = root * 2;
    else if (array[root * 2] > array[root * 2 + 1])
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;

    if (array[root] < array[maxChild])
    {
      temp = array[root];
      array[root] = array[maxChild];
      array[maxChild] = temp;
      root = maxChild;
    }
    else
      done = 1;
  }
}


