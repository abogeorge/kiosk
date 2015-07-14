#include "stdafx.h"
#include "RegistryUtilities.h"


RegistryUtilities::RegistryUtilities()
{
}


RegistryUtilities::~RegistryUtilities()
{
}

///
/// changeAllFeatures function calls changeSingleFeature with diferent params in order to enable/disable the menu features
/// param value: contains the data to be stored with the specified value name
///
bool RegistryUtilities::changeAllFeatures(DWORD value)
{
	bool resultOk = true;
	/// Modifying Task Manager
	HKEY regKey = HKEY_CURRENT_USER;
	LPCWSTR subKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
	LPCWSTR valueName = L"DisableTaskMgr";
	if (changeSingleFeature(regKey, subKey, value, valueName) == false)
		resultOk = false;

	/// Modifying Change Password
	valueName = L"DisableChangePassword";
	if (changeSingleFeature(regKey, subKey, value, valueName) == false)
		resultOk = false;

	/// Modifying Lock
	valueName = L"DisableLockWorkstation";
	if (changeSingleFeature(regKey, subKey, value, valueName) == false)
		resultOk = false;

	/// Modifying Shut Down
	subKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
	valueName = L"NoClose";
	if (changeSingleFeature(regKey, subKey, value, valueName) == false)
		resultOk = false;

	/// Modifying Logoff
	valueName = L"NoLogOff";
	if (changeSingleFeature(regKey, subKey, value, valueName) == false)
		resultOk = false;


	/// Modifying StartMenuLogoff
	valueName = L"StartMenuLogOff";
	if (changeSingleFeature(regKey, subKey, value, valueName) == false)
		resultOk = false;

	/// Modifying User Switching
	regKey = HKEY_LOCAL_MACHINE;
	subKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
	valueName = L"HideFastUserSwitching";
	if (changeSingleFeature(regKey, subKey, value, valueName) == false)
		resultOk = false;

	return resultOk;
}

///
/// changeSingleFeature function creates/opens a registry and inserts a value into that registry
/// param regKey: handle to an open registry key
/// param subKey: the name of a subkey that this function opens or creates
/// param value: pointer to a buffer that contains the data to be stored with the specified value name
/// param valueName: pointer to a string containing the name of the value to set
/// return bool: if the process runing this function has the necessary rights it will return true
///
bool RegistryUtilities::changeSingleFeature(HKEY regKey, LPCWSTR subKey, DWORD value, LPCWSTR valueName)
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

/// lockStatus function queries the registry to check if the user requested a lock on the second desktop
bool RegistryUtilities::lockStatus()
{
	HKEY hkey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Kiosk", 0, KEY_READ, &hkey) == ERROR_SUCCESS)
	{
		DWORD type;
		DWORD cbData;
		if (RegQueryValueEx(hkey, L"DeactivateSwitch", NULL, &type, NULL, &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
		}
		wstring value(cbData / sizeof(wchar_t), L'\0');
		if (RegQueryValueEx(hkey, L"DeactivateSwitch", NULL, NULL, reinterpret_cast<LPBYTE>(&value[0]), &cbData) != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			throw "Could not read registry value";
		}

		RegCloseKey(hkey);

		size_t firstNull = value.find_first_of(L'\0');
		if (firstNull != string::npos)
			value.resize(firstNull);

		if (value[0] == '\x1'){
			return true;
		}

	}

	return false;
}