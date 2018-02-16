#pragma once
#ifndef HLOADER_H
#define HLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QUrl>

typedef QSharedPointer<QNetworkAccessManager>	QNetworkAccessManagerPtr;
typedef QSharedPointer<QNetworkReply>			QNetworkReplyPtr;

class HLoader : public QObject {
	Q_OBJECT

public:
	HLoader( QObject *parent = nullptr, QNetworkAccessManagerPtr manager = QNetworkAccessManagerPtr() );
	HLoader( QUrl url, QObject *parent = nullptr, QNetworkAccessManagerPtr manager = QNetworkAccessManagerPtr() );

public:
	void				load( QUrl url );
	QNetworkReplyPtr	getData();

protected:
	QNetworkAccessManagerPtr	manager;
	QNetworkReplyPtr			reply;

	qint64						bytesReceived,
								bytesTotal;

protected slots:
	void onFinished();
	void downloadProgress( qint64 bytesReceived, qint64 bytesTotal );

signals:
	void finished();
	
};

#endif // HLOADER_H
