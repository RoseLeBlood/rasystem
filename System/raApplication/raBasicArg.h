#pragma once

namespace System
{
	class raBasicArg : public IRAArgParam
	{
	public:
		raBasicArg(void) { }

		bool Parse(std::string argument,std::string value)
		{
			raConsole::Write("Argument: ");
			raConsole::Write(argument);
			raConsole::Write(" hat Value: ");
			raConsole::WriteLine(value);

			return false;
		}
		std::string GetError()
		{
			return "";
		}
	};
}