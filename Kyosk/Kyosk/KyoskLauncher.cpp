#include "stdafx.h"
#include "KyoskLauncher.h"


KyoskLauncher::KyoskLauncher()
{
}


KyoskLauncher::~KyoskLauncher()
{
}

/// controls the status of the application
// 0: Unitialized	|	1: User requested a desktop change	|	2: User requested closing the application
DWORD status = 0;

PROCESS_INFORMATION pInfoNTA; /// process infromation for the custom application thread

DWORD WINAPI pipeThread(LPVOID lpParam)
{
	char data[1024]; /// char array to store the received data from the pipe
	DWORD numRead; /// stores the number of items received from the pipe
	bool active = true;

	while (active)
	{
		/// handle to the pipe used to read the inbound messages
		HANDLE pipe = CreateNamedPipe(L"\\\\.\\pipe\\LogPipe", PIPE_ACCESS_INBOUND | PIPE_ACCESS_OUTBOUND, PIPE_WAIT, 1, 1024, 1024, 120 * 1000, NULL);
		ConnectNamedPipe(pipe, NULL);
		ReadFile(pipe, data, 1024, &numRead, NULL); /// reading the content of the named pipe

		if (numRead > 0){
			if (data[0] == 'S')  /// if the received data from pipe is a switch message
			{
				status = 1;
			}
			if (data[0] == 'C') /// if the received data from pipe is a exit message
			{
				active = false;
				status = 2;
			}
		}
		CloseHandle(pipe); /// closing the pipe handle
	}

	return 0;
}

DWORD WINAPI customApplicationThread(LPVOID lpParam)
{
	DWORD exitCode = 0; /// exit code for the custom application
	GetExitCodeProcess(pInfoNTA.hProcess, &exitCode);
	while (exitCode == STILL_ACTIVE){
		Sleep(20);
		GetExitCodeProcess(pInfoNTA.hProcess, &exitCode);
	}
	status = 2;
	return 0;
}

/// function startNewKyosk manages the lifecycle of the launched processes
void KyoskLauncher::startNewKyosk()
{
	status = 0;

	/// handle to the original desktop
	HDESK currentDesktop = GetThreadDesktop(GetCurrentThreadId());
	/// handle to the newly created desktop
	HDESK threadDesktop = CreateDesktop(L"threadDesktop", NULL, NULL, 0, GENERIC_ALL, NULL);
	bool activeDesktop = true; // if true = original desktop, false = "threadDesktop"
	LOG(INFO) << "Created desktops";

	/// Initializing key listener and tray icons
	STARTUPINFO sInfoKLOD; /// startupinfo for the key listener of the original desktop
	PROCESS_INFORMATION pInfoKLOD; /// process infromation for the key listener of the original desktop
	ZeroMemory(&sInfoKLOD, sizeof(sInfoKLOD));  /// this macro fills a block of memory with zeros
	pInfoKLOD = startProcess(sInfoKLOD, L"DesktopKeyLogger.exe"); /// starting the process for the key listener of the original desktop
	if (!pInfoKLOD.hProcess)
		LOG(ERROR) << "Unable to start the key listener for the current desktop";
	else
		LOG(INFO) << "Started the key listener for the current desktop";

	STARTUPINFO sInfoKLTD; /// startupinfo for the key listener of the new desktop
	PROCESS_INFORMATION pInfoKLTD; /// process infromation for the key listener of the new desktop
	ZeroMemory(&sInfoKLTD, sizeof(sInfoKLTD)); /// this macro fills a block of memory with zeros
	sInfoKLTD.lpDesktop = L"threadDesktop"; /// setting the desktop for the process
	pInfoKLTD = startProcess(sInfoKLTD, L"DesktopKeyLogger.exe"); /// starting the process for the key listener of the new desktop
	if (!pInfoKLTD.hProcess)
		LOG(ERROR) << "Unable to start the key listener for the new desktop";
	else
		LOG(INFO) << "Started the key listener for the new desktop";

	STARTUPINFO sInfoTITD;  /// startupinfo for the DesktopTrayIcon
	PROCESS_INFORMATION pInfoTITD; /// process infromation for the DesktopTrayIcon
	ZeroMemory(&sInfoTITD, sizeof(sInfoTITD)); /// this macro fills a block of memory with zeros
	sInfoTITD.lpDesktop = L"threadDesktop"; /// setting the desktop for the process
	pInfoTITD = startProcess(sInfoTITD, L"DesktopTrayIcon.exe"); /// starting the process for the DesktopTrayIcon
	if (!pInfoTITD.hProcess)
		LOG(ERROR) << "Unable to start the tray icon for the new desktop";
	else
		LOG(INFO) << "Started the tray icon for the new desktop";

	STARTUPINFO sInfoTI;  /// startupinfo for the DesktopTrayIcon
	PROCESS_INFORMATION pInfoTI; /// process infromation for the DesktopTrayIcon
	ZeroMemory(&sInfoTI, sizeof(sInfoTI)); /// this macro fills a block of memory with zeros
	//sInfoTITD.lpDesktop = L"threadDesktop"; /// setting the desktop for the process
	pInfoTI = startProcess(sInfoTI, L"DesktopTrayIcon.exe"); /// starting the process for the DesktopTrayIcon
	if (!pInfoTI.hProcess)
		LOG(ERROR) << "Unable to start the tray icon for the new desktop";
	else
		LOG(INFO) << "Started the tray icon for the new desktop";

	/// Disabling all options from ctrl+alt+del menu
	RegistryUtilities regUtilities;
	DWORD value = 1;
	if (regUtilities.changeAllFeatures(value) == true /*regUtilities.disableAllFeatures(value) == true*/)
		LOG(INFO) << "Disabled options from CTRL+ALT+DEL menu";
	else
		LOG(ERROR) << "Unable to disable options from CTRL+ALT+DEL menu";

	/// check if the user asked for a custom application to run
	LPCWSTR additionalProcess = checkAdditionalProcess();

	if (wcscmp(additionalProcess, L"#") == 0) {
		STARTUPINFO sInfoNT; /// startupinfo for the explorer thread
		PROCESS_INFORMATION pInfoNT; /// process infromation for the explorer thread
		ZeroMemory(&sInfoNT, sizeof(sInfoNT)); /// this macro fills a block of memory with zeros
		sInfoNT.lpDesktop = L"threadDesktop"; /// setting the desktop for the process
		pInfoNT = startProcess(sInfoNT, L"C:\\Windows\\explorer.exe"); /// starting the process for the explorer
		if (!pInfoNT.hProcess)
		{
			LOG(ERROR) << "Unable to start the new explorer process";
		}
		else
		{
			LOG(INFO) << "Started the new explorer process";
		}
	}
	else 
	{
		ZeroMemory(&pInfoNTA, sizeof(pInfoNTA));
		STARTUPINFO sInfoNTA; /// startupinfo for the custom application thread
		ZeroMemory(&sInfoNTA, sizeof(sInfoNTA)); /// this macro fills a block of memory with zeros
		sInfoNTA.lpDesktop = L"threadDesktop"; /// setting the desktop for the process
		pInfoNTA = startProcess(sInfoNTA, additionalProcess); /// starting the process for the new applcation
		if (!pInfoNTA.hProcess)
		{
			LOG(ERROR) << "Unable to start the new additional process";
		}
		else
		{
			LOG(INFO) << "Started the additional process";
		}

		HANDLE handleApplicationThread = 0;
		int dataOfApplicationThread = 1;
		handleApplicationThread = CreateThread(NULL, 0, customApplicationThread, &dataOfApplicationThread, 0, NULL);
	}

	NamedPipeUtilities pipes;
	bool stillActive = true; /// stiilActive variable keep the execution in the while loop as long as the key ESC is not pressed
	LOG(INFO) << "Waiting for either LCTRL or ESC to be pressed";
	HANDLE handlePipeThread = 0;
	int dataOfPipeThread = 1;
	handlePipeThread = CreateThread(NULL, 0, pipeThread, &dataOfPipeThread, 0, NULL);

	while (stillActive)
	{
		if (status == 1)
		{
			if (!activeDesktop) /// if the user is in the new thread desktop
			{ 
				SwitchDesktop(currentDesktop);
				activeDesktop = true;
				pipes.sendMessage("1");
				LOG(INFO) << "Switched to the original desktop";
				/// the desktop is changed to the original one
			}
			else /// if the user is in the original desktop
			{ 
				SwitchDesktop(threadDesktop);
				activeDesktop = false;
				pipes.sendMessage("2"); /// sends a message via named to pipe to inform the DesktopTrayIcon to display a message in the noficiation area
				LOG(INFO) << "Switched to the new desktop";
				///  the desktop is changed to the newly created one
			}
			status = 0;
		}

		if (status == 2)
		{
			stillActive = false; /// the variable that keeps us in the while loop is changed
			LOG(INFO) << "Detected ESC press";
			pipes.sendMessage("3"); /// sends a message via named pipe to eliminate the icon from the notification are
		}
	}

	/// enabling all options from ctrl+alt+del menu
	value = 0;
	if (regUtilities.changeAllFeatures(value) == true /*regUtilities.enableAllFeatures(value) == true*/)
		LOG(INFO) << "Enabled options from CTRL+ALT+DEL menu";
	else
		LOG(ERROR) << "Unable to enable options from CTRL+ALT+DEL menu";

	CloseDesktop(threadDesktop);
	SwitchDesktop(currentDesktop); /// the new destkop is closed and is changed to the original one
	LOG(INFO) << "Closed the new thread desktop and returned to the original one";

	/// the key listener for the current desktop is closed
	if (killProcess(pInfoKLOD) == true)
		LOG(INFO) << "Closed the key listener for the current desktop";
	else
		LOG(ERROR) << "Unable to close the key listener for the current desktop";

	/// the key listener for the new desktop is closed
	if (killProcess(pInfoKLTD) == true)
		LOG(INFO) << "Closed the key listener for the new desktop";
	else
		LOG(ERROR) << "Unable to close the key listener for the new desktop";

	/// the new explorer is closed
	/*if (killProcess(pInfoNT) == true)
		LOG(INFO) << "Closed the new explorer process";
		else
		LOG(ERROR) << "Unable to close the new explorer process";*/

	/// the tray icon is closed
	if (killProcess(pInfoTITD) == true)
		LOG(INFO) << "Closed the tray icon for the new desktop";
	else
		LOG(ERROR) << "Unable to close the tray icon for the new desktop";

	/// the tray icon is closed
	if (killProcess(pInfoTI) == true)
		LOG(INFO) << "Closed the tray icon for the new desktop";
	else
		LOG(ERROR) << "Unable to close the tray icon for the new desktop";

	LOG(INFO) << "*** LOG CLOSED ***\n";

}

/// function startProcess creates a new process and returns a handle to it
/// param startUpInfo: structure that specifies the window station, desktop, standard handles, and appearance of the main window for a process at creation time
/// param lpApplcationName: the path of the application we want to start
/// return PROCESS_INFORMATION: structure that contains information about a newly created process and its primary thread
PROCESS_INFORMATION KyoskLauncher::startProcess(STARTUPINFO startUpInfo, LPCTSTR lpApplicationName)
{
	PROCESS_INFORMATION processInformation;
	ZeroMemory(&processInformation, sizeof(processInformation)); /// this macro fills a block of memory with zeros

	if (!CreateProcess(lpApplicationName, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startUpInfo, &processInformation))
		MessageBox(0, L"Unable to start the process!\nThe path is broken!", L"Path Error!", MB_ICONERROR);
	/// Creates a new process and its primary thread. The new process runs in the security context of the calling process.

	return processInformation;
}

/// function killProcess terminates the specified process
/// param processInfo: structure that contains information about a process and its primary thread
bool KyoskLauncher::killProcess(PROCESS_INFORMATION processInfo)
{
	bool result = TerminateProcess(processInfo.hProcess, 0); /// terminates the specified process and all of its threads
	CloseHandle(processInfo.hProcess); /// Closes the process opened handle
	CloseHandle(processInfo.hThread); /// Closes the thread opened handle
	return result;
}

//// function checkAdditionalProcess returns the content of the config file
LPCWSTR KyoskLauncher::checkAdditionalProcess()
{
	char path[100];
	ifstream in("application.conf", ios::in);
	in.getline(path, 100);
	in.close();
	wchar_t wpath[101];
	mbstowcs(wpath, path, strlen(path) + 1);
	LPCWSTR lpath = wpath;
	return lpath;
}
