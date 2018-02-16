#pragma once
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/viz/vizcore.hpp>
#include "hthread.h"
#include "3d/rect.h"
#include "3d/heye.h"
#include "3d/hscreen.h"
#include "3d/hcamera.h"
#include "hkalmanfilter.h"

class H3DVizualizer : public HThread {
	Q_OBJECT

public:
	H3DVizualizer();

private:
	cv::viz::Viz3d		window;
	HScreen				screen1;
	HCamera				camera1;
	HEye				eye1, eye2;

	HKalmanFilter		filter;

public slots:
	void	init();
	void	prepare();
	void	update() noexcept;

	void	showScreen( const cv::String & name, const HScreen & screen );
	void	showCamera( const cv::String & name, const HCamera & camera );
	void	showEye( const cv::String & name, const HEye & eye, const HScreen & screen, const cv::viz::Color & color, const cv::Rect & previewRect = {} );

	void	showEyeMesh( const cv::String & name, const cv::Mat & mat, const cv::Point3d & pos, const cv::Vec3d & normal, double radius );
	void	showScreenCloud( const cv::String & name, const HEye & eye, const cv::Rect3d & rect, const cv::viz::Color & color );
	void	showScreenEyeCircle( const cv::String & name, const cv::Point3d & point, const cv::Vec3d & normal, const cv::viz::Color & color );
	void	showEyePlane( const cv::String & name, const cv::Mat & mat, double radius, const cv::Point3d & position, const cv::Vec3d & normal, const cv::viz::Color & color );

	void	showVizScreen( const QImage & image );
	void	showVizCamera( const QImage & image );
	//cv::Point3d	computeEyePos( const cv::Point2d & position, const HCamera & camera );

	void	setVizEyeParams( const QRectF & rect, const QPointF & center, double radius );
	void	setAlternativeVizEyeParams( const QRectF & rect, const QPointF & center, double radius );

	//void	showTest( const HScreen & screen );

	void	showLine( const cv::String & name, const cv::Point3d & point1, const cv::Point3d & point2, const cv::viz::Color & color, double width = 1.0 );
	void	showRect( const cv::String & name, const cv::Rect3d & rect, const cv::viz::Color & color, double width = 1.0 );
	void	showLinesToRect( const cv::String & name, const cv::Point3d & start, const cv::Rect3d & rect, const cv::viz::Color & color, double width = 1.0 );
	void	showImage( const cv::String & name, const cv::Rect3d & rect, const cv::Mat & mat );

signals:
	void	setEyePoint( const QPointF & point );
	void	setAlternativeEyePoint( const QPointF & point );

};
