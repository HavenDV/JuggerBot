#pragma once
#include <opencv2\opencv.hpp>
//#define GPU

bool		isCuda();
bool		isOpenCL();

void		initCuda();

cv::Mat		filterGPU( cv::Mat in );
int			getOptimalAffineAngleGPU( cv::Mat in1, cv::Mat in2, int start_angle, int end_angle, int thresh );