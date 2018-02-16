#include "himageloader.h"
#include <QPainter>
#include <QDebug>

HImageLoader::HImageLoader( QObject *parent, QNetworkAccessManagerPtr _manager ) :
	HLoader{ parent, _manager } {}

HImageLoader::HImageLoader( QUrl url, QObject *parent, QNetworkAccessManagerPtr manager ) :
	HLoader{ url, parent, manager } {}

QImage HImageLoader::downloadImage( qint64 bytesReceived, qint64 bytesTotal, int width = 300, int height = 30 ) {
	QImage image{ { width, height }, QImage::Format_RGB888 };
	QPainter painter{ &image };

	QFont font;
	font.setPixelSize( height - 2 );
	painter.setFont( font );

	painter.setRenderHint( QPainter::TextAntialiasing );
	painter.setPen( { 255, 255, 255 } );
	painter.setBrush( { { 1, 1, 1 } } );

	painter.drawRect( image.rect() );
	painter.drawText( image.rect(), Qt::AlignCenter,
					  QString( "%1 kB / %2 kB" ).arg( bytesReceived / 1000 ).arg( bytesTotal / 1000 ) );

	return image;
}

auto	HImageLoader::getImage() -> QImage {
	if ( reply.isNull() || !reply->isFinished() ) {
		return downloadImage( bytesReceived, bytesTotal, 300, 30 );
	}

	if ( image.isNull() ) {
		QImageReader reader{ reply.data() };
		reader.read( &image );
	}
	return image;
}
