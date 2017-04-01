#pragma once

	class raTexture
	{
	public:
		char   cSource[MAX_PATH]; // Texturen aus Memory == ""
		UINT Width;
		UINT Height;
		UINT MipLevels;
		UINT MiscFlags;
		D3D11_USAGE Usage11;
		DXGI_FORMAT Format;
		UINT CpuAccessFlags;
		UINT BindFlags;
		ID3D11ShaderResourceView* pSRV11;

		raTexture() { pSRV11 = NULL; }
	};

	class RAPI raResource
	{
	public:
		~raResource();
		// Texturen aus Datein--------------------------------------------------------------------------------------
		HRESULT                 CreateTextureFromFile( raSmartPointer<raDirectX> dx, raString pSrcFile,
													   ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false )
		{
			return CreateTextureFromFileEx( dx, pSrcFile, NULL, NULL, ppOutputRV, bSRGB );
		}
		HRESULT                 CreateTextureFromFileEx( raSmartPointer<raDirectX> dx, raString pSrcFile,
														 D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
														 ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false );
		// Texturen aus Memory---------------------------------------------------------------------------------------
		HRESULT                 CreateTextureFromMemory( raSmartPointer<raDirectX> dx, LPVOID Data, size_t Size,
													     ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false )
		{
			return CreateTextureFromMemoryEx( dx, Data, Size, NULL, NULL, ppOutputRV, bSRGB );
		}

		HRESULT                 CreateTextureFromMemoryEx( raSmartPointer<raDirectX> dx, LPVOID Data, size_t Size,
														   D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
														   ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false );
		// Texturen aus Resourcen---------------------------------------------------------------------------------------
		HRESULT                 CreateTextureFromResource( raSmartPointer<raDirectX> dx, HMODULE hSrcModule, LPCSTR pSrcResource,
													       ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false )
		{
			return CreateTextureFromResourceEx(dx, hSrcModule, pSrcResource, NULL, NULL, ppOutputRV, bSRGB);
		}

		HRESULT                 CreateTextureFromResourceEx( raSmartPointer<raDirectX> dx, HMODULE hSrcModule, LPCSTR pSrcResource,
														     D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
														     ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false );

		static raResource& Instance();
	public:
		HRESULT                 OnLostDevice();
		HRESULT                 OnDestroyDevice();

	protected:
		raArray <raTexture>		m_TextureCache;
		raResource();
	};