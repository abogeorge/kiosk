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

//
//  FUNCTION: TrayLoadPopupMenu(HWND)
//
//  PURPOSE:  Load tray specific popup menu
//
//
void TrayLoadPopupMenu(HWND hWnd) {
	POINT cursor;
	HMENU hMenu;
	GetCursorPos(&cursor);
	hMenu = (HMENU)GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1)), 0);
	SetMenuDefaultItem(hMenu, ID__OPENCONFIGURATOR, false);
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, cursor.x, cursor.y, 0, hWnd, NULL);
}

PROCESS_INFORMATION startConfig()
{
	PROCESS_INFORMATION pInfoNTA; /// process infromation for the new thread
	ZeroMemory(&pInfoNTA, sizeof(pInfoNTA));
	STARTUPINFO sInfoNTA; /// startupinfo for the new thread
	ZeroMemory(&sInfoNTA, sizeof(sInfoNTA)); /// this macro fills a block of memory with zeros
	//sInfoNTA.lpDesktop = L"threadDesktop"; /// setting the desktop for the process
	CreateProcess(L"KeyConfig.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sInfoNTA, &pInfoNTA); /// starting the process for the key listener of the new desktop
	if (!pInfoNTA.hProcess){
		MessageBox(0, L"Unable to start the process!\nThe path is broken!", L"Path Error!", MB_ICONERROR);
	}
	return pInfoNTA;
}