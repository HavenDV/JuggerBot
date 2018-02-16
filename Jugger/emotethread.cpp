#include "emoteThread.h"
#include <QDebug>

void emoteThread::add( QVariant data ) {
    emit setEmote( data );
}


