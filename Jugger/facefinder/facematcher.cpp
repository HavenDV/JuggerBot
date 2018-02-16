#include "facematcher.h"
#include "../utils/utils.h"
#include "../utils/qt.h"
#include "../utils/print.h"
#include <opencv2/face.hpp>
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/surface_matching.hpp>

cv::Mat		findHomo( cv::Mat in, cv::Mat object, int minHessian ) {
	if ( in.empty() || object.empty() )
		return in;

	auto detector = cv::xfeatures2d::SIFT::create();

	std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
	cv::Mat descriptors_object, descriptors_scene;

	detector->detectAndCompute( object, cv::noArray(), keypoints_object, descriptors_object );
	detector->detectAndCompute( in, cv::noArray(), keypoints_scene, descriptors_scene );

	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match( descriptors_object, descriptors_scene, matches );

	auto max_dist = 0.0;
	auto min_dist = 0.0;
	for ( int i = 0; i < descriptors_object.rows; i++ ) {
		double dist = matches[ i ].distance;
		if ( dist < min_dist ) {
			min_dist = dist;
		}
		if ( dist > max_dist ) {
			max_dist = dist;
		}
	}

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< cv::DMatch > good_matches;

	for ( auto i = 0; i < descriptors_object.rows; ++i ) {
		if ( matches[ i ].distance < 3 * min_dist ) {
			good_matches.push_back( matches[ i ] );
		}
	}

	cv::Mat img_matches;
	drawMatches( object, keypoints_object, in, keypoints_scene,
				 good_matches, img_matches, cv::Scalar::all( -1 ), cv::Scalar::all( -1 ),
				 std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//-- Localize the object
	std::vector<cv::Point2f> obj;
	std::vector<cv::Point2f> scene;

	for ( int i = 0; i < good_matches.size(); i++ ) {
		//-- Get the keypoints from the good matches
		obj.push_back( keypoints_object[ good_matches[ i ].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[ i ].trainIdx ].pt );
	}

	if ( obj.size() < 5 || scene.size() < 5 ) {
		return img_matches;
	}

	cv::Mat H = findHomography( obj, scene, CV_RANSAC );

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<cv::Point2f> obj_corners( 4 );
	obj_corners[ 0 ] = cvPoint( 0, 0 ); obj_corners[ 1 ] = cvPoint( object.cols, 0 );
	obj_corners[ 2 ] = cvPoint( object.cols, object.rows ); obj_corners[ 3 ] = cvPoint( 0, object.rows );
	std::vector<cv::Point2f> scene_corners( 4 );

	cv::perspectiveTransform( obj_corners, scene_corners, H );

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line( img_matches, scene_corners[ 0 ] + cv::Point2f( object.cols, 0 ), scene_corners[ 1 ] + cv::Point2f( object.cols, 0 ), cv::Scalar( 0, 255, 0 ), 4 );
	line( img_matches, scene_corners[ 1 ] + cv::Point2f( object.cols, 0 ), scene_corners[ 2 ] + cv::Point2f( object.cols, 0 ), cv::Scalar( 0, 255, 0 ), 4 );
	line( img_matches, scene_corners[ 2 ] + cv::Point2f( object.cols, 0 ), scene_corners[ 3 ] + cv::Point2f( object.cols, 0 ), cv::Scalar( 0, 255, 0 ), 4 );
	line( img_matches, scene_corners[ 3 ] + cv::Point2f( object.cols, 0 ), scene_corners[ 0 ] + cv::Point2f( object.cols, 0 ), cv::Scalar( 0, 255, 0 ), 4 );

	return img_matches;
}

FaceMatcher::FaceMatcher() {

}

void FaceMatcher::getFeatureInfo( cv::Mat &mat, Detector &detector, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors, std::vector<cv::Mat> &mats ) {
	//MSER surf; //StarAdjuster
	auto surf = cv::xfeatures2d::SIFT::create();
	cv::Mat grayMat{ toGray( mat, false ) };

	auto rects = detector.detectFaces( grayMat );
	for ( auto rect : rects ) {
		rectangle( mat, rect, { 0, 0, 255 } );
		cv::Mat temp = grayMat( rect );
		surf->detect( temp, keypoints );
		surf->detectAndCompute( temp, cv::noArray(), keypoints, descriptors );
		mats.insert( mats.end(), temp );
	}
}

void FaceMatcher::findFaceMatches( cv::Mat& mat1, cv::Mat& mat2, Detector &detector ) {
	if ( mat1.empty() || mat2.empty() ) {
		return;
	}

	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	std::vector<cv::Mat> mats1, mats2;

	//getFeatureInfo( mat1, detector, keypoints1, descriptors1, mats1 );
	//getFeatureInfo( mat2, detector, keypoints2, descriptors2, mats2 );

	int i = 0;
	for ( auto mat1 : mats1 ) {
		int j = 0;
		for ( auto mat2 : mats2 ) {
			cv::Mat out;
			cv::BFMatcher matcher;
			std::vector< cv::DMatch > matches;
			matcher.match( descriptors1, descriptors2, matches );
			cv::drawMatches( mat1, keypoints1, mat2, keypoints2, matches, out );
			Print( out, { "%1 / %2"_q.arg( matches.size() ).
							arg( std::max( keypoints1.size(), keypoints2.size() ) ) } );
			imshow_s( "matches%1%2"_q.arg( i ).arg( j ).toStdString(), out );
			j++;
		}
		i++;
	}

	auto rects = detector.detectFaces( toGray( mat1 ) );
	if ( !rects.empty() ) {
		//imshow_s( "matches", findHomo( mat2, mat1( rects[ 0 ] ) ) );
	}
	imshow_s( "mat1", mat1 );
	imshow_s( "mat2", mat2 );
}