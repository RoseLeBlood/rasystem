#pragma once

	class RAPI raTexturedMaterial : public raMaterial
	{
	public:
		raTexturedMaterial(raSmartPointer<raDirectX> dx, LPCSTR EffectFile, LPCSTR Technique,
						   LPCSTR textureFileName = "grass.jpg", LPCSTR effectVariable = "g_MeshTexture");

		virtual bool Create();
		virtual bool Destroy();
		virtual void Setup();

		void SetTexture(LPCSTR TextureFileName)		{ m_TextureFileName = TextureFileName; }
		LPCSTR GetTexture(void)						{ return m_TextureFileName; }

		void GenerateMips();
	protected:
		LPCSTR		m_TextureFileName;
		LPCSTR		m_effectVariableName;

		ID3D11ShaderResourceView*	m_pRV;
		ID3DX11EffectShaderResourceVariable* m_ptxDiffuseVariable;
	};