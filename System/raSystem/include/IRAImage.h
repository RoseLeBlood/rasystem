#pragma once

	class RAPI IRAImage
	{
	public:
		IRAImage(raString File);

		virtual ~IRAImage(void);

		virtual bool Create(void) = 0;
		virtual void Exit(void)	= 0;
		virtual bool Save(raString File) = 0;

		unsigned  char* GetImage(void)			{ return m_MemImage; }
		int			   GetImageSize(void)		{ return m_imagesize; }
	protected:
		unsigned int			m_imagesize;
		unsigned  char*			m_MemImage;
		raString				m_File;
		BITMAPINFO				*m_bmpi;
	};