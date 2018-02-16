#pragma once
#include <opencv2\opencv.hpp>

class Detector {

public:
	Detector();

	static void		loadCascade( cv::CascadeClassifier & cascade, const std::string & path );

	//Automate in first use any methods.
	void	initialise( bool reload = false );

	auto	isInitialised() -> const bool;
	auto	isDetected() -> const bool;

	auto	detectSmiles( const cv::Mat & grayMat, const cv::Rect & roi = {} ) -> const std::vector< cv::Rect >;
	auto	detectFaces( const cv::Mat & grayMat, const cv::Rect & roi = {} ) -> const std::vector< cv::Rect >;
	auto	detectEyes( const cv::Mat & grayMat, const cv::Rect & roi = {} ) -> const std::vector< cv::Rect >;

	auto	getEyeRect() -> const cv::Rect;
	auto	getEyePoint() -> const cv::Point;

	auto	getInfo() -> const std::vector< std::pair< cv::Rect, std::vector< cv::Rect > > >;
	auto	getEyes() -> const std::vector< cv::Rect >;
	auto	getFaces() -> const std::vector< cv::Rect >;

	auto	getColorfulEye( const cv::Mat & mat, const cv::Rect & roi = {} ) -> const cv::Rect;

private:
	bool					initialised;

	cv::CascadeClassifier	faceCascade,
							eyesCascade,
							smileCascade;

	//std::vector< cv::Rect >	faces;
	//std::vector< cv::Rect >	eyes;
	//std::vector< cv::Rect >	smiles;
	std::vector< std::pair< cv::Rect, std::vector< cv::Rect > > > answer;

	int			time;
	cv::Point	point;
	cv::Rect	rect;

};

