#pragma once

	class RAPI raThreadManager : public IRASingleton<raThreadManager>
	{
		friend class IRASingleton<raThreadManager>;
	public:
		~raThreadManager(void);

	protected:
		raThreadManager();
	};