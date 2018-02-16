#include "../utils/utils.h"
#include "../utils/qt.h"
#include "../utils/entersections.h"
#include "../utils/edges.h"
#include "eyeshapefinder.h"

EyeShapeFinder::EyeShapeFinder( const cv::Mat & mat, const cv::Rect & roi ) {
	if ( mat.empty() ) {
		return;
	}

	find( mat, empty( roi ) ? toRect( mat ) : roi );
}

auto	generateIrisMarkers( const cv::Size & size, const cv::Point2f & center, float radius ) -> cv::Mat {
	auto mat = cv::Mat_< uchar >{ size, cv::GC_BGD };
	cv::circle( mat, center, radius * 1.5F, cv::GC_PR_FGD, cv::FILLED );
	cv::circle( mat, center, radius / 1.5F, cv::GC_FGD, cv::FILLED );
	return mat;
}

auto	findIrisRect( cv::Mat & mat ) -> cv::Rect { //const 
	auto markers = generateIrisMarkers( mat.size(), getRealCenterMass( toGray( mat ) < 50 ), 7.0F );

	cv::grabCut( mat, markers, {}, cv::Mat{}, cv::Mat{}, 1, cv::GC_INIT_WITH_MASK );
	cv::Mat eyeMask = markers != cv::GC_PR_FGD & markers != cv::GC_FGD;

	auto points = maskToContour( eyeMask );
	if ( points.size() > 5 ) {
		auto irisRect = cv::boundingRect( points );
		//return irisRect;
		//rectangle( mat, irisRect, { 255, 255, 255 } );
		//cv::drawContours( mat, std::vector< std::vector< cv::Point > >{ points }, 0, { 0, 0, 0 } );
	}

	//mat = mat & atColor( eyeMask, Colors::All );

	return toRect( mat.size() );
}

auto	EyeShapeFinder::generateMarkers( const cv::Size & matSize, const cv::Point & center, const cv::Size & size ) -> cv::Mat {
	auto mat = cv::Mat_< uchar >{ matSize, cv::GC_BGD };
	cv::ellipse( mat, center, size, 0, 0, 360, cv::GC_PR_FGD, cv::FILLED );
	cv::ellipse( mat, center, size / 3, 0, 0, 360, cv::GC_FGD, cv::FILLED );
	return mat;
}

auto	EyeShapeFinder::getContour( const cv::Mat & mat, const cv::Point & point )
-> Contour {
	auto ellipseMaxSize = cv::Size{ 20, 8 };
	auto markers = generateMarkers( mat.size(), point, ellipseMaxSize );
	//cv::watershed( mat, markers );

	cv::grabCut( mat, markers, {}, cv::Mat{}, cv::Mat{}, 1, cv::GC_INIT_WITH_MASK );

	auto mask = cv::Mat( markers != cv::GC_PR_FGD & markers != cv::GC_FGD );
	return maskToContour( mask );
}

auto	EyeShapeFinder::optimalContour( const cv::Mat & mat )
-> Contour {
	/*
	//auto ellipseMaxSize = cv::Size{ ( 11 * mat.size().width ) / 24, mat.size().height / 6 };
	auto ellipseMaxSize = cv::Size{ 20, 8 };
	auto markers = generateMarkers( mat.size(), getCenterPoint< int >( mat ), ellipseMaxSize );
	//cv::watershed( mat, markers );
	cv::grabCut( mat, markers, {}, cv::Mat{}, cv::Mat{}, 1, cv::GC_INIT_WITH_MASK );

	auto mask = cv::Mat( markers != cv::GC_PR_FGD & markers != cv::GC_FGD );
	auto points = maskToContour( mask );
	auto s = cv::contourArea( points );
	auto width = contourWidth( points );
	auto height = contourHeight( points );
	*/
	auto center = getCenterPoint< int >( mat );
	auto points = getContour( mat, center );
	auto maxWidth = contourWidth( points );
	auto maxArea = cv::contourArea( points );
	for ( auto y = mat.rows / 4; y < 3 * mat.rows / 4; ++y ) {
		auto contour = getContour( mat, { center.x, y } );
		auto width = contourWidth( contour );
		auto area = cv::contourArea( contour );
		if ( area > maxArea ) {
			points = contour;
			maxWidth = width;
			maxArea = area;
		}
	}

	return points;
}

auto	EyeShapeFinder::contourFind( const cv::Mat & mat ) -> cv::Rect {
	auto points = optimalContour( mat );
	/*
	auto mask = cv::Mat_< uchar >{ mat.size(), 0 };
	for ( auto x = 0; x < mat.cols; ++x ) {
		for ( auto y = 0; y < mat.rows - 1; ++y ) {
			setEyeMask( mat, { x, y }, { x, y + 1 }, mask );
		}
	}
	auto mask2 = cv::Mat_< uchar >{ mat.size(), 0 };
	for ( auto x = 0; x < mat.cols; ++x ) {
		for ( auto y = 1; y < mat.rows; ++y ) {
			setEyeMask( mat, { x, y }, { x, y - 1 }, mask2 );
		}
	}
	*/
	auto mask = getEdgesFromKernel( mat, { { 0, 1 }, { 0, -1 } } );
	debugMat = mat + asColor( mask, Colors::Green );
	//cv::pyrMeanShiftFiltering( mat, debugMat, 5, 10, 3 );
	//debugMat = mat.clone();
	//cv::floodFill( debugMat, { 0, 0 }, { 0, 0, 255 }, &getCenterRect< int >( mat ), { 10, 10, 255 }, { 3, 3, 255 }, 8 ); //, &domain, loDiff, upDiff, neighbors


	if ( points.size() > 10 ) {
		auto boundingRect = cv::boundingRect( points );
		//boundingRect.x -= 5;
		boundingRect.width += 10;
		auto delta = std::max( boundingRect.width - boundingRect.height, 0 );
		boundingRect -= cv::Point{ 5, delta / 2 };
		//boundingRect += cv::Size{ 10, delta };
		//if ( delta > 0 ) {
		//boundingRect.y -= delta / 2;
		boundingRect.height += delta;
		//}
		rect = clamp( boundingRect, mat );
		/*
		debugMat = mat.clone();
		cv::rectangle( debugMat, rect, { 0, 255, 255 } );
		cv::drawContours( debugMat, Contours{ points }, 0, { 0, 255, 255 } );
		cv::addText( debugMat, "%1"_q.arg( contourWidth( points ) ).toStdString(), { 5, 10 }, {} );
		//auto ellipse = cv::fitEllipse( points );
		//cv::ellipse( debugMat, ellipse, { 0, 255, 0 } );
		*/
		return rect;
	}

	return rect = toRect( mat.size() );
}

auto	EyeShapeFinder::getTemplate( float radius ) -> cv::Mat {
	if ( templateCache.count( radius ) > 0 ) {
		//return templateCache.at( radius );
	}

	auto size = cv::Size2f{ radius, radius / 2 };
	auto matSize = 2 * size + cv::Size2f{ 7.0F, 7.0F };
	auto out = cv::Mat_< float >{ matSize, 0.0F };
	auto center = getCenterPoint( out );
	cv::ellipse( out, center, size, 0, 180, 360, { -1.0F }, 4, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 0, 360, { 0.1F }, cv::FILLED, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 0, 180, { 0.2F }, 1, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 180, 360, { 1.0F }, 2, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 180, 230, { 2.0F }, 3, cv::LINE_AA );
	cv::ellipse( out, center, size, 0, 310, 360, { 2.0F }, 3, cv::LINE_AA );
	//cv::ellipse( out, cv::Point2f{ center.x, center.y / 2 }, cv::Size2f{ radius / 2, radius / 2 }, 0, 0, 180, { 2.0F }, 2, cv::LINE_AA );
	templateCache.insert( std::pair< float, cv::Mat >( radius, out ) );
	//ellipse = { center, size, 0 }; // NOT WORK WITH TEMPLATE CACHE!

	return out;
}

auto	EyeShapeFinder::createEllipseTemplateTop( float radius ) -> cv::Mat_< float > {
	auto size = cv::Size2f{ 2.0F * 0.5F * radius, 2.0F * 0.6F * radius } / 2.0F;
	auto matSize = cv::Size2f{ size.width * 2, 1.2F * size.height };
	auto mat = cv::Mat_< float >{ matSize, 0.0F };
	auto center = cv::Point2f{ getCenterPoint( mat ).x, 0 };
	cv::ellipse( mat, center, size, 0, 45, 135, { 1.0F }, 2, cv::LINE_AA );
	top = { center, size, 0 };

	return mat;
}

auto	EyeShapeFinder::createEllipseTemplateBottom( float radius ) -> cv::Mat_< float > {
	auto size = cv::Size2f{ 2.0F * 0.5F * radius, 2.0F * 0.6F * radius } / 2.0F;
	auto matSize = cv::Size2f{ size.width * 2, size.height };
	auto mat = cv::Mat_< float >{ matSize, 0.0F };
	auto center = cv::Point2f{ getCenterPoint( mat ).x, 1.0F * matSize.height };
	cv::ellipse( mat, center, size, 0, 225, 315, { 1.0F }, 2, cv::LINE_AA );
	bottom = { { center.x, radius }, size, 0 };

	return mat;
}

void	showTemplateForDebug( cv::Mat & mat, const cv::Rect rect, const cv::Mat & templateMat ) {
	auto clampedRect = clamp( rect, mat );
	auto deltaPoint = rect.tl() - clampedRect.tl();
	auto deltaSize = rect.size() - clampedRect.size();
	auto outRect = toRect( templateMat );
	outRect += cv::Point{ std::max( -deltaPoint.x, 0 ), std::max( -deltaPoint.y, 0 ) };
	outRect -= cv::Size{ std::max( deltaSize.width, 0 ), std::max( deltaSize.height, 0 ) };

	mat( clampedRect ) += asColor( cv::Mat_< uchar >( 255 * templateMat( outRect ) ), Colors::Red );
}

auto	getEdges( const cv::Mat & mat )
-> cv::Mat_< uchar > {
	return getEdgesFromKernel( mat, { { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 0 }, { -1, 0 } } );
}

auto	EyeShapeFinder::templateFind( const cv::Mat & mat, const cv::Rect & roi ) -> cv::Rect {
	if ( mat.empty() ) {
		return{};
	}

	debugMat = getEdges( retina.getParvo( filter( mat( clamp( expand( roi, { 20, 20 } ), mat ) ) ) ) );
	return{};

	auto eyeMat = retina.getParvo( filter( mat( roi ) ) );
	auto findRadius = 14.0F;
	auto edgesMat = getEdges( eyeMat );
	//edgesMat -= cv::mean( edgesMat );
	templateMat = getTemplate( findRadius );

	rect = findTemplate( edgesMat, templateMat ); //add chanceMap

	//showTemplate( "Eye Shape Template", templateMat );
	debugMat = !asColor( edgesMat, Colors::All ) | cv::Mat_< cv::Vec3b >{ edgesMat.size(), { 255, 0, 0 } };
	cv::rectangle( debugMat, rect, { 0, 0, 255 }, 1 );
	//cv::ell	ipse( debugMat, getCenterPoint( rect ), cv::Size2f{ findRadius, findRadius / 2 }, 0, 180, 360, { 0, 0, 255 }, 2, cv::LINE_AA );
	showTemplateForDebug( debugMat, rect, templateMat );

	return rect;
}

auto	EyeShapeFinder::ellispesFind( const cv::Mat & mat, const cv::Rect & roi ) -> cv::Rect {
	if ( mat.empty() ) {
		return{};
	}
	if ( empty( roi ) ) {
		//roi = toRect( mat );
	}
	auto eyeMat = retina.getParvo( filter( mat( clamp( expand( roi, { 20, 20 } ), mat ) ) ) );
	//debugMat = getEdges( eyeMat );

	auto findRadius = 1.0F * eyeMat.size().height;
	auto edgesMat = getEdges( eyeMat );
	//edgesMat -= cv::mean( edgesMat );
	auto templateMatTop = createEllipseTemplateTop( findRadius );
	auto templateMatBottom = createEllipseTemplateBottom( findRadius );

	auto rectTop = findTemplate( edgesMat, templateMatTop );
	auto rectBottom = findTemplate( edgesMat, templateMatBottom );

	debugMat = !asColor( edgesMat, Colors::All ) | cv::Mat_< cv::Vec3b >{ edgesMat.size(), { 255, 0, 0 } };
	//cv::rectangle( debugMat, rect, { 0, 0, 255 }, 1 );
	showTemplateForDebug( debugMat, rectTop, templateMatTop );
	showTemplateForDebug( debugMat, rectBottom, templateMatBottom );

	//auto y1 = 1.0F * rectBottom.y;
	//auto y2 = 1.0F * rectTop.br().y;
	auto y1 = 0.4F * eyeMat.size().height;
	auto y2 = 0.6F * eyeMat.size().height;
	//auto center = cv::Point2f{ getCenterPoint( eyeMat ).x, ( y1 + y2 ) / 2 };
	//auto r = std::sqrt( y2 * y2 - center.y * center.y );
	//auto x1 = center.x - r;
	//auto x2 = center.x + r;

	auto x1 = 0.0F, x2 = 0.0F;
	auto points = ellipsesEntersection( cv::Rect{ 0, 0, eyeMat.size().width, static_cast< int >( y2 ) }, cv::Rect{ 0, static_cast< int >( y1 ), eyeMat.size().width, static_cast< int >( eyeMat.size().height - y1 ) } );
	if ( !points.empty() ) {
		x1 = points[ 0 ].x;
		x2 = points[ 1 ].x; //MB ERROR. NEED TEST!
	}

	rect = cv::Rect{ cv::Point2f{ x1, y1 }, cv::Point2f{ x2, y2 } };
	rect = expandToSquare( rect );
	//top.center = { top.center.x, 1.0F * rect.y };
	//bottom.center = { bottom.center.x, 1.0F * rect2.br().y };
	rect = toMatCoord( rect, roi, { 20, 20 } );
	top.center = toMatCoord( top.center, roi, { 20, 20 } );
	bottom.center = toMatCoord( bottom.center, roi, { 20, 20 } );

	return rect;
}

// Система поиска 4-ех эллипсов. Снизу, сверху - разрез, слева справа - радужка.
// Point system. As SURF.
auto	EyeShapeFinder::find( const cv::Mat & mat, const cv::Rect & roi ) -> cv::Rect {
	return ellispesFind( mat, roi );
}

auto	EyeShapeFinder::getRect() const
-> const cv::Rect2f {
	return rect;
}

auto EyeShapeFinder::getEllipse() const
-> const cv::RotatedRect {
	return { ellipse.center + rect.tl(), ellipse.size, ellipse.angle };
}

auto	EyeShapeFinder::getEllipseTop() const -> const cv::RotatedRect {
	return top;
}

auto	EyeShapeFinder::getEllipseBottom() const -> const cv::RotatedRect {
	return bottom;
}

auto	EyeShapeFinder::getDebugMat() const
-> const cv::Mat {
	return debugMat;
}

auto	EyeShapeFinder::getTemplate() const
-> const cv::Mat {
	return templateMat;
}
