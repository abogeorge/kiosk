#pragma once
class RegistryUtilities
{
public:
	RegistryUtilities();
	~RegistryUtilities();
	bool writeKeys(DWORD keySwitch, DWORD keyExit, DWORD keepApps, DWORD deactivateSwitch);
private:
	bool writeSingleKey(HKEY regKey, LPCWSTR subKey, DWORD value, LPCWSTR valueName);
};
