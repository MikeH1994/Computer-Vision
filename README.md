# Computer-Vision
Attemp at some fairly basic video processing & computer vision stuff on Linux.

Uses Video4Linux2 C++ wrapper from Severin Lemaignan at
https://github.com/severin-lemaignan/webcam-v4l2 (renamed to V4L2 wrapper here)
& CImg http://cimg.eu/reference/index.html

#Background Subtraction:
Background Subtraction using running Gaussian Average and mean spatial filtering

Before (w/ filter):

![Original](images/image0.jpg?raw=true "Title")

After (w/filter):

![Movement Detected](images/image2b.jpg?raw=true "Title")

Background Subtracted Image:

![Movement Foreground](images/image2a.jpg?raw=true "Title")

