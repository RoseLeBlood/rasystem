#pragma once

namespace System
{
#define UNKNOWN_PARAM "UNKNOWN_PARAM"
#define NO_VALUE_PARAM "NO_VALUE_PARAM"
#define GENERIC_ERROR "GENERIC_ERROR"

	class IRAArgParam
	{
	public:
		virtual bool Parse(std::string argument,std::string value) = 0;
		virtual std::string GetError() = 0;
	};
	class raCmdParser
	{
	public:
		raCmdParser(){m_defaultCommand = NULL;}

		void AddEmptyCommand(std::string argument,IRAArgParam* e);
		void AddValueCommand(std::string argument,IRAArgParam* e);

		bool Parse();

		WRITEONLY_PROPERTY(IRAArgParam*, DefaultCommand);
		SET(DefaultCommand)
		{
			SetDefaultCommand(value);
		}
		WRITEONLY_PROPERTY(IRAArgParam*, ErrorCommand);
		SET(ErrorCommand)
		{
			SetErrorCommand(value);
		}
		WRITEONLY_PROPERTY(System::raArray<raString> *, CommandLine);
		SET(CommandLine)
		{
			cmd = value;
		}
	protected:
		void SetDefaultCommand(IRAArgParam* e);
		void SetErrorCommand(IRAArgParam* e);
	private:
		IRAArgParam* m_defaultCommand;
		IRAArgParam* m_errorCommand;
		std::map<std::string,IRAArgParam*> m_mapValueCommand;
		std::map<std::string,IRAArgParam*> m_mapEmptyCommand;

		bool DeleteMinus(std::string& param);

		System::raArray<raString> *cmd;
	};
}