#include "eKey.h"

eKey::eKey(void) {

	}


eKey::~eKey(void) {

	}

void eKey::button(QString text) {
	keybd_event(VK_LWIN, 0, 0, 0);
	keybd_event(0x52, 0, 0, 0);
	keybd_event(0x52, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}