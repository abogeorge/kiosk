#pragma once
class KyoskLauncher
{
public:
	KyoskLauncher();
	~KyoskLauncher();
	void startNewKyosk();
private:
	PROCESS_INFORMATION startProcess(STARTUPINFO startUpInfo, LPCTSTR lpApplicationName);
	bool killProcess(PROCESS_INFORMATION processInfo);
};

//extern  HWND hWnd;

#define WM_TRAYMESSAGE (WM_USER + 1)