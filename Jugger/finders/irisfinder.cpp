#include "../utils/utils.h"
#include "../utils/hmat.h"
#include "../utils/edges.h"
#include "irisfinder.h"

IrisFinder::IrisFinder( const cv::Mat & mat, const cv::Rect & roi ) {
	if ( mat.empty() ) {
		return;
	}

	find( mat, empty( roi ) ? toRect( mat ) : roi );
}

auto	IrisFinder::getRect() const
-> const cv::Rect2d {
	return rect;
}

auto	IrisFinder::getPoint() const
-> const cv::Point2d {
	return point;
}

auto	IrisFinder::getRadius() const
-> const float {
	return radius;
}

auto	IrisFinder::getDebugMat() const
-> const cv::Mat {
	return debugMat;
}

auto	IrisFinder::getTemplate() const
-> const cv::Mat{
	return templateMat;
}

auto	IrisFinder::getHistoryMat() const
-> const cv::Mat {
	return historyMat;
}

auto	IrisFinder::getChanceMap( const cv::Size & size, const cv::Point & point ) -> cv::Mat {
	auto center = getCenterPoint< int >( size );
	auto minSize = std::min( size.width, size.height ) / 2 - 1; 
	//auto delta = cv::Point{ minSize, minSize };
	//auto start = clamp( center - delta, size );
	//auto end = clamp( center + delta, size );
	auto rect = toRect( size );
	auto start = rect.tl();
	auto end = rect.br();

	cv::Mat_< float > out{ size, 0.5F };
	
	for ( auto x = start.x; x < end.x; ++x ) {
		for ( auto y = start.y; y < end.y; ++y ) {
			auto current = cv::Point{ x, y } - center;
			//auto distance = std::sqrt( current.x * current.x + current.y * current.y );
			//auto chance = 0.25 * distance / minSize;
			auto chance = 1.0 - std::sqrt( 0.04 * current.x * current.x + 0.15 * current.y * current.y ) / minSize;
			auto procent = clamp< float >( chance, 0.0F, 1.0F );

			out.at< float >( { x, y } ) = procent;
		}
	}
	
	if ( point.x > 0 && point.y > 0 ) {
		auto rectDelta = cv::Point{ minSize / 4, minSize / 4 };
		out( clamp( cv::Rect{ point - rectDelta, point + rectDelta }, out ) ) += 0.1F;
	}
	//createWindow( "chanceMap", {} );
	//imshow_s( "chanceMap", out );
	
	return out;
}

auto	IrisFinder::getEdgesMat( const cv::Mat & mat )
-> cv::Mat_< uchar > {
	//cv::Canny( mat, out, 100, 255, 3, true );
	//cv::Laplacian( mat, out, CV_8U, 5, 0.5 ); toGray( out, false )
	return getEdgesFromKernel( mat, { { 1, 0 }, { -1, 0 }, { 0, 1 } } );
}

auto	IrisFinder::getIrisTemplate( float radius ) -> cv::Mat {
	if ( templateCache.count( radius ) > 0 ) {
		return templateCache.at( radius );
	}

	auto size = cv::Size2f{ radius, radius };
	auto matSize = 2 * size + cv::Size2f{ 7.0F, 7.0F };
	auto out = cv::Mat_< float >{ matSize, 0.0F };
	auto center = getCenterPoint< int >( out );
	cv::ellipse( out, center, size, 0, 0, 360, { 0.01F }, cv::FILLED, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 30, 150, { -0.2F }, 4, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 0, 180, { 0.2F }, cv::FILLED, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 0, 360, { 0.5F }, 2, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 0, 180, { 1.5F }, 2, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 340, 380, { 2.0F }, 3, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 160, 200, { 2.0F }, 3, cv::LINE_AA );
	templateCache.insert( std::pair< float, cv::Mat >( radius, out ) );

	return out;
}

template< typename T1 >
auto	easeBorders( const cv::Mat_< T1 > mat )
-> decltype( mat ) {
	auto out = decltype( mat ){ mat.size() };

	for ( auto x = 0; x < mat.cols; ++x ) {
		auto kx = 1.0 - ( 0.5 * std::abs( 0.5 - static_cast< double >( x ) / mat.cols ) );
		for ( auto y = 0; y < mat.rows; ++y ) {
			auto ky = 1.0 - ( 1.0 * std::abs( 0.5 - static_cast< double >( y ) / mat.rows ) );
			out.at< T1 >( { x, y } ) = static_cast< T1 >( kx * ky * mat.at< T1 >( { x, y } ) );
		}
	}

	return out;
}

auto	historyMean( const std::vector< cv::Mat > & history )
-> cv::Mat {
	if ( history.empty() ) {
		return{};
	}

	auto out = cv::Mat{ history[ 0 ].size(), history[ 0 ].type(), { 0, 0, 0 } };
	auto count = std::count_if( history.begin(), history.end(), [ &out ] ( auto && mat ) {
		return mat.size() == out.size();
	} );

	if ( count == 0 ) {
		return{};
	}

	for ( auto && mat : history ) {
		if ( mat.size() == out.size() ) {
			out += mat / count;
		}
	}

	return out;
}

//Поиск 4 эллипсов
//Поиск по ключевым точкам
//postEdges
//dif
//Картинка среднего за несколько десятков выборок.
//integral for template size > 11, 11
auto	IrisFinder::find( const cv::Mat & mat, const cv::Rect & roi )
-> Iris {
	if ( mat.empty() ) {
		return{};
	}

	//createWindow( "dfd1", { 0, 0 } );
	//createWindow( "dfd2", { 640, 0 } );
	//imshow_s( "dfd1", resize( mat( roi ), 10.0 ) );
	//imshow_s( "dfd2", HMat{ mat( roi ) } );

	auto eyeMat = retina.getParvo( filter( mat( roi ) ) );

	auto findRadius = 7.0F;
	//auto eyeShapeRect = eyeShapeFinder.find( mat, roi );
	auto edgesMat = easeBorders( getEdgesMat( eyeMat ) );
	/*
	cv::Mat integralMat;
	cv::integral( edgesMat, integralMat, cv::DataType< float >::type );
	cv::normalize( integralMat, integralMat, 1, 0, cv::NormTypes::NORM_MINMAX );
	createWindow( "integralMat", { 640, 0 } );
	imshow_s( "integralMat", integralMat );
	*/
	//edgesMat -= cv::mean( edgesMat );
	templateMat = getIrisTemplate( findRadius );

	if ( history.size() == 20 ) {
		auto mean = historyMean( history );
		if ( !mean.empty() ) {
			templateMat += ( 0.125F * 1.0F / 255.0F ) * cv::Mat_< float >( mean );
		}
	}
	auto chanceMap = getChanceMap( edgesMat.size() );

	rect = findTemplate( edgesMat, templateMat ); //chanceMap
	point = getCenterPoint( rect );
	radius = findRadius;
	lastMat = eyeMat.clone();
	// use dif frames
	//need full edges mat in cv::mean if expand
	history.insert( history.begin(), edgesMat( rect ).clone() );
	if ( history.size() > 20 ) {
		history.pop_back();
	}

	historyMat = historyMean( history );
	debugMat = !asColor( edgesMat, Colors::All ) | cv::Mat_< cv::Vec3b >{ edgesMat.size(), { 255, 0, 0 } };
	cv::rectangle( debugMat, rect, { 0, 0, 255 }, 1 );
	cv::circle( debugMat, point, radius, { 0, 0, 255 }, 1, cv::LINE_AA );

	rect = toMatCoord( rect, roi );
	point = toMatCoord( point, roi );
	return Iris{ radius, point };
}
