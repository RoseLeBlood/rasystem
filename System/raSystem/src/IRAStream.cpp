#include "..\include\raMain.h"

namespace raSystem
{
	bool			IRAStream::m_only2DSoftware = false;

	IRAStream::IRAStream(void)
	{
		m_paused = false;
	}

	IRAStream::~IRAStream(void)
	{
	}
}