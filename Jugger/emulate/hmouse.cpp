#include <windows.h>
#include "hmouse.h"

HMouse::HMouse() {}

void	HMouse::move( int x, int y ) {
	//auto hWnd = ::FindWindow( L"TJuggClientForm", NULL );
	//POINT point;
	//point.x = x;
	//point.y = y;
	//ClientToScreen( hWnd, &point );
	//::DestroyWindow( hWnd );

	SetCursorPos( x, y );
	
	//mouse_event(MOUSEEVENTF_MOVE| MOUSEEVENTF_ABSOLUTE, point.x, point.y, 0, GetMessageExtraInfo());
}

void	HMouse::move( const QPoint & point ) {
	move( point.x(), point.y() );
}
#include <QDebug>
void LeftClick() {
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput( 1, &Input, sizeof( INPUT ) );

	// left up
	::ZeroMemory( &Input, sizeof( INPUT ) );
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput( 1, &Input, sizeof( INPUT ) );
}

void	HMouse::leftClick( const QPoint & point ) {
	if ( !point.isNull() ) {
		move( point );
	}
	
	Sleep( 50 );
	mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo() );
	Sleep( 1 );
	mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo() );
	
	/*
	RECT rc;
	auto hWnd = ::FindWindow( TEXT( "TJuggClientForm" ), NULL );
	GetClientRect( hWnd, &rc );
	DWORD dwProcessId = 0;
	DWORD dwThreadId = GetWindowThreadProcessId( hWnd, &dwProcessId );
	POINT wpoint;
	wpoint.x = point.x();
	wpoint.y = point.y();
	ClientToScreen( hWnd, &wpoint );
	//GetProcessId
	AttachThreadInput( GetCurrentThreadId(), dwProcessId, TRUE );
	//SetFocus( hWnd );
	LeftClick();
	AttachThreadInput( GetCurrentThreadId(), dwProcessId, FALSE );
	//SendInput
	//Dispatch
	auto param = MAKELPARAM( point.x(), point.y() );
	::SetCapture( hWnd );
	PostMessage( hWnd, WM_MOUSEMOVE, 0, param );
	Sleep( 50 );
	
	PostMessage( hWnd, WM_NCLBUTTONUP, MK_LBUTTON, param );
	Sleep( 50 );
	//PostMessage( hWnd, WM_KEYUP, VK_F11, NULL );
	//PostMessage( hWnd, WM_KEYDOWN, VK_F11, NULL );
	PostMessage( hWnd, WM_NCLBUTTONDOWN, MK_LBUTTON, param );
	//WM_ WM_
	::ReleaseCapture();
	*/
}

void	HMouse::rightClick( const QPoint & point ) {
	if ( !point.isNull() ) {
		move( point );
	}

	Sleep( 50 );
	mouse_event( MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo() );
	Sleep( 50 );
	mouse_event( MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo() );
	Sleep( 50 );
}

auto	HMouse::getPosition() -> const QPoint {
	POINT pos;
	if ( !GetCursorPos( &pos ) ) {
		return{};
	}
	 
	return{ pos.x, pos.y };
}
/*
int main() {
	HWND window = FindWindow( "TaskManagerWindow", "Task Manager" );
	if ( window ) {
		RECT rect = { 0 };
		GetClientRect( window, &rect );

		SetForegroundWindow( window );
		SetActiveWindow( window );
		SetFocus( window );
		Sleep( 300 );
		SetCursorPos( rect.right - 200, rect.bottom - 200 );
	}

	return 0;
}
*/