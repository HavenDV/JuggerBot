#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ai.h"
#include "spymouse.h"

auto main( int argc, char *argv[] ) {
	QStringList paths = QCoreApplication::libraryPaths();
	paths.append( "." );
	paths.append( "platforms" );
	paths.append( "imageformats" );
	paths.append( "mediaservice" );
	paths.append( "audio" );
	QCoreApplication::setLibraryPaths( paths );
	
	QApplication a{ argc, argv };
	QIcon hIcon{ ":/AI/H16.png" };
	//a.setQuitOnLastWindowClosed( false );
	//a.setWindowIcon( hIcon );

	AI w{ &a };
	QObject::connect( &w, &AI::finished, &QApplication::quit );
	
	QMenu trayMenu;
	trayMenu.addAction( u8"Закрыть", &w, SLOT( stop() ) );

	QSystemTrayIcon trayIcon{ hIcon, &a };
	if ( QSystemTrayIcon::isSystemTrayAvailable() ) {
		trayIcon.setContextMenu( &trayMenu );
		trayIcon.show();
	}

	QObject::connect( &a, &QApplication::aboutToQuit, 
					  &trayIcon, &QSystemTrayIcon::hide );

	return a.exec();
}