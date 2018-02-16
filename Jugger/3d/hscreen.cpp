#include "hscreen.h"

HScreen::HScreen() {}

HScreen::HScreen( const cv::Mat & _mat, const cv::Rect3d & _rect ) : cv::Rect3d{ _rect } {
	mat = _mat;
}

auto	HScreen::empty() const -> const bool {
	return mat.empty();
}

auto	HScreen::getMat() const -> const cv::Mat {
	return mat;
}

void	HScreen::setMat( const cv::Mat & _mat ) {
	mat = _mat;
}

auto	HScreen::project( const HEye & eye ) const -> cv::Point3d {
	//eye.getEyeCenter() //Абсолютный центр глаза(не радужки). 
	//Скорей всего, середина разреза глаз. Для точнейшего проецирования.
	return cv::Rect3d::project( eye.getIrisCenter3D(), eye.getNormal() );
}

auto	HScreen::to2DPoint( const HEye & eye ) const -> cv::Point2d {
	return cv::Rect3d::to2DPoint( eye.getIrisCenter3D(), eye.getNormal() );
}
