#include "hloader.h"
#include <QNetworkRequest>
#include <QDebug>

HLoader::HLoader( QObject *parent, QNetworkAccessManagerPtr _manager ) :
	QObject{ parent }, manager{ _manager }, bytesReceived{ 0 }, bytesTotal{ 0 } {
	if ( manager.isNull() ) {
		manager = { new QNetworkAccessManager{ this }, &QObject::deleteLater };
	}
}

HLoader::HLoader( QUrl url, QObject *parent, QNetworkAccessManagerPtr manager ) :
	HLoader{ parent, manager } {
	load( url );
}

void	HLoader::load( QUrl url ) {
	QNetworkRequest request{ url };
	reply = { manager->get( request ), &QObject::deleteLater };
	connect( reply.data(), &QNetworkReply::finished,
			 this, &HLoader::onFinished );
	connect( reply.data(), &QNetworkReply::downloadProgress,
			 this, &HLoader::downloadProgress );
}

auto	HLoader::getData() -> QNetworkReplyPtr {
	return reply;
}

void HLoader::downloadProgress( qint64 _bytesReceived, qint64 _bytesTotal ) {
	bytesReceived = _bytesReceived;
	bytesTotal = _bytesTotal;
}

void	HLoader::onFinished() {
	auto redirectedAttr = reply->attribute( QNetworkRequest::RedirectionTargetAttribute );
	auto redirectedUrl = redirectedAttr.toUrl();

	if ( redirectedUrl.isValid() ) {
		if ( redirectedUrl != reply->request().url() ) {
			load( redirectedUrl );
		}
		else {
			qWarning() << "[NetworkingPrivate] Infinite redirect loop at " + redirectedUrl.toString();
		}
	}
	else {
		emit finished();
	}
}
