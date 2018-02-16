#pragma once
#ifndef HMATLOADER_H
#define HMATLOADER_H

#include "himageloader.h"
#include <opencv2\opencv.hpp>

class HMatLoader : public HImageLoader {
	Q_OBJECT

public:
	HMatLoader( QObject *parent = nullptr, QNetworkAccessManagerPtr manager = QNetworkAccessManagerPtr() );
	HMatLoader( QUrl url, QObject *parent = nullptr, QNetworkAccessManagerPtr manager = QNetworkAccessManagerPtr() );

	cv::Mat		getMat();

private:
	cv::Mat		mat;
	
};

#endif // HMATLOADER_H
