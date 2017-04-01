#include "..\include\raMain.h"

namespace System
{
	DXGI_FORMAT MAKE_SRGB( DXGI_FORMAT format )
	{
		switch( format )
		{
			case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_R8G8B8A8_UINT:
			case DXGI_FORMAT_R8G8B8A8_SNORM:
			case DXGI_FORMAT_R8G8B8A8_SINT:
				return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

			case DXGI_FORMAT_BC1_TYPELESS:
			case DXGI_FORMAT_BC1_UNORM:
				return DXGI_FORMAT_BC1_UNORM_SRGB;
			case DXGI_FORMAT_BC2_TYPELESS:
			case DXGI_FORMAT_BC2_UNORM:
				return DXGI_FORMAT_BC2_UNORM_SRGB;
			case DXGI_FORMAT_BC3_TYPELESS:
			case DXGI_FORMAT_BC3_UNORM:
				return DXGI_FORMAT_BC3_UNORM_SRGB;
		};

		return format;
	}
	raResource::raResource(void)
	{
		memset(this, 0, sizeof(raResource));
	}

	raResource::~raResource(void)
	{
		m_TextureCache.RemoveAll();
	}
	raResource& raResource::Instance()
	{
		static raResource instanz;
		return instanz;
	}
	HRESULT raResource::CreateTextureFromFileEx( raSmartPointer<raDirectX> dx, raString pSrcFile,
												 D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
												 ID3D11ShaderResourceView** ppOutputRV, bool bSRGB )
	{
		HRESULT hr = S_OK;
		D3DX11_IMAGE_LOAD_INFO ZeroInfo;	//D3DX11_IMAGE_LOAD_INFO has a default constructor
		D3DX11_IMAGE_INFO SrcInfo;

		if( !pLoadInfo )
		{
			pLoadInfo = &ZeroInfo;
		}

		if( !pLoadInfo->pSrcInfo )
		{
			D3DX11GetImageInfoFromFile( pSrcFile.c_str(), NULL, &SrcInfo, NULL );
			pLoadInfo->pSrcInfo = &SrcInfo;

			pLoadInfo->Format = pLoadInfo->pSrcInfo->Format;
		}
		for(int i=0; i < m_TextureCache.GetSize(); i++)
		{
			if( !strcmp( m_TextureCache[i].cSource, pSrcFile.c_str() ) &&
				m_TextureCache[i].Width == pLoadInfo->Width &&
				m_TextureCache[i].Height == pLoadInfo->Height &&
				m_TextureCache[i].MipLevels == pLoadInfo->MipLevels &&
				m_TextureCache[i].Usage11 == pLoadInfo->Usage &&
				m_TextureCache[i].Format == pLoadInfo->Format &&
				m_TextureCache[i].CpuAccessFlags == pLoadInfo->CpuAccessFlags &&
				m_TextureCache[i].BindFlags == pLoadInfo->BindFlags &&
				m_TextureCache[i].MiscFlags == pLoadInfo->MiscFlags )
			{
				return m_TextureCache[i].pSRV11->QueryInterface( __uuidof( ID3D11ShaderResourceView ), ( LPVOID* )ppOutputRV );
			}
		}

		raTexture NewEntry;
		strcpy_s( NewEntry.cSource, MAX_PATH, pSrcFile.c_str() );
		NewEntry.Width = pLoadInfo->Width;
		NewEntry.Height = pLoadInfo->Height;
		NewEntry.MipLevels = pLoadInfo->MipLevels;
		NewEntry.Usage11 = pLoadInfo->Usage;
		NewEntry.Format = pLoadInfo->Format;

		NewEntry.CpuAccessFlags = pLoadInfo->CpuAccessFlags;
		NewEntry.BindFlags = pLoadInfo->BindFlags;
		NewEntry.MiscFlags = pLoadInfo->MiscFlags;

		//Create the rexture
		ID3D11Texture2D* pRes = NULL;
		hr = D3DX11CreateTextureFromFile( dx->GetDevice(), pSrcFile.c_str(), pLoadInfo, pPump, ( ID3D11Resource** )&pRes, NULL );

		if( FAILED( hr ) )
			return hr;
		D3D11_TEXTURE2D_DESC tex_dsc;
		pRes->GetDesc(&tex_dsc);

		if (bSRGB )
		{
			ID3D11Texture2D* unormStaging = NULL;
			ID3D11Texture2D* srgbStaging = NULL;

			D3D11_TEXTURE2D_DESC CopyDesc;
			pRes->GetDesc( &CopyDesc );

			pLoadInfo->BindFlags = 0;
			pLoadInfo->CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			pLoadInfo->Depth = 0;
			pLoadInfo->Filter = D3DX11_FILTER_LINEAR;
			pLoadInfo->FirstMipLevel = 0;
			pLoadInfo->Format = CopyDesc.Format;
			pLoadInfo->Height = CopyDesc.Height;
			pLoadInfo->MipFilter = D3DX11_FILTER_LINEAR;
			pLoadInfo->MiscFlags = CopyDesc.MiscFlags;
			pLoadInfo->Usage = D3D11_USAGE_STAGING;
			pLoadInfo->Width = CopyDesc.Width;

			CopyDesc.BindFlags = 0;
			CopyDesc.Usage = D3D11_USAGE_STAGING;
			CopyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			CopyDesc.Format = MAKE_SRGB(CopyDesc.Format);

			hr = D3DX11CreateTextureFromFile( dx->GetDevice(), pSrcFile.c_str(), pLoadInfo, pPump, ( ID3D11Resource** )&unormStaging, NULL );

			hr = dx->GetDevice()->CreateTexture2D(&CopyDesc, NULL, &srgbStaging);
			dx->GetImmediateContext()->CopyResource( srgbStaging, unormStaging );
			ID3D11Texture2D* srgbGPU;

			pRes->GetDesc( &CopyDesc );
			CopyDesc.Format = MAKE_SRGB(CopyDesc.Format);
			hr = dx->GetDevice()->CreateTexture2D(&CopyDesc, NULL, &srgbGPU);
			dx->GetImmediateContext()->CopyResource( srgbGPU, srgbStaging );

			SAFE_RELEASE(pRes);
			SAFE_RELEASE(srgbStaging);
			SAFE_RELEASE(unormStaging);
			pRes = srgbGPU;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		if( bSRGB )
			SRVDesc.Format = MAKE_SRGB( ZeroInfo.Format );
		else
			SRVDesc.Format = ZeroInfo.Format;
		if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE1D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			SRVDesc.Texture1D.MostDetailedMip = 0;
			SRVDesc.Texture1D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
		}
		else if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;

			if( pLoadInfo->pSrcInfo->MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE )
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				SRVDesc.TextureCube.MostDetailedMip = 0;
				SRVDesc.TextureCube.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
			}
		}
		else if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture3D.MostDetailedMip = 0;
			SRVDesc.Texture3D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
		}
		if (bSRGB) {
			SRVDesc.Format = MAKE_SRGB(tex_dsc.Format);
		}else {
			SRVDesc.Format = tex_dsc.Format;
		}
		SRVDesc.Texture2D.MipLevels = tex_dsc.MipLevels;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		hr = dx->GetDevice()->CreateShaderResourceView( pRes, &SRVDesc, ppOutputRV );
		pRes->Release();
		if( FAILED( hr ) )
			return hr;

		( *ppOutputRV )->QueryInterface( __uuidof( ID3D11ShaderResourceView ), ( LPVOID* )&NewEntry.pSRV11 );

		m_TextureCache.Add(NewEntry);

		return S_OK;
	}
	HRESULT raResource::CreateTextureFromMemoryEx( raSmartPointer<raDirectX> dx, LPVOID Data, size_t Size,
												   D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
												   ID3D11ShaderResourceView** ppOutputRV, bool bSRGB)
	{
		HRESULT hr = S_OK;
		D3DX11_IMAGE_LOAD_INFO ZeroInfo;	//D3DX11_IMAGE_LOAD_INFO has a default constructor
		D3DX11_IMAGE_INFO SrcInfo;

		if( !pLoadInfo )
		{
			pLoadInfo = &ZeroInfo;
		}

		if( !pLoadInfo->pSrcInfo )
		{
			D3DX11GetImageInfoFromMemory( Data, Size, NULL, &SrcInfo, NULL );
			pLoadInfo->pSrcInfo = &SrcInfo;

			pLoadInfo->Format = pLoadInfo->pSrcInfo->Format;
		}
		for(int i=0; i < m_TextureCache.GetSize(); i++)
		{
			if( !strcmp( m_TextureCache[i].cSource, "" ) &&
				m_TextureCache[i].Width == pLoadInfo->Width &&
				m_TextureCache[i].Height == pLoadInfo->Height &&
				m_TextureCache[i].MipLevels == pLoadInfo->MipLevels &&
				m_TextureCache[i].Usage11 == pLoadInfo->Usage &&
				m_TextureCache[i].Format == pLoadInfo->Format &&
				m_TextureCache[i].CpuAccessFlags == pLoadInfo->CpuAccessFlags &&
				m_TextureCache[i].BindFlags == pLoadInfo->BindFlags &&
				m_TextureCache[i].MiscFlags == pLoadInfo->MiscFlags )
			{
				// A match is found. Obtain the IDirect3DTexture9 interface and return that.
				return m_TextureCache[i].pSRV11->QueryInterface( __uuidof( ID3D11ShaderResourceView ), ( LPVOID* )ppOutputRV );
			}
		}

		raTexture NewEntry;
		strcpy_s( NewEntry.cSource, MAX_PATH, "" );
		NewEntry.Width = pLoadInfo->Width;
		NewEntry.Height = pLoadInfo->Height;
		NewEntry.MipLevels = pLoadInfo->MipLevels;
		NewEntry.Usage11 = pLoadInfo->Usage;
		NewEntry.Format = pLoadInfo->Format;

		NewEntry.CpuAccessFlags = pLoadInfo->CpuAccessFlags;
		NewEntry.BindFlags = pLoadInfo->BindFlags;
		NewEntry.MiscFlags = pLoadInfo->MiscFlags;

		//Create the rexture
		ID3D11Texture2D* pRes = NULL;
		hr = D3DX11CreateTextureFromMemory( dx->GetDevice(), Data, Size, pLoadInfo, pPump, ( ID3D11Resource** )&pRes, NULL );

		if( FAILED( hr ) )
			return hr;
		D3D11_TEXTURE2D_DESC tex_dsc;
		pRes->GetDesc(&tex_dsc);

		if (bSRGB )
		{
			ID3D11Texture2D* unormStaging = NULL;
			ID3D11Texture2D* srgbStaging = NULL;

			D3D11_TEXTURE2D_DESC CopyDesc;
			pRes->GetDesc( &CopyDesc );

			pLoadInfo->BindFlags = 0;
			pLoadInfo->CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			pLoadInfo->Depth = 0;
			pLoadInfo->Filter = D3DX11_FILTER_LINEAR;
			pLoadInfo->FirstMipLevel = 0;
			pLoadInfo->Format = CopyDesc.Format;
			pLoadInfo->Height = CopyDesc.Height;
			pLoadInfo->MipFilter = D3DX11_FILTER_LINEAR;
			pLoadInfo->MiscFlags = CopyDesc.MiscFlags;
			pLoadInfo->Usage = D3D11_USAGE_STAGING;
			pLoadInfo->Width = CopyDesc.Width;

			CopyDesc.BindFlags = 0;
			CopyDesc.Usage = D3D11_USAGE_STAGING;
			CopyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			CopyDesc.Format = MAKE_SRGB(CopyDesc.Format);

			hr = D3DX11CreateTextureFromMemory( dx->GetDevice(), Data, Size, pLoadInfo, pPump, ( ID3D11Resource** )&unormStaging, NULL );

			hr = dx->GetDevice()->CreateTexture2D(&CopyDesc, NULL, &srgbStaging);
			dx->GetImmediateContext()->CopyResource( srgbStaging, unormStaging );
			ID3D11Texture2D* srgbGPU;

			pRes->GetDesc( &CopyDesc );
			CopyDesc.Format = MAKE_SRGB(CopyDesc.Format);
			hr = dx->GetDevice()->CreateTexture2D(&CopyDesc, NULL, &srgbGPU);
			dx->GetImmediateContext()->CopyResource( srgbGPU, srgbStaging );

			SAFE_RELEASE(pRes);
			SAFE_RELEASE(srgbStaging);
			SAFE_RELEASE(unormStaging);
			pRes = srgbGPU;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		if( bSRGB )
			SRVDesc.Format = MAKE_SRGB( ZeroInfo.Format );
		else
			SRVDesc.Format = ZeroInfo.Format;
		if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE1D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			SRVDesc.Texture1D.MostDetailedMip = 0;
			SRVDesc.Texture1D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
		}
		else if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;

			if( pLoadInfo->pSrcInfo->MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE )
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				SRVDesc.TextureCube.MostDetailedMip = 0;
				SRVDesc.TextureCube.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
			}
		}
		else if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture3D.MostDetailedMip = 0;
			SRVDesc.Texture3D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
		}
		if (bSRGB) {
			SRVDesc.Format = MAKE_SRGB(tex_dsc.Format);
		}else {
			SRVDesc.Format = tex_dsc.Format;
		}
		SRVDesc.Texture2D.MipLevels = tex_dsc.MipLevels;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		hr = dx->GetDevice()->CreateShaderResourceView( pRes, &SRVDesc, ppOutputRV );
		pRes->Release();
		if( FAILED( hr ) )
			return hr;

		( *ppOutputRV )->QueryInterface( __uuidof( ID3D11ShaderResourceView ), ( LPVOID* )&NewEntry.pSRV11 );

		m_TextureCache.Add(NewEntry);

		return S_OK;
	}
	HRESULT raResource::CreateTextureFromResourceEx( raSmartPointer<raDirectX> dx, HMODULE hSrcModule, LPCSTR pSrcResource,
													 D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
													 ID3D11ShaderResourceView** ppOutputRV, bool bSRGB)
	{
		HRESULT hr = S_OK;
		D3DX11_IMAGE_LOAD_INFO ZeroInfo;	//D3DX11_IMAGE_LOAD_INFO has a default constructor
		D3DX11_IMAGE_INFO SrcInfo;

		if( !pLoadInfo )
		{
			pLoadInfo = &ZeroInfo;
		}

		if( !pLoadInfo->pSrcInfo )
		{
			D3DX11GetImageInfoFromResource( hSrcModule, pSrcResource, NULL, &SrcInfo, NULL );
			pLoadInfo->pSrcInfo = &SrcInfo;

			pLoadInfo->Format = pLoadInfo->pSrcInfo->Format;
		}
		for(int i=0; i < m_TextureCache.GetSize(); i++)
		{
			if( !strcmp( m_TextureCache[i].cSource, pSrcResource ) &&
				m_TextureCache[i].Width == pLoadInfo->Width &&
				m_TextureCache[i].Height == pLoadInfo->Height &&
				m_TextureCache[i].MipLevels == pLoadInfo->MipLevels &&
				m_TextureCache[i].Usage11 == pLoadInfo->Usage &&
				m_TextureCache[i].Format == pLoadInfo->Format &&
				m_TextureCache[i].CpuAccessFlags == pLoadInfo->CpuAccessFlags &&
				m_TextureCache[i].BindFlags == pLoadInfo->BindFlags &&
				m_TextureCache[i].MiscFlags == pLoadInfo->MiscFlags )
			{
				// A match is found. Obtain the IDirect3DTexture9 interface and return that.
				return m_TextureCache[i].pSRV11->QueryInterface( __uuidof( ID3D11ShaderResourceView ), ( LPVOID* )ppOutputRV );
			}
		}

		raTexture NewEntry;
		strcpy_s( NewEntry.cSource, MAX_PATH, pSrcResource );
		NewEntry.Width = pLoadInfo->Width;
		NewEntry.Height = pLoadInfo->Height;
		NewEntry.MipLevels = pLoadInfo->MipLevels;
		NewEntry.Usage11 = pLoadInfo->Usage;
		NewEntry.Format = pLoadInfo->Format;

		NewEntry.CpuAccessFlags = pLoadInfo->CpuAccessFlags;
		NewEntry.BindFlags = pLoadInfo->BindFlags;
		NewEntry.MiscFlags = pLoadInfo->MiscFlags;

		//Create the rexture
		ID3D11Texture2D* pRes = NULL;
		hr = D3DX11CreateTextureFromResource( dx->GetDevice(), hSrcModule, pSrcResource, pLoadInfo, pPump, ( ID3D11Resource** )&pRes, NULL );

		if( FAILED( hr ) )
			return hr;
		D3D11_TEXTURE2D_DESC tex_dsc;
		pRes->GetDesc(&tex_dsc);

		if (bSRGB )
		{
			ID3D11Texture2D* unormStaging = NULL;
			ID3D11Texture2D* srgbStaging = NULL;

			D3D11_TEXTURE2D_DESC CopyDesc;
			pRes->GetDesc( &CopyDesc );

			pLoadInfo->BindFlags = 0;
			pLoadInfo->CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			pLoadInfo->Depth = 0;
			pLoadInfo->Filter = D3DX11_FILTER_LINEAR;
			pLoadInfo->FirstMipLevel = 0;
			pLoadInfo->Format = CopyDesc.Format;
			pLoadInfo->Height = CopyDesc.Height;
			pLoadInfo->MipFilter = D3DX11_FILTER_LINEAR;
			pLoadInfo->MiscFlags = CopyDesc.MiscFlags;
			pLoadInfo->Usage = D3D11_USAGE_STAGING;
			pLoadInfo->Width = CopyDesc.Width;

			CopyDesc.BindFlags = 0;
			CopyDesc.Usage = D3D11_USAGE_STAGING;
			CopyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			CopyDesc.Format = MAKE_SRGB(CopyDesc.Format);

			hr = D3DX11CreateTextureFromResource( dx->GetDevice(), hSrcModule, pSrcResource, pLoadInfo, pPump, ( ID3D11Resource** )&unormStaging, NULL );

			hr = dx->GetDevice()->CreateTexture2D(&CopyDesc, NULL, &srgbStaging);
			dx->GetImmediateContext()->CopyResource( srgbStaging, unormStaging );
			ID3D11Texture2D* srgbGPU;

			pRes->GetDesc( &CopyDesc );
			CopyDesc.Format = MAKE_SRGB(CopyDesc.Format);
			hr = dx->GetDevice()->CreateTexture2D(&CopyDesc, NULL, &srgbGPU);
			dx->GetImmediateContext()->CopyResource( srgbGPU, srgbStaging );

			SAFE_RELEASE(pRes);
			SAFE_RELEASE(srgbStaging);
			SAFE_RELEASE(unormStaging);
			pRes = srgbGPU;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		if( bSRGB )
			SRVDesc.Format = MAKE_SRGB( ZeroInfo.Format );
		else
			SRVDesc.Format = ZeroInfo.Format;
		if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE1D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			SRVDesc.Texture1D.MostDetailedMip = 0;
			SRVDesc.Texture1D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
		}
		else if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;

			if( pLoadInfo->pSrcInfo->MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE )
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				SRVDesc.TextureCube.MostDetailedMip = 0;
				SRVDesc.TextureCube.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
			}
		}
		else if( pLoadInfo->pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D )
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture3D.MostDetailedMip = 0;
			SRVDesc.Texture3D.MipLevels = pLoadInfo->pSrcInfo->MipLevels;
		}
		if (bSRGB) {
			SRVDesc.Format = MAKE_SRGB(tex_dsc.Format);
		}else {
			SRVDesc.Format = tex_dsc.Format;
		}
		SRVDesc.Texture2D.MipLevels = tex_dsc.MipLevels;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		hr = dx->GetDevice()->CreateShaderResourceView( pRes, &SRVDesc, ppOutputRV );
		pRes->Release();
		if( FAILED( hr ) )
			return hr;

		( *ppOutputRV )->QueryInterface( __uuidof( ID3D11ShaderResourceView ), ( LPVOID* )&NewEntry.pSRV11 );

		m_TextureCache.Add(NewEntry);

		return S_OK;
	}
};