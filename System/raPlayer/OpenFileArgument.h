#pragma once

namespace System
{
	class OpenFileArgument : public IRAArgParam
	{
	public:
		OpenFileArgument() : IRAArgParam()
		{
			OK = false;
		}
		bool Parse(std::string argument,std::string value)
		{
			raConsole::WriteLine(value);
			m_file = value;
			OK = (System::Util::raFile::raFileExists(value)) ? true : false;

			return OK;
		}
		void Run(raMainWindow* Window)
		{
			if(Window)
			{
				Window->Playliste->Clear();
				Window->Playliste->SetData<raString>(Window->Playliste->AddEntry(Util::raFile::raRemoveDir(m_file)), m_file);
				Window->Playliste->SetSelectedIndex(0);
			}
		}
		std::string GetError()
		{
			return "";
		}
		PROPERTY(raString, Data);
		GET(Data)
		{
			return m_file;
		}
		SET(Data)
		{
			m_file = value;
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
		raString m_file;
		bool m_ok;
	};
}