#pragma once

	class RAPI raISGI : public IRAImage
	{
	public:
		raISGI(raString File) : IRAImage(File)		{ }

		~raISGI(void)
		{
			Exit();
		}

		virtual bool Create(void)
		{
			if((m_MemImage = LoadSGI(m_File)) == NULL)
				return false;
			return true;
		}
		virtual void Exit(void)
		{
			SAFE_DELETE(m_bmpi);
		}
		virtual bool Save(raString File)
		{
			return false;
		}
		int GetZSize(void)					{ return m_zSize; }
	private:
		unsigned  char* LoadSGI(raString name);

	private:
		int m_zSize;
	};