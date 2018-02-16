#pragma once
#include "../hretina.h"
#include "../utils/contours.h"
#include <opencv2\opencv.hpp>

void	showTemplateForDebug( cv::Mat & mat, const cv::Rect rect, const cv::Mat & templateMat );

class EyeShapeFinder {

public:
	EyeShapeFinder( const cv::Mat & mat = {}, const cv::Rect & roi = {} );

	//Automate in first use any methods.
	//void	initialise( bool reload = false );

	//auto	isInitialised() -> const bool;

	//FIXME need rename
	auto	find( const cv::Mat & mat, const cv::Rect & roi ) -> cv::Rect;

	auto	getRect() const -> const cv::Rect2f;
	auto	getEllipse() const -> const cv::RotatedRect;
	auto	getEllipseTop() const -> const cv::RotatedRect;
	auto	getEllipseBottom() const -> const cv::RotatedRect;
	auto	getDebugMat() const -> const cv::Mat;
	auto	getTemplate() const -> const cv::Mat;

private:

	auto	generateMarkers( const cv::Size & matSize, const cv::Point & center, const cv::Size & size ) -> cv::Mat;
	auto	getContour( const cv::Mat & mat, const cv::Point & point ) -> Contour;
	auto	optimalContour( const cv::Mat & mat ) -> Contour;
	auto	contourFind( const cv::Mat & mat ) -> cv::Rect;
	auto	getTemplate( float radius )->cv::Mat;
	auto	createEllipseTemplateTop( float radius ) -> cv::Mat_<float>;
	auto	createEllipseTemplateBottom( float radius ) -> cv::Mat_<float>;
	auto	templateFind( const cv::Mat & mat, const cv::Rect & roi ) -> cv::Rect;

	auto	ellispesFind( const cv::Mat & mat, const cv::Rect & roi )->cv::Rect;

	//bool			initialised;
	HRetina			retina;

	cv::Rect2f		rect;
	cv::RotatedRect ellipse, top, bottom;

	cv::Mat			lastMat;
	cv::Mat			debugMat;
	cv::Mat			templateMat;

	std::map< float, cv::Mat > templateCache;
};
