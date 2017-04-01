#pragma once

namespace System
{
	class ViewVisArgument : public IRAArgParam
	{
	public:
		ViewVisArgument() : IRAArgParam()
		{
			OK = false;
		}
		bool Parse(std::string argument,std::string value)
		{
			if(argument == "hv")
			{
				OK = true;
			}
			return OK;
		}
		void Run(raVIS* Window)
		{
			if(Window)
			{
				Window->HideMe();
			}
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