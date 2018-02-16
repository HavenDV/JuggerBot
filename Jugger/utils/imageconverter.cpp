#include "imageconverter.h"

ImageConverter::ImageConverter( const QImage & _image ) {
	image = _image;
	mat = ASM::QImageToCvMat( image );
}

ImageConverter::ImageConverter( const cv::Mat & _mat ) {
	mat = _mat;
	image = ASM::cvMatToQImage( mat );
}

ImageConverter::operator cv::Mat() const {
	return mat;
}

ImageConverter::operator QImage() const {
	return image;
}