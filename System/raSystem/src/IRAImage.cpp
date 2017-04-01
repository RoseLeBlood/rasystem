#include "..\include\raMain.h"

namespace System
{
	IRAImage::IRAImage(raString File)
	{
		m_File = File.c_str();
	}

	IRAImage::~IRAImage(void)
	{
	}
};