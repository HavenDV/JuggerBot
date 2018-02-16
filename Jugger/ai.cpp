#include "ai.h"

AI::AI( QObject * parent ) : 
	QObject{ parent } {
	init();
}

void AI::init() {
	addhThread();
}

void AI::stop() {
	threadControl.prepare();
	threadControl.stop();
	emit finished();
}

void AI::addhThread() {
	auto object = new aiThread();
	threadControl.addThread( object );
}