#pragma once
class NamedPipeUtilities
{
public:
	NamedPipeUtilities();
	~NamedPipeUtilities();
	void getPipeMessage();
};

extern  HWND hWnd;

#define WM_TRAYMESSAGE (WM_USER + 1)

