#pragma once
#include <QObject>
#include <QMimeData>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

class spyMouse : public QObject {
	Q_OBJECT

public:
	explicit spyMouse( QObject *parent = nullptr );

protected:
	bool eventFilter( QObject* obj, QEvent* event );
	bool testFormat( QObject* obj );

signals:
	void newMessage();
	void record();

};
