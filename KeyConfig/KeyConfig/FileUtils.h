#pragma once
class FileUtils
{
public:
	FileUtils();
	~FileUtils();
	char readConfigSwitch();
	char readConfigExit();
	char readConfigKeepApp();
	char readConfigDeactivateSwitch();
	void writeConfig(char keySwitch, char keyExit, char keepApps, char deactivateSwitch);
	void writeApplicationExe(CString path);
	void writeApplicationEmpty();
	char* readAppConfig();
};

