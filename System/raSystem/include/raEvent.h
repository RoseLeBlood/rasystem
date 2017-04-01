	template<class TSender, typename TYPE>
	class raEvent
	{
	public:
		raEvent() { }
		~raEvent();

		void Call( const TSender* pSender, IRADelegateParam<TYPE>* pParameter);

		void operator += ( const IRADelegate<TSender, TYPE>* pHandler);
		void operator -= ( const IRADelegate<TSender, TYPE>* pHandler);
		void operator () ( const TSender* pSender, IRADelegateParam<TYPE>* pParameter);
	private:
		void Register( const IRADelegate<TSender, TYPE>* pHandler);
		void Unregister( const IRADelegate<TSender, TYPE>* pHandler);
	private:
		std::list< IRADelegate<TSender, TYPE>* > m_observers;
	};

	template<class TSender, typename TYPE>
	void raEvent<TSender, TYPE>::operator += (const IRADelegate<TSender, TYPE>* pHandler)
	{
		Register(pHandler);
	}

	template<class TSender, typename TYPE>
	void raEvent<TSender, TYPE>::operator -= (const IRADelegate<TSender, TYPE>* pHandler)
	{
		Unregister(pHandler);
	}

	template<class TSender, typename TYPE>
	void raEvent<TSender, TYPE>::operator ()(const TSender* pSender, IRADelegateParam<TYPE>* pParameter)
	{
		Call(pSender, pParameter);
	}

	template<class TSender, typename TYPE>
	void raEvent<TSender, TYPE>::Register(const IRADelegate<TSender, TYPE>* pHandler)
	{
		assert(pHandler != NULL);

		for(std::list< IRADelegate<TSender, TYPE>* >::iterator itr = m_observers.begin();
		   itr != m_observers.end();
		   itr++)
		{
			if((*itr)->Equals(pHandler))
				return;
		}

		m_observers.push_back(const_cast<IRADelegate<TSender, TYPE>*>(pHandler));
	}

	template<class TSender, typename TYPE>
	void raEvent<TSender, TYPE>::Unregister( const IRADelegate<TSender, TYPE>* pHandler)
	{
		assert(pHandler != NULL);

		std::vector< IRADelegate<TSender, TYPE>* >::iterator where;
		for(list< IRADelegate<TSender, TYPE>* >::iterator itr = m_observers.begin();
			itr != m_observers.end();
			itr++)
		{
			if((*itr)->Equals(pHandler))
			{
				where = itr;
				break;
			}
		}

		m_observers.erase(where);
	}

	template<class TSender, typename TYPE>
	void raEvent<TSender, TYPE>::Call(const TSender* pSender, IRADelegateParam<TYPE>* pParameter)
	{
		for(std::list< IRADelegate<TSender, TYPE>* >::iterator itr = m_observers.begin();
			itr != m_observers.end();
			itr++)
		{
			(*itr)->Call(pSender, pParameter);
		}
	}

	template<class TSender, typename TYPE>
	raEvent<TSender, TYPE>::~raEvent()
	{
		for(std::list< IRADelegate<TSender, TYPE>* >::iterator itr = m_observers.begin();
			itr != m_observers.end();
			itr++)
		{
			assert(*itr != NULL);
			delete (*itr);
		}
	}