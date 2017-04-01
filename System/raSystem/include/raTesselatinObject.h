#pragma once

	class RAPI raTesselatinObject : public raEntity<VERTEXPOSITION, false>
	{
	public:
		raTesselatinObject(raDirectX *dx);
		virtual ~raTesselatinObject()	{};

		virtual void SetupVertices();
		virtual bool RenderMesh(LPCSTR techniqueName = "");

		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);

		void SetTessellationsFactor(float tessellationFactor)
			{ m_TesselationsFactor = tessellationFactor; }

	private:
		float m_TesselationsFactor;
	};