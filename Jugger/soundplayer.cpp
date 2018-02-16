/* 
 * Доделать - выбрасывает исключение "неизвестный интерфейс"
 */

#include "soundPlayer.h"
#include <QDebug>
#include <QTextDocument>
#include <QTextCodec>
#include <QNetworkConfigurationManager>
#include <QFileInfo>
#include <QBuffer>

soundPlayer::soundPlayer() : 
				player{ this, QMediaPlayer::LowLatency }, playlist{ this } {
	connect( &player, 
			 static_cast<void ( QMediaPlayer::* )( QMediaPlayer::Error )>( &QMediaPlayer::error ),
			 this, &soundPlayer::error );
}

void soundPlayer::init() {
	//connect( &player, &QMediaPlayer::positionChanged, 
	//		 this, &soundPlayer::positionChanged );

	//connect(	&nam, &QNetworkAccessManager::finished ), 
	//			this, &soundPlayer::onFinished );

	player.setVolume( 100 );
	player.setPlaylist( &playlist );

	//QNetworkAccessManager s;
	//qDebug() << "Internet status: " << s.isOnline();
	//playlist->setPlaybackMode( QMediaPlaylist::Sequential );
}

//QString soundPlayer::urlToFileName( QUrl url ) {
//	QString cacheDir="D:/cache/";
//	QList<QString> list=url.toString(QUrl::EncodeSpaces).split("/");
//	QString fileName=list.at(list.size()-1);
//	QList<QString> list1=fileName.split("?");
//	fileName=list1.at(list1.size()-1);
//	QList<QString> list2=fileName.split("%20");
//	fileName="";
//	for (int i=0; i<list2.size(); i++) 
//		fileName+=list2.at(i);
//	return cacheDir+fileName;
//}

void soundPlayer::add( QVariant _url ) {
	QUrl url{ _url.toString() };
	playlist.addMedia( url );
	if ( player.state() != QMediaPlayer::PlayingState ) {
		player.play(); //Выбрасывает исключения
	}

	//auto codec = QTextCodec::codecForName("UTF-8");
	//test = codec->fromUnicode( _url.toString() );
	//auto filePath = urlToFileName( url );
	//playlist.addMedia( { filePath } );
	//qDebug() << filePath << " added to playlist";
	//if ( !QFile::exists( filePath ) ) {
	//	download( url );
	//}	
}

void soundPlayer::sayText( QVariant text ) {
	playlist.clear();

	// Ограничение API Google на 100 символов. Разделяем на предложения.
    auto list = text.toString().split("."); 
    for ( auto str : list ) {
        add( "http://translate.google.com/translate_tts?ie=UTF-8&q=" + str + "&tl=ru" );
	}
	//add( "http://tts.voicetech.yandex.net/generate?text=\"%D0%A1%D0%BB%D0%BE%D0%B2%D0%BE%20%D0%BF%D1%80%D0%B8%D0%B2%D0%B5%D1%82%20%D1%83%D0%B6%D0%B5%20%D1%81%D1%83%D1%89%D0%B5%D1%81%D1%82%D0%B2%D1%83%D0%B5%D1%82%20%D0%B2%20%D0%B1%D0%B0%D0%B7%D0%B5%20%D0%B4%D0%B0%D0%BD%D0%BD%D1%8B%D1%85\"&format=wav&lang=ru-RU&speaker=zahar&key=1ce29818-0d15-4080-b6a1-ea5267c9fefd" );
	//download("http://download.wavetlan.com/SVV/Media/HTTP/H264/Talkinghead_Media/H264_test1_Talkinghead_mov_480x360.mov");
}

void soundPlayer::play() {
	if ( player.state() != QMediaPlayer::PlayingState ) {
		player.play();
	}
}


void soundPlayer::update() {
    //if (mediaObject->state()==Phonon::PausedState) mediaObject->clear();
    //mediaObject->play();
}

void soundPlayer::error( QMediaPlayer::Error error ) {
	qDebug() << "Type error: " << error << ". Error message: " << player.errorString();
	//qDebug() << "-----------------";
	//QNetworkConfigurationManager hello;
	//foreach (QNetworkConfiguration str, hello.allConfigurations()) {
	//	qDebug() << str.name() << str.bearerTypeName() << str.purpose();
	//}		
	//qDebug() << "-----------------";
	//qDebug() << "Player conf: " << player.currentNetworkConfiguration().name() << player.currentNetworkConfiguration().isValid();
	//qDebug() << "Default conf: " << hello.defaultConfiguration().name() << hello.defaultConfiguration().isValid();
}
//
//void soundPlayer::download(QUrl url) {
//    QNetworkRequest request;
//    request.setUrl(url);
//    QNetworkReply * reply = nam->get(request);
//    requests.insert(reply, url);
//	downCount++;
//	qDebug() << "Start download: " << url.toString();
//}
//
//void soundPlayer::onFinished(QNetworkReply* reply) {
//	qDebug() << "End download: " << reply->url().toString();
//    if (requests.contains(reply)) {
//        QVariant redirectedUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
//        QUrl redirectedTo = redirectedUrl.toUrl();
//        if (redirectedTo.isValid()) {
//            if (redirectedTo != reply->request().url()) {
//                download(redirectedTo.toString());
//            }
//            else {
//                qWarning() << "[NetworkingPrivate] Infinite redirect loop at " + redirectedTo.toString();
//            }
//        }
//        else {
//            if (reply->error() == QNetworkReply::NoError) {
//				QFile file(urlToFileName(reply->url()));
//				if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//					qDebug() << "File error";
//					return;
//				}
//				//QDataStream out(&file);
//				//out.setVersion(QDataStream::Qt_5_3);
//				//out << reply->readAll();
//				QByteArray data=reply->readAll();
//				file.write(data);
//				qDebug() << "File saved as " << file.fileName();
//				//add(file.fileName());
//				downCount--;
//				play();
//			}   
//            else
//                qDebug() << QString("[NetworkingPrivate] Reply error: %1").arg(reply->error());
//        }
//        requests.remove(reply);
//        reply->deleteLater();
//    }
//}
