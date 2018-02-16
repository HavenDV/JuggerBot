#pragma once
#include <opencv2/opencv.hpp>

template < typename T1 >
auto	dif( const T1 value1, const T1 value2 )
-> T1 {
	//return value1 > value2 ? value1 - value2 : value2 - value1;
	return value1 > value2 ? value1 - value2 : 0;
}

template < typename T1, int T2 >
auto	dif( const cv::Vec< T1, T2 > & vec1, const cv::Vec< T1, T2 > & vec2 )
-> cv::Vec< T1, T2 > {
	cv::Vec< T1, T2 > out;
	for ( auto i = 0; i < cv::Vec< T1, T2 >::channels; ++i ) {
		out[ i ] = dif( vec1[ i ], vec2[ i ] );
	}
	return out;
}

auto	getEdgesFromKernel( const cv::Mat & mat, const cv::Point & kernel ) -> cv::Mat_< uchar >;
auto	getEdgesFromKernel( const cv::Mat & mat, const std::vector< cv::Point > & kernels ) -> cv::Mat_< uchar >;