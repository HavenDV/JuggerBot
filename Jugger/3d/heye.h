#pragma once
#include "hcamera.h"
#include <opencv2\opencv.hpp>

auto	points2Plane( const std::vector< cv::Point3d > & points ) -> cv::Vec4d;
auto	toSpherePoint( const cv::Point2d & point, const cv::Point2d & center ) -> cv::Point3d;
auto	toSpherePoint( const cv::Point2d & point, const cv::Point2d & center, const cv::Vec3d & normal ) -> cv::Point3d;

class HEye {

public:
	HEye();
	HEye( const cv::Rect2d & _rect, const cv::Point2d & _center, double _radius, const HCamera & camera );

	auto	empty() const -> const bool;
	auto	getMat() const -> const cv::Mat;
	auto	getCenter() const -> const cv::Point3d;
	auto	getIrisCenter() const -> const cv::Point2d;
	auto	getRadius() const -> const double;
	auto	getPlane() const -> const cv::Vec4d;
	auto	getNormal() const -> const cv::Vec3d;
	auto	getRectNormal() const -> const cv::Vec3d;
	auto	getIrisCenter3D() const -> const cv::Point3d;
	auto	getPixelSize() const -> const double;
	auto	getSize() const -> const cv::Size2d;
	auto	getEyeRadius() const -> const double;
	auto	getRect() const -> const cv::Rect3d;

	auto	computeCenter3D( const HCamera & camera ) -> cv::Point3d;
	auto	computeEyePosition( const cv::Point2d & point, const HCamera & camera ) -> cv::Point3d;
	auto	computePlane( const cv::Point2d center, double radius, const cv::Vec3d & normal ) -> cv::Vec4d;
	auto	computePlaneNormal( const cv::Vec4d & plane ) -> cv::Vec3d;

private:

	cv::Mat		mat;
	cv::Point3d	center;
	cv::Point2d	irisCenter;
	double		radius;
	cv::Vec4d	plane;
	cv::Vec3d	normal, rectNormal;
	cv::Point3d	irisCenter3D;
	double		pixelSize;
	cv::Size2d	size;
	double		eyeRadius;

};