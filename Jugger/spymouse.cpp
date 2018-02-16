#include "spymouse.h"

spyMouse::spyMouse( QObject *parent ) :
    QObject( parent ) {
}

bool spyMouse::eventFilter( QObject* obj, QEvent* event ) {
    if ( event->type() == QEvent::KeyPress ) {
        auto keyEvent = static_cast<QKeyEvent*>( event );
        //qDebug() << keyEvent->key();
        if ( testFormat( obj ) && keyEvent->key() == Qt::Key_Return ) {
            emit newMessage();
        }
		if ( testFormat( obj ) && keyEvent->key() == Qt::Key_Space ) {
			emit record();
		}
    }

    return QObject::eventFilter( obj, event );
}

bool spyMouse::testFormat( QObject* obj ) {
	if ( obj->objectName() != "text_edit" ) {
		return false;
	}
   // if (obj->metaObject()->className()==QString("QPlainTextEdit")) return true;
   // if (obj->parent()&&
   //         obj->parent()->metaObject()->className()==QString("QPlainTextEdit")) return true;
    return true;
}
