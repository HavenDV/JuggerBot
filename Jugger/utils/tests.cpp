#include <vector>
#include <opencv2\viz\vizcore.hpp>
#include "tests.h"
#include "utils.h"
#include <QDebug>

//namespace {

	cv::Mat	cornerDetect( cv::Mat in, int arg1, int arg2, int arg3 ) {
		cv::Mat corners, dilated_corners;
		preCornerDetect( toGray( in, false ), corners, 7 );
		// dilation with 3x3 rectangular structuring element
		dilated_corners = morph( corners, 55, 12 );
		auto corner_mask = dilated_corners < ( static_cast< float >( arg1 ) / 100 );
		return corners;
	}

	cv::Mat furie( cv::Mat I ) {
		if ( I.empty() )
			return I;

		I = toGray( I );
		cv::Mat padded;                            //expand input image to optimal size
		int m = cv::getOptimalDFTSize( I.rows );
		int n = cv::getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder( I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all( 0 ) );

		cv::Mat planes[] = { cv::Mat_<float>( padded ), cv::Mat::zeros( padded.size(), CV_32F ) };
		cv::Mat complexI, ngng;
		merge( planes, 2, complexI );         // Add to the expanded another plane with zeros

		dft( complexI, complexI );            // this way the result may fit in the source matrix

		// compute the magnitude and switch to logarithmic scale
		// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
		split( complexI, planes );                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		magnitude( planes[ 0 ], planes[ 1 ], planes[ 0 ] );// planes[0] = magnitude
		cv::Mat magI = planes[ 0 ];

		magI += cv::Scalar::all( 1 );                    // switch to logarithmic scale

		log( magI, magI );

		// crop the spectrum, if it has an odd number of rows or columns
		magI = magI( cv::Rect( 0, 0, magI.cols & -2, magI.rows & -2 ) );
		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = magI.cols / 2;
		int cy = magI.rows / 2;

		cv::Mat q0( magI, cv::Rect( 0, 0, cx, cy ) );   // Top-Left - Create a ROI per quadrant
		cv::Mat q1( magI, cv::Rect( cx, 0, cx, cy ) );  // Top-Right
		cv::Mat q2( magI, cv::Rect( 0, cy, cx, cy ) );  // Bottom-Left
		cv::Mat q3( magI, cv::Rect( cx, cy, cx, cy ) ); // Bottom-Right

		cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo( tmp );
		q3.copyTo( q0 );
		tmp.copyTo( q3 );

		q1.copyTo( tmp );                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo( q1 );
		tmp.copyTo( q2 );

		normalize( magI, magI, 0, 1, cv::NORM_MINMAX ); // Transform the matrix with float values into a
												// viewable image form (float between values 0 and 1).

		return magI;
	}

	cv::Mat moments( cv::Mat in, int thresh ) {
		if ( in.empty() )
			return in;
		cv::Mat out;
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;

		//Canny( toGray( in ), out, thresh, thresh*2, 3 );
		threshold( toGray( in ), out, thresh, 255, cv::THRESH_BINARY );
		findContours( out, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, { 0, 0 } );

		std::vector<std::vector<cv::Point> >hull( contours.size() );
		for ( int i = 0; i < contours.size(); i++ )
			cv::convexHull( cv::Mat( contours[ i ] ), hull[ i ], false );

		std::vector<cv::Moments> mu( contours.size() );
		for ( int i = 0; i < contours.size(); i++ )
			mu[ i ] = cv::moments( hull[ i ], false );

		std::vector<cv::Point2f> mc( contours.size() );
		for ( int i = 0; i < contours.size(); i++ )
			mc[ i ] = cv::Point2f( mu[ i ].m10 / mu[ i ].m00, mu[ i ].m01 / mu[ i ].m00 );

		cv::Mat drawing = cv::Mat::zeros( out.size(), CV_8UC3 );
		for ( int i = 0; i < contours.size(); i++ ) {
			auto color( randomColor() );
			//drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
			//drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			//drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			circle( drawing, mc[ i ], 4, color, -1, 8, 0 );
		}

		return drawing;
	}

//}