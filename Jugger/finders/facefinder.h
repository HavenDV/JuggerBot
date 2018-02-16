#pragma once
#include <opencv2\opencv.hpp>

class FaceFinder {

public:
	FaceFinder();

	//Automate in first use any methods.
	void	initialise();

	auto	isInitialised() -> const bool;
	auto	isDetected() -> const bool;

	auto	find( const cv::Mat & mat ) -> const std::vector< cv::Rect >;

	auto	getFaces() -> const std::vector< cv::Rect >;

private:

	bool					initialised;
	cv::CascadeClassifier	cascade;
	std::vector< cv::Rect >	faces;

};