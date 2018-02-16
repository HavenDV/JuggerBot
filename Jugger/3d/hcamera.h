#pragma once
#include "rect.h"
#include <opencv2\opencv.hpp>

class HCamera : public cv::Rect3d {

public:

	HCamera();
	HCamera( const cv::Mat & _mat, const cv::Point3d _position, cv::Rect3d & _rect );
	//HCamera( const cv::Mat & _mat, const cv::Point3d _position, cv::Vec3d & _normal, double angle );

	auto	empty() const -> const bool;
	auto	getMat() const -> const cv::Mat;
	auto	getPosition() const -> const cv::Point3d;
	auto	getDistance() const -> const double;
	auto	getCameraNormal() const -> const cv::Vec3d;

	void	setMat( const cv::Mat & _mat );
	void	setPosition( const cv::Point3d & _position );
	void	setDistance( double _distance );

	void	computeDistance();
	void	computeRect();

private:

	cv::Mat		mat;
	cv::Point3d	position;
	double		distance;
	cv::Vec3d	cameraNormal;

};