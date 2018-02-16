#include "hthread.h"

HThread::HThread() :
	t { this }, prepared{ false }, stopped{ false } {
	connect( &t, &QTimer::timeout, this, &HThread::update );
}

HThread::~HThread() {
}

auto	HThread::isPrepared() const -> const bool {
	return prepared;
}

auto	HThread::isStopped() const -> const bool {
	return stopped;
}

void	HThread::init() {
}

void	HThread::prepare() {
	t.stop(); 
	prepared = true;
	emit threadPrepared();
}

void	HThread::stop() {
	stopped = true;
	emit threadStopped();
}

void	HThread::update() {
}