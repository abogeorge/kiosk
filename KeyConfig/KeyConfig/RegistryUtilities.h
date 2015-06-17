#pragma once
class RegistryUtilities
{
public:
	RegistryUtilities();
	~RegistryUtilities();
	bool writeKeys(DWORD keySwitch, DWORD keyExit);
private:
	bool writeSingleKey(HKEY regKey, LPCWSTR subKey, DWORD value, LPCWSTR valueName);
};

