#include "..\include\raMain.h"

namespace System
{
	raProceduralMaterial::raProceduralMaterial(raSmartPointer<raDirectX> dx, int frequenz,
	   LPCSTR techniquename, int colorsSelection) : raMaterial(dx,
	   "raEffects\\Noise.fx", techniquename)
	{
		m_pFreqVariable = NULL;
		m_Freq = frequenz;
		m_Bias = 0.5f;

		m_pPermutationsVariable = NULL;
		CreatePermutations();

		m_pHeightTexture = NULL;
		m_pHeightTextureSRV = NULL;
		m_ptxHeight = NULL;
		m_ColorsSelection = colorsSelection;
	}

	raProceduralMaterial::raProceduralMaterial(raSmartPointer<raDirectX> dx,
		int frequenz, LPCSTR effectFilename,
		LPCSTR techniquename, int colorsSelection): raMaterial(dx,
		effectFilename, techniquename)
	{
		m_pFreqVariable = NULL;
		m_Freq = frequenz;
		m_Bias = 0.4f;

		m_pPermutationsVariable = NULL;
		CreatePermutations();

		m_pHeightTexture = NULL;
		m_pHeightTextureSRV = NULL;
		m_ptxHeight = NULL;
		m_ColorsSelection = colorsSelection;
	}

	void raProceduralMaterial::CreatePermutations()
	{
		for(int i = 0; i < 256; i++)
		{
			m_pPermutations[i] = rand() % 8;
		}

		if(m_pPermutationsVariable)
			m_pPermutationsVariable->SetIntArray(m_pPermutations, 0, 256); //muss nicht in jedem Frame neu gemacht werden
	}

	bool raProceduralMaterial::Create()
	{
		raMaterial::Create();

		m_pFreqVariable = m_pEffect->
			GetVariableByName("g_freq")->AsScalar();

		m_pBiasVariable = m_pEffect->
			GetVariableByName("g_bias")->AsScalar();

		m_pPermutationsVariable = m_pEffect->
			GetVariableByName("g_indices" )->AsScalar();
		m_pPermutationsVariable->SetIntArray(m_pPermutations, 0, 256); //muss nicht in jedem Frame neu gemacht werden

		CreateHeightTexture();
		m_ptxHeight = m_pEffect->
			GetVariableByName("g_txColorsFromHeight" )->AsShaderResource();
		m_ptxHeight->SetResource(m_pHeightTextureSRV);

		return true;
	}

	bool raProceduralMaterial::Destroy()
	{
		raMaterial::Destroy();
		SAFE_RELEASE(m_pHeightTexture);
		SAFE_RELEASE(m_pHeightTextureSRV);

		return true;
	}

	void raProceduralMaterial::Setup()
	{
		m_pFreqVariable->SetInt(m_Freq);
		m_pBiasVariable->SetFloat(m_Bias);
	}

	void raProceduralMaterial::CreateHeightTexture()
	{
		raColor data[5];
		switch(m_ColorsSelection)
		{
		case 0:
			{
				//Mond (weiss)
				data[0] = raColor(1.0f, 1.0f, 1.0f, 1.0f);
				data[1] = raColor(1.0f, 1.0f, 1.0f, 1.0f);
				data[2] = raColor(1.0f, 1.0f, 1.0f, 1.0f);
				data[3] = raColor(1.0f, 1.0f, 1.0f, 1.0f);
				data[4] = raColor(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		case 3:
			{
				//Mars
				data[0] = raColor(0.6f, 0.5f, 0.4f, 1.0f);
				data[1] = raColor(0.6f, 0.25f, 0.2f, 1.0f);
				data[2] = raColor(0.6f, 0.4f, 0.05f, 1.0f);
				data[3] = raColor(0.6f, 0.4f, 0.4f, 1.0f);
				data[4] = raColor(0.9f, 0.8f, 0.7f, 1.0f);
			}
			break;
		case 4:
			{
				//Wolken
				data[0] = raColor(0.1f, 0.0f, 0.9f, 1.0f); //hellblau
				data[1] = raColor(0.0f, 0.0f, 0.9f, 1.0f); //hellblau
				data[2] = raColor(0.9f, 0.9f, 1.0f, 1.0f); //hellblau
				data[3] = raColor(0.9f, 0.9f, 0.9f, 1.0f); //schneeweiß
				data[4] = raColor(0.7f, 0.7f, 0.7f, 1.0f); //hellgrau
			}
			break;
		default:
			{
			//Erde
			data[0] = raColor(0.0f, 0.0f, 0.8f, 1.0f); //blau
			data[1] = raColor(0.0f, 0.2f, 0.6f, 1.0f); //blau
			data[2] = raColor(0.0f, 0.5f, 0.05f, 1.0f); //grün
			data[3] = raColor(0.5f, 0.4f, 0.4f, 1.0f); //braun
			data[4] = raColor(0.9f, 0.9f, 0.9f, 1.0f); //schneeweiß
			}
			break;
		}
		D3D11_TEXTURE1D_DESC dstex;
		dstex.Width = 5;
		dstex.MipLevels = 1;
		dstex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		dstex.Usage = D3D11_USAGE_DEFAULT;
		dstex.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		dstex.CPUAccessFlags = 0;
		dstex.MiscFlags = 0;
		dstex.ArraySize = 1;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = (float*) data;
		InitData.SysMemPitch = 4 * 4 * sizeof(float);
		InitData.SysMemSlicePitch = 4 * 4 * sizeof(float);
		m_dx->GetDevice()->CreateTexture1D(&dstex, &InitData, &m_pHeightTexture);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
		SRVDesc.Format = dstex.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		SRVDesc.Texture2D.MipLevels = dstex.MipLevels;
		m_dx->GetDevice()->CreateShaderResourceView(m_pHeightTexture, &SRVDesc,
			&m_pHeightTextureSRV);
	}
};