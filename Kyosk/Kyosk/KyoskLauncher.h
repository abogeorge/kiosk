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
	PROCESS_INFORMATION startAdditionalProcess();
};

//extern  HWND hWnd;

#define WM_TRAYMESSAGE (WM_USER + 1)