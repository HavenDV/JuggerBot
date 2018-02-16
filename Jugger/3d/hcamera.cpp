#include "hcamera.h"

HCamera::HCamera() {}

//_rect change to camera normal. rect fully compute
HCamera::HCamera( const cv::Mat & _mat, const cv::Point3d _position, cv::Rect3d & _rect ) : cv::Rect3d{ _rect } {
	mat = _mat;
	position = _position;
	cameraNormal = -getNormal();

	computeDistance();
}

auto	HCamera::empty() const -> const bool {
	return mat.empty();
}

auto	HCamera::getMat() const -> const cv::Mat {
	return mat;
}

auto	HCamera::getPosition() const -> const cv::Point3d {
	return position;
}

auto	HCamera::getDistance() const -> const double {
	return distance;
}

auto	HCamera::getCameraNormal() const -> const cv::Vec3d {
	return cameraNormal;
}

void	HCamera::setMat( const cv::Mat & _mat ) {
	mat = _mat;
}

void	HCamera::setPosition( const cv::Point3d & _position ) {
	position = _position;
}

void	HCamera::setDistance( double _distance ) {
	distance = _distance;

	computeRect();
}

void	HCamera::computeDistance() {
	auto vec = getCenter() - getPosition();
	distance = std::sqrt( vec.ddot( vec ) );
}

void	HCamera::computeRect() {
	//problem if change normal. no recompute;
	setCenter( position + cv::Point3d{ getCameraNormal() * distance } );
	//setNormal( -getCameraNormal() );
	//setSize( distance * { } );
}