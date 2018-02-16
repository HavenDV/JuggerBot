#pragma once
#include "QObject"
#include "aithread.h"
#include "hthreadparent.h"

class AI : public QObject {
	Q_OBJECT

public:
			AI( QObject * parent = nullptr );

private:
	HThreadParent		threadControl;

	void	addhThread();

public slots:
	void	init();
	void	stop();

signals:
	void	finished();

};