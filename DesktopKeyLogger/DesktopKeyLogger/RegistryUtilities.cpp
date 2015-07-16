#include "stdafx.h"
#include "RegistryUtilities.h"

char letters[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

RegistryUtilities::RegistryUtilities()
{
}


RegistryUtilities::~RegistryUtilities()
{
}


char RegistryUtilities::getKeySwitch()
{

	HKEY hkey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Kiosk", 0, KEY_READ, &hkey) == ERROR_SUCCESS){
		DWORD type;
		DWORD cbData;
		if (RegQueryValueEx(hkey, L"SwitchKey", NULL, &type, NULL, &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
		}
		wstring value(cbData / sizeof(wchar_t), L'\0');
		if (RegQueryValueEx(hkey, L"SwitchKey", NULL, NULL, reinterpret_cast<LPBYTE>(&value[0]), &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			throw "Could not read registry value";
		}

		RegCloseKey(hkey);

		size_t firstNull = value.find_first_of(L'\0');
		if (firstNull != string::npos)
		{
			value.resize(firstNull);
		}

		return letters[value[0]];
	}

	return NULL;
}


char RegistryUtilities::getKeyExit()
{
	HKEY hkey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Kiosk", 0, KEY_READ, &hkey) == ERROR_SUCCESS){
		DWORD type;
		DWORD cbData;
		if (RegQueryValueEx(hkey, L"ExitKey", NULL, &type, NULL, &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
		}
		wstring value(cbData / sizeof(wchar_t), L'\0');
		if (RegQueryValueEx(hkey, L"ExitKey", NULL, NULL, reinterpret_cast<LPBYTE>(&value[0]), &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			throw "Could not read registry value";
		}

		RegCloseKey(hkey);

		size_t firstNull = value.find_first_of(L'\0');
		if (firstNull != string::npos)
		{
			value.resize(firstNull);
		}

		return letters[value[0]];
	}

	return NULL;

}
