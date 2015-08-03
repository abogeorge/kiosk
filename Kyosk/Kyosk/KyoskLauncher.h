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
	char* checkAdditionalProcess();
	void sweepProcesses(HDESK desktopHandle);
	DWORD listProcessThreads(DWORD dwOwnerPID);
	BOOL terminateProcessHandle(DWORD dwProcessId);
};

#define WM_TRAYMESSAGE (WM_USER + 1)