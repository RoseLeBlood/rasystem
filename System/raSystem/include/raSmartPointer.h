#pragma once

	template<class T> class raSmartPointer
	{
	public:
		raSmartPointer() throw()
		{
			m_data = NULL;
			m_rc = new IRAUnknown();
			m_rc->AddRef();
		}
		raSmartPointer(T *pVal) throw()
		{
			m_data = pVal;
			m_rc = new IRAUnknown();
			m_rc->AddRef();
		}
		raSmartPointer(const raSmartPointer<T>& ptrCopy) throw()
		{
			m_data = ptrCopy.m_data;
			m_rc = ptrCopy.m_rc;

			m_rc->AddRef();
		}
		raSmartPointer<T>& operator = (const raSmartPointer<T>& sp) throw()
		{
			if (this != &sp)
			{
				if(m_rc->Release() == 0)
				{
					delete m_data;
					delete m_rc;
				}
				m_data = sp.m_data;
				m_rc = sp.m_rc;
				m_rc->AddRef();
			}
			return *this;
		}
		~raSmartPointer(void)
		{
			if(m_rc->Release() == 0)
			{
				delete m_data;
				delete m_rc;
			}
		}
		T& operator*() const throw()
		{
			return *m_data;
		}

		T *operator->() const throw()
		{
			return m_data;
		}

		T *get() const throw()
		{
			return m_data;
		}
		PROPERTY(T*, Data);
		GET(Data)
		{
			return m_data;
		}
		SET(Data)
		{
			m_data = value;
			m_rc->AddRef();
		}
	private:
		T				*m_data;
		IRAUnknown*		m_rc;
	};