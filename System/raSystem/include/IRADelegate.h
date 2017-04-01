#pragma once

	template<typename TYPE>
	class IRADelegateParam
	{
	public:
		IRADelegateParam()
		{
		}
		IRADelegateParam(raSmartPointer<TYPE> Data)
		{
			m_data.Add(Data);
		}

		READONLY_PROPERTY(unsigned long, Count);
		GET(Count) { return m_data.GetSize(); }

		void operator += (raSmartPointer<TYPE> data)
		{
			m_data.Add(data);
		}
		raSmartPointer<TYPE>* GetAt(int nIndex)
		{
			return &m_data.GetAt( (int)nIndex );
		}
	protected:
		raArray<raSmartPointer<TYPE>>			m_data;
	};

	template<class TSender, typename TYPE>
	class IRADelegate
	{
	public:
		virtual bool Equals( const IRADelegate<TSender, TYPE>* pOther) = 0;
		virtual void operator()( const TSender* pSender, IRADelegateParam<TYPE>* pParam) = 0;
		virtual void Call( const TSender* pSender, IRADelegateParam<TYPE>* pParam) = 0;
	};

	template<class TReciever, class TSender, typename TYPE>
	class raDelegate : public IRADelegate<TSender, TYPE>
	{
	private:
		typedef void (TReciever::*callFunction)(const TSender*, IRADelegateParam<TYPE>* pParam);
		callFunction         m_ptr2Func;
		TReciever*  m_ptr2Object;

	public:
		raDelegate(TReciever* p_ptr2Object, callFunction p_ptr2Func)
		{
			m_ptr2Func      = p_ptr2Func;
			m_ptr2Object    = p_ptr2Object;
		}

		bool Equals(const IRADelegate<TSender, TYPE>* pOther)
		{
			const raDelegate<TReciever, TSender, TYPE>* other;

			other = static_cast<const raDelegate<TReciever, TSender, TYPE>*>(pOther);

			assert(other != NULL);
			assert(m_ptr2Object != NULL);

			return other->m_ptr2Object == m_ptr2Object && other->m_ptr2Func == m_ptr2Func;
		}

		virtual void operator()(const TSender* pSender, IRADelegateParam<TYPE>* pParam)
		{
			assert(pSender != NULL);
			(m_ptr2Object->*m_ptr2Func)(pSender, pParam);
		}

		virtual void Call(const TSender* pSender, IRADelegateParam<TYPE>* pParam)
		{
			assert(pSender != NULL);
			(m_ptr2Object->*m_ptr2Func)(pSender, pParam);
		}
	};