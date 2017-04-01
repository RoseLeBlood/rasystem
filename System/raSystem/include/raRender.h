#pragma once

	class RAPI raRender
	{
		friend bool operator == (const raRender& a, const raRender& b);
	public:
		raRender(raSmartPointer<raDirectX> dx, const raMatrix* mWorld = NULL, LPCSTR Name = "Render");

		virtual ~raRender(void) {};

		virtual bool Create() { return false; }
		virtual void Destroy() { };

		virtual bool Render(UINT drawOrder, const raMatrix& pView, const raMatrix& pProj,
			const raLight* pLight, LPCSTR techniqueName = "") {return true; }

		virtual bool Update(float fTime, float fRunTime){ return true; }

		LPCSTR GetName()	{ return m_Name; }
		void   SetName(LPCSTR Name) { m_Name = Name; }

		virtual void SetVisible (bool bVisible) { }
		virtual void SetSelected(bool bSelected) {};
		virtual void SetWorldMatrix(const raMatrix& value);
		virtual raMatrix* GetWorldMatrix(void);

		virtual const float GetBoundingsphereRadius()		{ return 1.0f; }
		virtual bool		Intersects(const raVector3* pRayPos,
									   const raVector3* pRayDir,
									   float* pDist) { return false; };

	protected:
		raSmartPointer<raDirectX>		m_dx;
		raMatrix						m_World;
		LPCSTR							m_Name;
	};
	inline bool operator == (const raRender& a, const raRender& b)						{ return ((a.m_Name == b.m_Name) && (a.m_World == b.m_World)); }