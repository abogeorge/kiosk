#pragma once
class NamedPipeUtilities
{
public:
	NamedPipeUtilities();
	~NamedPipeUtilities();
	void sendMessage(std::string message);
};

