#pragma once
#include "heye.h"
#include "rect.h"
#include <opencv2\opencv.hpp>

class HScreen : public cv::Rect3d {

public:

	HScreen();
	HScreen( const cv::Mat & _mat, const cv::Rect3d & _rect );

	auto	empty() const -> const bool;
	auto	getMat() const -> const cv::Mat;

	void	setMat( const cv::Mat & _mat );

	auto	project( const HEye & eye ) const -> cv::Point3d;
	auto	to2DPoint( const HEye & eye ) const -> cv::Point2d;

private:

	cv::Mat		mat;

};