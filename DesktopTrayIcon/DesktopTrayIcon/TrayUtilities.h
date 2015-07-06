#pragma once
class TrayUtilities
{
public:

};

#define WM_TRAYMESSAGE (WM_USER + 1)

extern HINSTANCE hInst;

void TrayDrawIcon(HWND hWnd);
void TrayDeleteIcon(HWND hWnd);
void ShowBalloon(LPCTSTR szText, LPCTSTR szTitle, DWORD dwIcon = NIIF_NONE, UINT uTimeout = 10);
void TrayLoadPopupMenu(HWND hWnd);
PROCESS_INFORMATION startConfig();