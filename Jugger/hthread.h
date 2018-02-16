#pragma once
#ifndef HTHREAD_H
#define HTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>

class HThread : public QObject {
	Q_OBJECT

public:
	HThread();
	virtual ~HThread();

	auto	isPrepared() const -> const bool;
	auto	isStopped() const -> const bool;
    
protected:
    QTimer	t;
	bool	prepared,
			stopped;

public slots:
	virtual void	init();
	virtual void	prepare();
	virtual void	stop();
	virtual void	update();

signals: 
	void	threadPrepared();
    void	threadStopped();

};

#endif // HTHREAD_H