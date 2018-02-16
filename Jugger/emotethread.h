#pragma once
#include <QVariant>
#include "hthread.h"

class emoteThread : public HThread {
	Q_OBJECT

public slots:
    void add(QVariant);

signals: 
    void setEmote(QVariant);
    void setSound(QVariant);
};

