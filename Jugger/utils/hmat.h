#pragma once
#include <opencv2/opencv.hpp>

// Add support for K < 1.0
class HMat {

public:
	HMat( const cv::Mat & _mat );
	HMat( const cv::Mat & _mat, double k );
	HMat( const cv::Mat & _mat, const cv::Size2d size );

	auto	setVirtualSize( const cv::Size2d & size ) -> HMat&;

	operator cv::Mat();
	operator cv::_InputArray();

	template < typename T1 >
	auto	HMat::at( double x, double y )
		-> T1 {
		auto cx = static_cast< int >( std::ceil( x ) );
		auto cy = static_cast< int >( std::ceil( y ) );
		auto && c1 = mat.at< T1 >( { cx, cy } );
		auto && c2 = mat.at< T1 >( { cx + 1, cy } );
		auto && c3 = mat.at< T1 >( { cx, cy + 1 } );
		auto && c4 = mat.at< T1 >( { cx + 1, cy + 1 } );
		auto kx1 = cx - x, kx2 = 1 - kx1;
		auto ky1 = cy - y, ky2 = 1 - ky1;
		auto k1 = kx1 * ky1;
		auto k2 = kx2 * ky1;
		auto k3 = kx1 * ky2;
		auto k4 = kx2 * ky2;
		return k1 * c1 + k2 * c2 + k3 * c3 + k4 * c4;
	}

	template < typename T1 >
	auto	asMat( double k )
		-> cv::Mat {
		cv::Mat out{ cv::Size2d{ k * ( mat.size().width - 2 ), k * ( mat.size().height - 2 ) }, mat.type() };
		out.forEach< T1 >( [ this, k ] ( auto && color, auto && position ) {
			auto x = position[ 1 ];
			auto y = position[ 0 ];

			color = at< T1 >( x / k, y / k );
		} );

		return out;
	}

private:
	cv::Mat mat;
	cv::Mat out;
	double k;

};