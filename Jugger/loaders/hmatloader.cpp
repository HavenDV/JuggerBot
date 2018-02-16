#include "hmatloader.h"
#include "../utils/imageconverter.h"
#include <QDebug>

HMatLoader::HMatLoader( QObject *parent, QNetworkAccessManagerPtr _manager ) :
	HImageLoader{ parent, _manager } {}

HMatLoader::HMatLoader( QUrl url, QObject *parent, QNetworkAccessManagerPtr manager ) :
	HImageLoader{ url, parent, manager } {}

auto	HMatLoader::getMat()
-> cv::Mat {
	auto image = getImage();
	if ( !image.isNull() ) {
		mat = ImageConverter{ image };
	}

	return mat;
}
