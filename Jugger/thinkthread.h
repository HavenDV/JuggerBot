#pragma once
#include <QVariant>
#include "hthread.h"

class thinkThread : public HThread {
	Q_OBJECT

public slots:
    void add(QVariant);

signals: 
    void setThink(QVariant);
    void setSound(QVariant);
};

