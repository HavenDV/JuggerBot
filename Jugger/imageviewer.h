#pragma once
#include <QWidget>
#include <QPixmap>
#include <QUrl>
#include <QLabel>
#include <QImage>
#include "loaders\himageloader.h"

class ImageViewer : public QWidget {
	Q_OBJECT

public:
	ImageViewer( QWidget * parent = nullptr );

	void	view( const QPixmap & pixmap );
	void	view( const QImage & image );
	void	view( const QUrl & url );

private:
	void	onFinished();

	QLabel			label;
	HImageLoader	loader;

};

