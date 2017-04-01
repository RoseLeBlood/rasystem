#pragma once

	class RAPI raRFMaterial : public raRTTMaterial
	{
	public:
		raRFMaterial(raSmartPointer<raDirectX>,
			LPCSTR techniqueName = "Reflection", //Kann geändert werden in Reflection, Refraction oder Disperiosn
			int envMapWidth = 128, int envMapHeight = 128);

		virtual bool Create();
		virtual void Setup();

		void SetEtaRatio(float etaRatio)			{ m_r = etaRatio; }
		void SetFresnelPower(float pow)				{ m_pow = pow; }
		void SetReflectivity(float refelectivity)	{ m_reflectivity = refelectivity; }

	private:
		float m_r;
		float m_pow;
		float m_reflectivity;

		ID3DX11EffectScalarVariable*         m_pR;
		ID3DX11EffectVectorVariable*         m_pR3;
		ID3DX11EffectScalarVariable*         m_pFres0;
		ID3DX11EffectScalarVariable*         m_pFresPow;
		ID3DX11EffectScalarVariable*         m_pRefl;
	};