#pragma once

	class RAPI raBillboard: public raTexturedQuad
	{
	public:
		raBillboard(raSmartPointer<raDirectX>,  LPCSTR textureFileName,
					bool bAdditive = false);

		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);
	};

	class RAPI raHimmelkoerper: public raTexturedQuad
	{
	public:
		raHimmelkoerper(raSmartPointer<raDirectX>,  LPCSTR textureFileName,
						bool bAdditive = false);

		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);
	};

	class raCrosshair: public raTexturedQuad
	{
	public:
		raCrosshair(raSmartPointer<raDirectX>,  LPCSTR textureFileName,
					bool bAdditive = false);
	};