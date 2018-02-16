#pragma once
#include <opencv2/opencv.hpp>

class HKalmanFilter {

public:
	HKalmanFilter();
	
	auto	correct( const cv::Point2d & point ) -> cv::Point2d;

private:

	cv::KalmanFilter	filter;

};