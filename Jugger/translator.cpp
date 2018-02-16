#include "translator.h"

using namespace cv;

translator::translator(QObject *parent)
	: QObject{ parent } {
	reset();
	setInvert();
}

translator::~translator() {

}

void	translator::setSize( Size in ) {
	outSize = in;
}

void	translator::setInvert( bool invertx, bool inverty ) {
	invx = invertx;
	invy = inverty;
}

void	translator::reset() {
	x1 = -1;
	x2 = -1;
	y1 = -1;
	y2 = -1;
}

bool	translator::isEmpty() {
	return x1 == -1 && x2 == -1 && y1 == -1 && y2 == -1;
}

Mat	translator::operator ()( Point in ) {
	if ( !in.x || !in.y ) return Mat();
	if ( isEmpty() ) {
		x1 = x2 = in.x;
		y1 = y2 = in.y;
	}

	if ( in.x < x1 ) x1 = in.x;
	if ( in.x > x2 ) x2 = in.x;
	if ( in.y < y1 ) y1 = in.y;
	if ( in.y > y2 ) y2 = in.y;

	inSize = { x2 - x1, y2 - y1 };
	Point outPos{ in.x - x1, in.y - y1 };
	outPos = {	invx ? inSize.width - outPos.x : outPos.x,
				invy ? inSize.height - outPos.y : outPos.y };
	inSize += Size{ 1, 1 };
	Point2f scale{	inSize.width > 0 ? static_cast< float >( outSize.width ) / static_cast< float >( inSize.width ) : 1.0F,
					inSize.height > 0 ? static_cast< float >( outSize.height ) / static_cast< float >( inSize.height ) : 1.0F };
	Rect outRect{	Point( scale.x * outPos.x, scale.y * outPos.y ),
					Size( scale.x, scale.y ) };
	Mat out{ outSize, CV_8U, Scalar::all( 255 ) };
	rectangle( out, outRect, Scalar::all( 0 ), -1 );
	//out( outRect ) = 255;
	//Mat out( inSize, CV_8U, Scalar::all( 255 ) ); 
	//rectangle( out, outPos, outPos, Scalar::all( 0 ), -1 );
	return out;
}
