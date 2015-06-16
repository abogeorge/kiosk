#pragma once
class RegistryUtilities
{
public:
	RegistryUtilities();
	~RegistryUtilities();
	bool changeAllFeatures(DWORD value);
private:
	bool changeSingleFeature(HKEY regKey, LPCWSTR subKey, DWORD value, LPCWSTR valueName);
};

//#pragma once
//class RegistryUtilities
//{
//public:
//	RegistryUtilities();
//  ~RegistryUtilities();
//	bool enableAllFeatures(DWORD value);
//	bool disableAllFeatures(DWORD value);
//private:
//	bool enableSingleFeature(HKEY regKey, LPCWSTR subKey, LPCWSTR valueName);
//	bool disableSingleFeature(HKEY regKey, LPCWSTR subKey, DWORD value, LPCWSTR valueName);
//};
