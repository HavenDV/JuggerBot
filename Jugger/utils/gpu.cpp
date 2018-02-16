#include "gpu.h"
#include "utils.h"
#include <opencv2/core/ocl.hpp>
#include <QDebug>
#ifdef GPU
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudawarping.hpp>
#endif
//DEPRECATED. USE UMAT

bool	isCuda() {
	int count = 0;

	try {
#ifdef GPU
		count = cv::cuda::getCudaEnabledDeviceCount();
#endif
	}
	catch ( const cv::Exception & e ) {
		qDebug() << e.what();
	}

	return count > 0 ? true : false;
}

void	initCuda()	{
#ifdef GPU
	cv::cuda::setDevice( 0 );
	filterGPU( cv::Mat::zeros( 10, 10, CV_8UC3 ) );
#endif
}

bool	isOpenCL() {
	return cv::ocl::haveOpenCL();
}

int getOptimalAffineAngleGPU( cv::Mat in1, cv::Mat in2, int start_angle, int end_angle, int thresh ) {
	double smin = 9999, angle = 0;
#ifdef GPU
	cv::cuda::GpuMat gpuin1( in1 ), gpuin2( in2 ), gpuOut, gpuCheck;
	
	for (int i = start_angle; i < end_angle; i += 1 ) {
		cv::Mat rotMat = cv::getRotationMatrix2D( cv::Point( in1.cols >> 1, in1.rows >> 1 ), i, 1.0);
		cv::cuda::warpAffine( gpuin1, gpuOut, rotMat, in1.size() );
		cv::cuda::absdiff( gpuin2, gpuOut, gpuCheck );
		double s = drawContour( cv::Mat(gpuCheck), thresh, false );
		if ( s < smin ) { 
			smin = s;
			angle = i;
		}
	}
#endif
	return angle;
}

cv::Mat filterGPU( cv::Mat in ) {
	
	if( in.empty() )
        return in;
#ifdef GPU
	cv::cuda::GpuMat gpuin( in );
	//blur
	//GaussianBlur( in, out, Size(7,7), 1 );
	//medianBlur
	//adaptiveBilateralFilter( in, out, Size( 5, 5 ), 5, 100.0 );
	cv::cuda::bilateralFilter( gpuin, gpuin, 9, 100, 5 );
	return cv::Mat{ gpuin };
#endif
	return in;
}
