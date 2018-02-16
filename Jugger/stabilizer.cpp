#include "../utils/utils.h"
#include "stabilizer.h"

stabilizer::stabilizer(QObject *parent)
	: QObject(parent) {

}

stabilizer::~stabilizer() {

}

cv::Mat	stabilizer::antiRotate( cv::Mat in) {
	if ( in.empty() || in.size().width <= 5 || in.size().height <= 5 ) return in;
	cv::Mat out;
	if ( prevRotate.empty() || prevRotate.size() != in.size() ) {
		updateRotate( in );
	}
	int angle = getOptimalAffineAngle( in, prevRotate, -25, 25, 50 );
	cv::Mat rotMat = getRotationMatrix2D( getCenterPoint( in ), angle, 1.0 );
	warpAffine( in, out, rotMat, in.size() );
	return out;
}

//Rect match( Mat in, Mat templ, int match_method = CV_TM_CCOEFF_NORMED ) {
cv::Rect match( cv::Mat in, cv::Mat templ, int match_method = cv::TM_CCOEFF_NORMED ) {
	cv::Mat out;
	
	cv::matchTemplate( in, templ, out, match_method );
	cv::normalize( out, out, 0, 1, cv::NORM_MINMAX );

	double minVal, maxVal; 
	cv::Point minLoc, maxLoc, matchLoc;

	cv::minMaxLoc( out, &minVal, &maxVal, &minLoc, &maxLoc );

	//if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) 
	if( match_method  == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED )
		matchLoc = minLoc;
	else
		matchLoc = maxLoc;

	return{ matchLoc, templ.size() };
}

cv::Mat	stabilizer::antiMove( cv::Mat in) {
	if ( in.empty() || in.size().width <= 5 || in.size().height <= 5 ) return in;
	cv::Mat gray, curr64f, prev64f;

	gray = toGray( in ); 
	if ( prevMove.empty() || prevMove.size() != gray.size() ) {
		updateMove( gray );
		createHanningWindow( hann, gray.size(), CV_64F );
	}

	prevMove.convertTo( prev64f, CV_64F ); 
    gray.convertTo( curr64f, CV_64F );

	cv::Point2d shift = phaseCorrelate( prev64f, curr64f, hann );
	int	dx = round( shift.x, 1.0 ), dy = round( shift.y, 1.0 );

	cv::Mat out( in.size(), in.type(), cv::Scalar::all( 0 ) );
	cv::Size	sizeRect( in.size() - cv::Size( std::min( abs( dx ), in.size().width ),
										std::min( abs( dy ), in.size().height ) ) );
	cv::Point	onlyPositive( dx > 0 ? dx : 0,	dy > 0 ? dy : 0 ),
			onlyNegative( dx < 0 ? -dx : 0,	dy < 0 ? -dy : 0 );
	cv::Rect	inRect( onlyPositive, sizeRect );
	cv::Rect	outRect( onlyNegative, sizeRect );

	if ( sizeRect.width && sizeRect.height )
		out( outRect ) += in( inRect );

	///*
	cv::Rect centerRect( cv::Rect(	in.size().width >> 2, in.size().height >> 2,
							in.size().width >> 1, in.size().height >> 1 ) );
	cv::Rect matchRect = match( toGray( in ), prevMove( centerRect ) );
	
	out = cv::Scalar::all( 0 );
	out( centerRect ) += in( matchRect );
	//*/
	//rectangle( out, matchRect, Scalar::all(255) );

	return out;
}

cv::Mat stabilizer::antiMove2( cv::Mat in ) {
	if ( in.empty() ) return in;

	cv::Mat gray, flow, out = in.clone();

    gray = toGray( in );

	if ( prevMove.empty() || prevMove.size() != gray.size() )
		updateMove( gray );

	cv::calcOpticalFlowFarneback( prevMove, gray, flow, 0.5, 3, 15, 5, 5, 1.2, cv::OPTFLOW_FARNEBACK_GAUSSIAN );
	drawOptFlow( flow, out, 4.0, 2.0, { 0, 255, 0 } );

	//update( gray );
	return out;
}

void stabilizer::updateMove( cv::Mat in ) {
	prevMove = in.clone();
}

void stabilizer::updateRotate( cv::Mat in) {
	prevRotate = in.clone();
}

/* Affine Transform
Mat rotMat = getRotationMatrix2D(Point( in.cols >> 1, in.rows >> 1 ), arg, 1.0);
warpAffine( in, out, rotMat, in.size() );
*/

/* Perspective Transform
Point2f inputQuad[4], outputQuad[4]; 
 
inputQuad[0] = Point2f( arg - arg2, arg2 - arg );
inputQuad[1] = Point2f( in.cols - arg + arg2, arg2 - arg );
inputQuad[2] = Point2f( in.cols + arg + arg2, in.rows + arg2 + arg );
inputQuad[3] = Point2f( -arg - arg2, in.rows - arg2 + arg );  

outputQuad[0] = Point2f( 0, 0 );
outputQuad[1] = Point2f( in.cols, 0 );
outputQuad[2] = Point2f( in.cols, in.rows );
outputQuad[3] = Point2f( 0, in.rows );
		
Mat lambda = getPerspectiveTransform( inputQuad, outputQuad );
warpPerspective(in,in2,lambda,in.size() );
*/

