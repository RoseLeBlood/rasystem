#pragma once

namespace GUI
{
	class RGAPI raInstance
	{
	public:
		raInstance(HINSTANCE Instance);
		~raInstance(void);

		PROPERTY(HINSTANCE, Handler);
		GET(Handler) { return m_instance; }
		SET(Handler) { m_instance = value; }
	private:
		HINSTANCE			m_instance;
	};
}