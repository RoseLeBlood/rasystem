#pragma once
namespace System
{
	class raErrorArg : public IRAArgParam
	{
	public:
		raErrorArg() { }

		bool Parse(std::string argument,std::string value)
		{
			if(argument == GENERIC_ERROR)
			{
				raConsole::WriteLine("Allgemeiner Fehler");
				return false;
			}
			if(argument == UNKNOWN_PARAM)
			{
				raConsole::WriteLine("Unbekannter Parameter");
				return false;
			}
			if(argument == NO_VALUE_PARAM)
			{
				raConsole::WriteLine("Parameter ohne Value");
				return false;
			}

			return true;
		}

		std::string GetError()
		{
			return "";
		}
	};
}