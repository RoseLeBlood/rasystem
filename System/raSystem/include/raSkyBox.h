#pragma once

	class RAPI raSkyBox : public raEntity<CUBEMAPVERTEX>
	{
	public:
		raSkyBox(raSmartPointer<raDirectX> dx, raMaterial* pMaterial);
		raSkyBox(raSmartPointer<raDirectX> dx, LPCSTR texFileName = "raGrafik\\SkyBox.dds");

		virtual void SetupVertices();
		virtual void SetupIndices();

		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);
	};