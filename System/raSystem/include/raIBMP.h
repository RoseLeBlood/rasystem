#pragma once

	//Load a DIB/BMP file from disk.
	class RAPI raIBMP : public IRAImage
	{
	public:
		raIBMP(raString File) : IRAImage(File)		{ }

		~raIBMP(void)
		{
			Exit();
		}

		virtual bool Create(void)
		{
			if((m_MemImage = LoadDIBitmap(m_File, &m_bmpi)) == NULL)
				return false;
			return true;
		}
		virtual void Exit(void)
		{
			SAFE_DELETE(m_bmpi);
		}
		virtual bool Save(raString File)
		{
			return SaveDIBitmap(File, m_bmpi, m_MemImage);
		}

	private:
		unsigned  char* LoadDIBitmap(raString filename, BITMAPINFO **info);
		bool SaveDIBitmap(raString filename, BITMAPINFO *info, unsigned char* bits);
	};