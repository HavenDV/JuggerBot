#pragma once
#include <QUrl>
#include <QVariant>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include "hthread.h"

class soundPlayer : public HThread {
    Q_OBJECT
public:
    soundPlayer();
    //void play(QString _url);
private:
	QMediaPlayer player;
	QMediaPlaylist playlist;
	//QNetworkAccessManager * nam;
    //QMap<QNetworkReply*,QUrl> requests;
	//QString urlToFileName(QUrl);
public slots:
	void init();
    void add(QVariant);
    void sayText(QVariant);
    void play();
    void update();
	void error(QMediaPlayer::Error error);
    //void download(QUrl);
    //void onFinished(QNetworkReply*);
};

