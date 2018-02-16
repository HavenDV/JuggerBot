#pragma once
#include <opencv2/opencv.hpp>

//Entersector ??
auto	ellipsesEntersection( const cv::Rect & rect1, const cv::Rect & rect2 ) -> std::vector< cv::Point >;
auto	ellipsesEntersection( const cv::RotatedRect & ellipse1, const cv::RotatedRect & ellipse2 ) -> std::vector< cv::Point >;
