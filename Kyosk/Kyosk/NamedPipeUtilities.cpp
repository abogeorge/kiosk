#include "stdafx.h"
#include "NamedPipeUtilities.h"


NamedPipeUtilities::NamedPipeUtilities()
{
}


NamedPipeUtilities::~NamedPipeUtilities()
{
}

/// function sendMessage writes the received string to the designated pipe
/// param message: a string message that needs to be sent
void NamedPipeUtilities::sendMessage(std::string message)
{

	HANDLE pipe = CreateFile(L"\\\\.\\pipe\\TrayIconPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD numWritten;
	WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	CloseHandle(pipe); 

}