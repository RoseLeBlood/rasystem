	class RAPI raMinimap : public raTexturedQuad
	{
	public:
		raMinimap(raSmartPointer<raDirectX> dx, LPCSTR playerIconFile, bool bAlpha = false);

		virtual void CreateEffectVariables();
		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);

		virtual void Destroy();

	private:
		LPCSTR									m_PlayerIcon;
		ID3D11ShaderResourceView*				m_pRVPlayerIconTexture;
		ID3DX11EffectShaderResourceVariable*	m_ptxPlayerIconTextureVariable;
	};