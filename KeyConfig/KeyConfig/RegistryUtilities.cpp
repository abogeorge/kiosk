#include "stdafx.h"
#include "RegistryUtilities.h"


RegistryUtilities::RegistryUtilities()
{
}


RegistryUtilities::~RegistryUtilities()
{
}

///
/// writeKeys function calls writeSingleKey with diferent params in order to write the keys in the registry
/// param keySwitch - key used for switching
/// param keyExit - key used for exiting
/// param keepApp - records application settings for application killing
/// param deactivateSwitch - records application setting for desktop switching option
bool RegistryUtilities::writeKeys(DWORD keySwitch, DWORD keyExit, DWORD keepApps, DWORD deactivateSwitch)
{
	bool resultOk = true;

	/// Writing the switch key
	HKEY regKey = HKEY_CURRENT_USER;
	LPCWSTR subKey = L"Software\\Kiosk";
	LPCWSTR valueName = L"SwitchKey";
	if (writeSingleKey(regKey, subKey, keySwitch, valueName) == false)
		resultOk = false;

	/// Writing the exit key
	valueName = L"ExitKey";
	if (writeSingleKey(regKey, subKey, keyExit, valueName) == false)
		resultOk = false;

	/// Writing the application settings for processes killing
	valueName = L"KeepApps";
	if (writeSingleKey(regKey, subKey, keepApps, valueName) == false)
		resultOk = false;

	/// Writing the application settings for desktop switching deactivation
	valueName = L"DeactivateSwitch";
	if (writeSingleKey(regKey, subKey, deactivateSwitch, valueName) == false)
		resultOk = false;

	return resultOk;
}

///
/// writeSingleKey function creates/opens a registry and inserts a value into that registry
/// param regKey: handle to an open registry key
/// param subKey: the name of a subkey that this function opens or creates
/// param value: pointer to a buffer that contains the data to be stored with the specified value name
/// param valueName: pointer to a string containing the name of the value to set
/// return bool: if the process runing this function has the necessary rights it will return true
///
bool RegistryUtilities::writeSingleKey(HKEY regKey, LPCWSTR subKey, DWORD value, LPCWSTR valueName)
{
	bool resultOk = false;
	HKEY hkey;
	DWORD dwDisposition;
	if (RegCreateKeyEx(regKey, subKey, 0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition) == ERROR_SUCCESS) {
		/// creating or opening the specified registry key
		if (RegSetValueEx(hkey, valueName, 0, REG_DWORD, (PBYTE)&value, sizeof(DWORD)) == ERROR_SUCCESS){
			/// stores data in the value field of the open registry key
			resultOk = true;
		}
		RegCloseKey(hkey);
		/// closing the opened registry key
	}
	return resultOk;
}