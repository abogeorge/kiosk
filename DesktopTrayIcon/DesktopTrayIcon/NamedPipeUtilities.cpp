#include "stdafx.h"
#include "NamedPipeUtilities.h"


NamedPipeUtilities::NamedPipeUtilities()
{
}


NamedPipeUtilities::~NamedPipeUtilities()
{
}

/// function getPipeMessage acces the given pipe to check for inbound messages, reads them and based on the message content
/// sends a message to either display a message or close the tray icon
void NamedPipeUtilities::getPipeMessage()
{
	char data[1024];
	DWORD numRead;
	bool stillActive = true;

	while (true)
	{
		HANDLE pipe = CreateNamedPipe(L"\\\\.\\pipe\\TrayIconPipe", PIPE_ACCESS_INBOUND | PIPE_ACCESS_OUTBOUND, PIPE_WAIT, 1, 1024, 1024, 120 * 1000, NULL);
		ConnectNamedPipe(pipe, NULL);
		ReadFile(pipe, data, 1024, &numRead, NULL);

		if (numRead > 0)
		{
			if (data[0] == '1')
			{
				SendMessage(hWnd, WM_TRAYMESSAGE, 0, 1);
			}
			if (data[0] == '2')
			{
				SendMessage(hWnd, WM_TRAYMESSAGE, 0, 2);
			}
			if (data[0] == '3')
			{
				SendMessage(hWnd, WM_TRAYMESSAGE, 0, 3);
			}
			CloseHandle(pipe);
		}

	}
}
