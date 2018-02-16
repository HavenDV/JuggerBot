#pragma once
#include <opencv2\opencv.hpp>
#include "finders\detector.h"

cv::Mat		findHomo( cv::Mat in, cv::Mat object, int minHessian = 400 );

class FaceMatcher {

public:
	FaceMatcher();

	static void getFeatureInfo( cv::Mat & mat, Detector &detector, std::vector<cv::KeyPoint>& keypoints, cv::Mat & descriptors, std::vector<cv::Mat>& mats );
	static void findFaceMatches( cv::Mat & mat1, cv::Mat & mat2, Detector &detector );

private:
	
};
