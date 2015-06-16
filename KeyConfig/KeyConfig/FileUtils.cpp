#include "stdafx.h"
#include "FileUtils.h"


FileUtils::FileUtils()
{
}


FileUtils::~FileUtils()
{
}

/// Function readConfigSwitch, returns the stored key for desktop switching, and it's used to initialize the switch key at application start
/// return char
char FileUtils::readConfigSwitch()
{
	ifstream in("config.conf", ios::in);
	char keySwitch; in.get(keySwitch);
	in.close();
	return keySwitch;
}

/// Function readConfigExit, returns the stored key for desktop exit, and it's used to initialize the exit key at application start
/// return char
char FileUtils::readConfigExit()
{
	ifstream in("config.conf", ios::in);
	char keySwitch; in.get(keySwitch);
	char keyExit; in.get(keyExit);
	in.close();
	return keyExit;
}

/// Function writeConfig, writes to config file the keys used for desktop switching
/// param keySwitch - key used for switching
/// param keyExit - key used for exiting
void FileUtils::writeConfig(char keySwitch, char keyExit){
	ofstream out("config.conf", ios::out);
	out << keySwitch << keyExit;
}