#pragma once

	class RAPI raProceduralMaterial: public raMaterial
	{
	public:
		raProceduralMaterial(raSmartPointer<raDirectX> dx, int frequenz,
			LPCSTR techniquename, int colorsSelection = 5);
		raProceduralMaterial(raSmartPointer<raDirectX> dx, int frequenz,
			LPCSTR effectFilename, LPCSTR techniquename, int colorsSelection = 5);

		virtual bool Create();
		virtual bool Destroy();

		virtual void Setup();

		void SetFrequenz(int frequenz){m_Freq = frequenz;}
		void SetBias(float bias){m_Bias = bias;}

		void CreatePermutations();

	protected:
		virtual void CreateHeightTexture();
	protected:
		int			m_pPermutations[256];
		int			m_Freq;
		int			m_ColorsSelection;
		float		m_Bias;

		ID3DX11EffectScalarVariable*			m_pPermutationsVariable;
		ID3DX11EffectShaderResourceVariable*	m_ptxHeight;
		ID3DX11EffectScalarVariable*			m_pFreqVariable;
		ID3DX11EffectScalarVariable*			m_pBiasVariable;
		ID3D11Texture1D*						m_pHeightTexture;
		ID3D11ShaderResourceView*				m_pHeightTextureSRV;
	};