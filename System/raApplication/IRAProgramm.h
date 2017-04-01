// IRAProgramm ist eine abstracte Klasse
// die Basis eines Programmes

	class IRAProgramm
	{
	public:
		IRAProgramm() 		  { };
		~IRAProgramm()		{};

		virtual bool Create() = 0;
		virtual bool Idle() = 0;
		virtual void Destroy() = 0;
	};