#include "thinkThread.h"

void thinkThread::add( QVariant data ) {
    emit setThink( data );
    //if ( data.toString() == "тоска" ) 
	//	emit setSound( "http://upload.wikimedia.org/wikipedia/commons/9/95/Ru-%D1%82%D0%BE%D1%81%D0%BA%D0%B0.ogg" );
}


