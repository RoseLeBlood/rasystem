#pragma once

	class RAPI raSkyDome : public raEntity<VERTEXPOSITIONTEXTURED>
	{
	public:
		raSkyDome(raSmartPointer<raDirectX> dx, int width, int height, float radius = 1.0f, bool bProcedural = false,
			bool bLighted = false, raMaterial*  pMaterial = NULL);

		virtual void SetupVertices();
		virtual void SetupIndices();

		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
			const raLight *Light);

	private:
		float m_r;
		float m_width, m_height;
	};