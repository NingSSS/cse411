Run as follows: 

The first step:   g++ main.cpp  -std=c++11  -lopencv_core  -lopencv_imgproc  -lopencv_imgcodecs  -lopencv_videoio  -pthread  -ltbb  -o life

The second step: ./life -idata.txt -omyvideo.avi -f5 -p100 -r20 -wNing -s1,2,3,4,5

And in my conway.fuction, I use the parallel_for to reduce the time. I don't use pipeline because it has less affection for this function. 