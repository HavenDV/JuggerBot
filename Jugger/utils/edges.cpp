#include "utils.h"
#include "edges.h"

auto	getEdgesFromKernel( const cv::Mat & mat, const cv::Point & kernel )
-> cv::Mat_< uchar > {
	cv::Mat_< uchar > out{ mat.size(), 0 };
	out.forEach( [ & ] ( auto && color, auto && position ) {
		auto x = position[ 1 ];
		auto y = position[ 0 ];
		if ( x < std::max( -kernel.x, 0 ) || x >= mat.cols - std::max( kernel.x, 0 ) ||
			 y < std::max( -kernel.y, 0 ) || y >= mat.rows - std::max( kernel.y, 0 ) ) {
			return;
		}

		auto to = cv::Point{ x + kernel.x, y + kernel.y };
		auto && fromColor = mat.at< cv::Vec3b >( position );
		auto && toColor = mat.at< cv::Vec3b >( to );
		auto difColor = dif( toColor, fromColor );
		auto value = vectorSum( difColor );
		out.at< uchar >( to ) = clamp( value, 0, 255 );
	} );

	out -= 3 * cv::mean( out );
	return out;
}

auto	getEdgesFromKernel( const cv::Mat & mat, const std::vector< cv::Point > & kernels )
-> cv::Mat_< uchar > {
	cv::Mat_< uchar > out{ mat.size(), 0 };
	for ( auto && kernel : kernels ) {
		out += getEdgesFromKernel( mat, kernel );
	}

	return out;
}