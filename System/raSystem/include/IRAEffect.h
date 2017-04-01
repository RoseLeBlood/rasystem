#pragma once

namespace raSystem
{
	class RAPI IRAEffect
	{
	public:
		IRAEffect(void);
		~IRAEffect(void);

		virtual bool Create(IRASoundSystem *System)
		{
			m_system = System;
			return true;
		}
		virtual void Destroy() {};

		virtual bool Start() {return false;};
		virtual bool Stop() {return false;};

	protected:
		IRASoundSystem			*m_system;
	};
}