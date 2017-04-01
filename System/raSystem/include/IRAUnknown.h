#pragma once

	class RAPI IRAUnknown
	{
	public:
		IRAUnknown()
		{
			m_count = 0;
		}
		void AddRef()
		{
			m_count++;
		}
		int Release()
		{
			return --m_count;
		}

		static bool CreateInstance(IRAUnknown** output)
		{
			if(*output != NULL) return false;

			IRAUnknown *mtemp = new IRAUnknown();

			if(output)
			{
				mtemp->AddRef();
				memcpy(*output, mtemp, sizeof(mtemp));
				return true;
			}
			return false;
		}
		READONLY_PROPERTY(int, Count);
		GET(Count)
		{
			return m_count;
		};
	private:
		int		m_count;
	};