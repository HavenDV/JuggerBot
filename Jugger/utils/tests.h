#pragma once
#include <opencv2\opencv.hpp>

cv::Mat		cornerDetect( cv::Mat in, int arg1, int arg2, int arg3 );
cv::Mat		furie( cv::Mat in );
cv::Mat		moments( cv::Mat in, int thresh );

