#include "..\raSystem\include\raMain.h"
#include "raApplication.h"

using namespace System;

void raCmdParser::SetDefaultCommand(IRAArgParam* obj)
{
	//SAFE_DELETE(m_defaultCommand);
	m_defaultCommand = obj;
}
void raCmdParser::AddEmptyCommand(std::string argument,IRAArgParam* obj)
{
	m_mapEmptyCommand[argument] = obj;
}
void raCmdParser::AddValueCommand(std::string argument,IRAArgParam* obj)
{
	m_mapValueCommand[argument] = obj;
}
void raCmdParser::SetErrorCommand(IRAArgParam* obj)
{
	//SAFE_DELETE(m_errorCommand);
	m_errorCommand = obj;
}

bool raCmdParser::Parse()
{
	bool bAllOK = true;

	if(cmd == NULL) return false;

	for(int i = 0; i < cmd->GetSize(); i++)
	{
		std::string argument = cmd->GetAt(i);

		if(DeleteMinus(argument))
		{
			std::map<std::string,IRAArgParam*>::iterator it = m_mapValueCommand.find(argument);

			if(it != m_mapValueCommand.end())
			{
				if(cmd->GetAt(i + 1) == "")
				{
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				std::string paso = cmd->GetAt(i + 1);
				if(DeleteMinus(paso))
				{
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				else
				{
					bAllOK &= it->second->Parse(argument,paso);
					i++;
				}
			}
			else
			{
				it = m_mapEmptyCommand.find(argument);
				if(it != m_mapEmptyCommand.end())
				{
					bAllOK &= it->second->Parse(argument,"");
				}
				else
				{
					std::string::size_type position = argument.find_first_of(":");
					if(position != std::string::npos)
					{
						std::string command = argument.substr(0,position);
						std::string value = argument.substr(position + 1);
						std::map<std::string,IRAArgParam*>::iterator it = m_mapValueCommand.find(command);
						if(it != m_mapValueCommand.end())
						{
							bAllOK &= it->second->Parse(command,value);
						}
						else
						{
							bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,command);
							return false;
						}
					}
					else
					{
						bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,argument);
						return false;
					}
				}
			}
		}
		else
		{
			if(m_defaultCommand != NULL)
			{
				bAllOK &= m_defaultCommand->Parse("",argument);
			}
			else
			{
				return false;
			}
		}
		i++;
	}

	std::string error = m_defaultCommand->GetError();
	if(error != "")
	{
		m_errorCommand->Parse(GENERIC_ERROR,error);
		return false;
	}

	std::map<std::string,IRAArgParam*>::iterator it = m_mapValueCommand.begin();

	while(it != m_mapValueCommand.end())
	{
		error = it->second->GetError();
		if(error != "")
		{
			m_errorCommand->Parse(GENERIC_ERROR,error);
			return false;
		}
		it++;
	}

	std::map<std::string,IRAArgParam*>::iterator it2 = m_mapEmptyCommand.begin();

	while(it2 != m_mapEmptyCommand.end())
	{
		error = it2->second->GetError();
		if(error != "")
		{
			m_errorCommand->Parse(GENERIC_ERROR,error);
			return false;
		}
		it2++;
	}

	return bAllOK;
}

bool raCmdParser::DeleteMinus(std::string& argument)
{
	switch(argument[0])
	{
	case '/':
		argument = &(argument.c_str()[1]);
		return true;
	case '-':
		if(argument[1] == '-')
		{
			argument = &(argument.c_str()[2]);
		}
		else
		{
			argument = &(argument.c_str()[1]);
		}
		return true;
	}

	return false;
}