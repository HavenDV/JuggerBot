#include "imageviewer.h"
#include <QVariant>
#include <QDebug>


ImageViewer::ImageViewer( QWidget * parent ) :
	QWidget{ parent }, label{ this } {
	connect( &loader, &HImageLoader::finished, 
			 this, &ImageViewer::onFinished );
}

void ImageViewer::view( const QPixmap & pixmap ) {
	label.resize( pixmap.size() );
	label.setPixmap( pixmap );
	resize( pixmap.size() );
	show();
}

void ImageViewer::view( const QImage & image ) {
	view( QPixmap::fromImage( image ) );
}

void ImageViewer::view( const QUrl & url ) {
	loader.load( url );
}

void ImageViewer::onFinished() {
	view( loader.getImage() );
}