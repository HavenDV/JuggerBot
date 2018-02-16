#pragma once
#include <QImage>
#include "hthreadparent.h"

class aiThread : public HThreadParent {
    Q_OBJECT

public:
    aiThread();

    void	init();
    void	update();

private:
	void	addBot();

signals:
	void	setScreen( QImage );
    void	finished();
	
};
