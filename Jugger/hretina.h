#pragma once
#ifndef HRETINA_H
#define HRETINA_H

#include <opencv2\opencv.hpp>
#include <opencv2\bioinspired.hpp>

class HRetina {

public:
	HRetina();

	void	update( const cv::Mat & in );
	auto	getMagno( const cv::Mat & mat = {} ) -> cv::Mat;
	auto	getParvo( const cv::Mat & mat = {} ) -> cv::Mat;

private:
	cv::Ptr<cv::bioinspired::Retina>	retina;
	cv::Mat								parvo, magno;

};

#endif // HRETINA_H
