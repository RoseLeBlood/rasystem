#pragma once

namespace System
{
	class ShowConsole : public IRAArgParam
	{
	public:
		ShowConsole() : IRAArgParam()
		{
			OK = false;
		}
		bool Parse(std::string argument,std::string value)
		{
			OK = true;

			return true;
		}
		void Run()
		{
			raConsole::Show();
			raConsole::SetTitel("ramp");
			raConsole::SetTextColor(COLOR_YELLOW, COLOR_BLACK);
			raConsole::WriteLine("ramp --- Ramona Player Debug Console");
			raConsole::SetTextColor(COLOR_WHITE, COLOR_BLACK);
		}
		std::string GetError()
		{
			return "";
		}
		PROPERTY(bool, OK);
		GET(OK)
		{
			return m_ok;
		}
		SET(OK)
		{
			m_ok = value;
		}
	private:
		bool m_ok;
	};
}