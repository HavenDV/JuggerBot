#pragma once
#include "asmopencv.h"

class ImageConverter {

public:

	ImageConverter( const QImage & _image );
	ImageConverter( const cv::Mat & _mat );

	operator cv::Mat() const;
	operator QImage() const;

private:

	QImage image;
	cv::Mat mat;

};