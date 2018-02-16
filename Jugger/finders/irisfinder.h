#pragma once
#include <opencv2\opencv.hpp>
#include "hretina.h"
//#include "eyeshapefinder.h"

typedef std::tuple< double, cv::Point2d > Iris;

class IrisFinder {

public:
	IrisFinder( const cv::Mat & mat = {}, const cv::Rect & roi = {} );

	auto	getRect() const -> const cv::Rect2d;
	auto	getPoint() const -> const cv::Point2d;
	auto	getRadius() const -> const float;
	auto	getDebugMat() const -> const cv::Mat;
	auto	getTemplate() const -> const cv::Mat;
	auto	getHistoryMat() const -> const cv::Mat;

	auto	find( const cv::Mat & mat, const cv::Rect & roi ) -> Iris;

private:

	auto	getChanceMap( const cv::Size & size, const cv::Point & point = {} )->cv::Mat;
	auto	getEdgesMat( const cv::Mat & mat ) -> cv::Mat_< uchar >;
	auto	getIrisTemplate( float radius ) -> cv::Mat;

	//EyeShapeFinder	eyeShapeFinder;
	HRetina			retina;

	cv::Rect2d		rect;
	cv::Point2d		point;
	double			radius;

	cv::Mat			lastMat;

	cv::Mat			debugMat;
	cv::Mat			templateMat;
	cv::Mat			historyMat;
	std::vector< cv::Mat > history;

	std::map< float, cv::Mat > templateCache;
};
