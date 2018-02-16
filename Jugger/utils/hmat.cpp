#include "hmat.h"

HMat::HMat( const cv::Mat & _mat ) : k{ 1.0 } {
	mat = _mat;
}

HMat::HMat( const cv::Mat & _mat, const cv::Size2d _size ) : HMat{ _mat } {
	setVirtualSize( _size );
}

HMat::HMat( const cv::Mat & _mat, double _k ) : HMat{ _mat } {
	k = _k;
}

auto	HMat::setVirtualSize( const cv::Size2d & size ) -> HMat& {
	k = size.width / mat.size().width;
	return *this;
}

HMat::operator cv::Mat() {
	return asMat< cv::Vec3b >( k );
}

HMat::operator cv::_InputArray() {
	return out = operator cv::Mat();
}