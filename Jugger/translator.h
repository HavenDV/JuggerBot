#pragma once
#include <QObject>
#include <opencv2\opencv.hpp>

class translator : public QObject {
	Q_OBJECT

public:
	cv::Size	inSize, outSize;
	cv::Rect	outRect;
	int			x1, x2, y1, y2;
	bool		invx, invy;

		translator(QObject *parent);
		~translator();
void	setSize( cv::Size in );
void	setInvert( bool invertx = true, bool inverty = false );
void	reset();
bool	isEmpty();
cv::Mat	operator ()( cv::Point in );

private:
	
};
