#include <QApplication>
#include <QDesktopwidget>
#include <QScreen>
#include <QTimer>
#include <QPixmap>
#include "bot.h"
#include "aithread.h"
#include <QDebug>

aiThread::aiThread() {}

void aiThread::init() {
	addBot();
	QTimer::singleShot( 1, this, &aiThread::update );
}

void aiThread::addBot() {
	auto object = new Bot();
	addThread( object );

	connect( this, &aiThread::setScreen, object, &Bot::getScreenshot );
}

#include <QtWin>
#include <windows.h>

BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam ) {
	if ( !IsWindowVisible( hwnd ) ) {
		return TRUE;
	}

	WCHAR class_name[ 800 ];
	WCHAR title[ 800 ];
	GetClassName( hwnd, class_name, sizeof( class_name ) );
	GetWindowText( hwnd, title, sizeof( title ) );
	qDebug() << "Window title: " << QString::fromWCharArray( title ) << endl;
	qDebug() << "Class name: " << QString::fromWCharArray( class_name ) << endl << endl;

	return TRUE;
}

auto	getJuggerScreen()
-> QImage {
	//EnumWindows( EnumWindowsProc, NULL );
	
	//
	RECT rc;
	HWND hWnd = ::FindWindow( L"TJuggClientForm", NULL );    //the window can't be min
	if ( hWnd == NULL ) {
		return{};
	}
	//EnumChildWindows( hWnd, EnumWindowsProc, NULL );

	//hWnd = ::FindWindowEx( hWnd, NULL, L"TBrowserTabSheet", NULL );
	//if ( hWnd == NULL ) {
	//	return{};
	//}
	

	GetClientRect( hWnd, &rc );

	HDC hdcScreen = GetDC( NULL );
	HDC hdc = CreateCompatibleDC( hdcScreen );
	HBITMAP hbmp = CreateCompatibleBitmap( hdcScreen,
										   rc.right - rc.left, rc.bottom - rc.top );
	SelectObject( hdc, hbmp );

	PrintWindow( hWnd, hdc, PW_CLIENTONLY );
	return QtWin::fromHBITMAP( hbmp ).toImage();
}

void aiThread::update() {
	auto screen = QGuiApplication::primaryScreen();
	auto id = QApplication::desktop()->winId();

	auto screenPixmap = screen->grabWindow( id );
	emit setScreen( screenPixmap.toImage() ); //getJuggerScreen()

	QTimer::singleShot( 500, this, &aiThread::update );
}
