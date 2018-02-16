#pragma once
#ifndef HTHREADPARENT_H
#define HTHREADPARENT_H

#include "hthread.h"

class HThreadParent : public HThread {
	Q_OBJECT

public:
	HThreadParent();
	virtual			~HThreadParent();

	virtual void	prepare();
	virtual void	stop();

	void	addThread( HThread* object );
	void	waitPreparedThreads();
	void	waitThreads();


private:
	size_t	notPreparedThreadCount,
		threadCount;

	public slots:
	void	threadStopped();
	void	threadPrepared();

signals:
	void	prepareAll();
	void	stopAll();

};

#endif // HTHREADPARENT_H
