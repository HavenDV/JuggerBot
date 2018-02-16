#pragma once
#include <QObject>
#include <opencv2\opencv.hpp>

class stabilizer : public QObject {
	Q_OBJECT

public:
				stabilizer(QObject *parent);
				~stabilizer();
	cv::Mat		antiRotate( cv::Mat in );
	cv::Mat		antiMove( cv::Mat in );
	cv::Mat		antiMove2( cv::Mat in );
	void		updateMove( cv::Mat in );
	void		updateRotate( cv::Mat in );

private:
	cv::Mat		prevRotate, prevMove;
	cv::Mat		hann;
};
