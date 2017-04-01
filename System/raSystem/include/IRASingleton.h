#pragma once

template<typename TYPE> class IRASingleton
{
	//friend class IRASingleton<TYPE>;
public:
	static TYPE& Instance()
	{
		if(m_instance == NULL)
		{
			m_sec.Create();
			RACSECTION(&m_sec)
			{
				CreateInstance();
			}
			m_sec.Destroy();
		}
		return *m_instance;
	}
protected:
	IRASingleton()
	{
	}
private:
	static void CreateInstance()
	{
		m_instance = new TYPE();
	}
private:
	static TYPE* m_instance;
	static System::raCriticalSection m_sec;
};
template<typename TYPE> TYPE* IRASingleton<TYPE>::m_instance = NULL;
template<typename TYPE> System::raCriticalSection  IRASingleton<TYPE>::m_sec;