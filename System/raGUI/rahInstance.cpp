#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raInstance::raInstance(HINSTANCE Instance)
	{
		m_instance = Instance;
	}

	raInstance::~raInstance(void)
	{
	}
}
}