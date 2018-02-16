#pragma once
#ifndef HIMAGELOADER_H
#define HIMAGELOADER_H

#include <QImage>
#include <QImageReader>
#include "hloader.h"

class HImageLoader : public HLoader {
	Q_OBJECT

public:
	HImageLoader( QObject *parent = nullptr, QNetworkAccessManagerPtr manager = QNetworkAccessManagerPtr() );
	HImageLoader( QUrl url, QObject *parent = nullptr, QNetworkAccessManagerPtr manager = QNetworkAccessManagerPtr() );

	static QImage	downloadImage( qint64 bytesReceived, qint64 bytesTotal, int width, int height );

	QImage			getImage();

private:
	QImage				image;

};

#endif // HIMAGELOADER_H
