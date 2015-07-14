#pragma once
class RegistryUtilities
{
public:
	RegistryUtilities();
	~RegistryUtilities();
	bool changeAllFeatures(DWORD value);
	bool lockStatus();
private:
	bool changeSingleFeature(HKEY regKey, LPCWSTR subKey, DWORD value, LPCWSTR valueName);
};
