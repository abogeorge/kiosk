#include "stdafx.h"
#include "TrayUtilities.h"
#include "Resource.h"

/// structure that contains information that the system needs to display notifications in the notification area. Used by Shell_NotifyIcon.
NOTIFYICONDATA nid;

/// initialize and displays the tray icon
void TrayDrawIcon(HWND hWnd) {
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = IDI_ICON1;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.uCallbackMessage = WM_TRAYMESSAGE;
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	LoadString(hInst, IDS_APP_TITLE, nid.szTip, 128);
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &nid);
}

/// removes the tray icon
void TrayDeleteIcon(HWND hWnd) {
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = IDI_ICON1;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

/// displays a baloon text
void ShowBalloon(LPCTSTR szText, LPCTSTR szTitle, DWORD dwIcon, UINT uTimeout)
{
	nid.uFlags = NIF_INFO;
	wcsncpy_s(nid.szInfo, szText, 256);
	if (szTitle)
		wcsncpy_s(nid.szInfoTitle, szTitle, 64);
	else
		nid.szInfoTitle[0] = _T('\0');
	nid.dwInfoFlags = dwIcon;
	nid.uTimeout = uTimeout * 1000;   // convert time to ms

	Shell_NotifyIcon(NIM_MODIFY, &nid);
	nid.szInfo[0] = _T('\0');
}