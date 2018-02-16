#pragma once
#include <QPoint>

class HMouse {

public:

	HMouse();

	static	void move( int x, int y );
	static	void move( const QPoint & point );
	static	void leftClick( const QPoint & point = {} );
	static	void rightClick( const QPoint & point = {} );
	static	auto getPosition() -> const QPoint;

};