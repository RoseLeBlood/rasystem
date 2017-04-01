#include "..\include\raMain.h"

namespace System
{
	raEffectMap raMaterial::m_effects;

	void raMaterial::CreateEffectFromCompiledFile(raString strFilename, DWORD dwShaderFlags)
	{
		HANDLE hFile;
		DWORD dwFileSize;
		DWORD dwBytesRead;

		VOID* pData = NULL;

		hFile = CreateFile(strFilename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL );
		if(hFile)
		{
			dwFileSize = GetFileSizeEx(hFile, NULL);
			if(dwFileSize != INVALID_FILE_SIZE)
			{
				pData = CoTaskMemAlloc(dwFileSize);
				if(pData)
				{
					ReadFile(hFile, pData, dwFileSize,
						&dwBytesRead, NULL);

					D3DX11CreateEffectFromMemory(pData, dwBytesRead,
					dwShaderFlags, m_dx->GetDevice(), &m_pEffect);
				}
			}
			CloseHandle(hFile);
		}
	}
	raMaterial::raMaterial(raSmartPointer<raDirectX> dx, raString EffectFile, raString Technique) :
		m_DiffuseColor(1.0f), m_AmbientColor(0.5f)
	{
		m_dx = dx;

		m_EffectFileName = EffectFile;
		m_TechniqueName = Technique;

		m_pEffect = NULL;
		m_pTechnique = NULL;

		m_bSelected = false;

		m_pMaterialDiffuseColorVariable = NULL;
		m_pMaterialAmbientVariable      = NULL;
		m_pSelectedVariable = NULL;
	}

	raMaterial::~raMaterial(void)
	{
	}
	bool raMaterial::Create()
	{
		raEffectMap::iterator iterator = m_effects.find(m_EffectFileName);
		if(iterator != m_effects.end())
		{
			m_pEffect = iterator->second;
		}
		else
		{
			DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

			//Nicht bei Verwendung von Effektgruppen
			//#if defined( DEBUG ) || defined( _DEBUG )
			// //Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
			// //Setting this flag improves the shader debugging experience, but still allows
			// //the shaders to be optimized and to run exactly the way they will run in
			// //the release configuration of this program.
			//dwShaderFlags |= D3D10_SHADER_DEBUG;
			//#endif

			ID3DBlob* pBlob = NULL;
			D3DX11CompileFromFile(m_EffectFileName.c_str(), NULL, NULL, "", "fx_5_0", dwShaderFlags, 0, NULL,
				&pBlob, NULL, NULL);
			D3DX11CreateEffectFromMemory(pBlob->GetBufferPointer(),pBlob->GetBufferSize(),
			dwShaderFlags, m_dx->GetDevice(), &m_pEffect);

			m_effects.insert(raEffectMap::value_type(m_EffectFileName, m_pEffect));
		}
		m_pEffect->AddRef();

		//Technique
		m_pTechnique = m_pEffect->GetTechniqueByName(m_TechniqueName.c_str());
		assert(m_pTechnique->IsValid());

		m_pMaterialDiffuseColorVariable =
			m_pEffect->GetVariableByName("g_MaterialDiffuseColor")->AsVector();
		m_pMaterialAmbientVariable =
			m_pEffect->GetVariableByName("g_MaterialAmbientColor")->AsVector();
		m_pSelectedVariable =
			m_pEffect->GetVariableByName("g_bSelected" )->AsScalar();

		return true;
	}
	bool raMaterial::Destroy()
	{
		if(m_pEffect) m_pEffect->Release(); //Nicht SAFE_RELEASE, da Material mehrfach genutzt sein kann
		return true;
	}
	bool raMaterial::CreateVertexLayout(
			const D3D11_INPUT_ELEMENT_DESC* pLayout,
			UINT numElements, ID3D11InputLayout** ppInputLayout)
	{
		D3DX11_PASS_DESC PassDesc;
		m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
		HRESULT hr = m_dx->GetDevice()->CreateInputLayout(pLayout, numElements,
			PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize,
			ppInputLayout);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateInputLayout", hr);
			return false;
		}

		return true;
	}
	void raMaterial::SetupPerObject(const raMatrix& mWorld,
			const raMatrix& mWorldView, const raMatrix& mWorldViewProjection)
	{
		ID3DX11EffectMatrixVariable*   m_pmWorldViewProj =
		m_pEffect->GetVariableByName("g_mWorldViewProjection")->
			AsMatrix();
		m_pmWorldViewProj->SetMatrix((float*)&mWorldViewProjection);
		m_pEffect->GetVariableByName("g_mWorldView" )->AsMatrix()->
			SetMatrix((float*)&mWorldView);
		m_pEffect->GetVariableByName("g_mWorld" )->AsMatrix()->
			SetMatrix((float*)&mWorld);
	}

	//Kann entfallen, da vom Scenemanager direkt für Effekte erledigt
	void raMaterial::SetupPerFrame(const raLight* pLight,
			const raMatrix* pView, const raMatrix* pProj)
	{
		D3DXVECTOR3 lightDir = pLight->m_Direction;
		m_pEffect->GetVariableByName("g_vLightDir")->AsVector()->
			SetFloatVector(lightDir);
		raColor lightDiffuseColor = pLight->m_DiffuseColor;
		m_pEffect->GetVariableByName("g_LightDiffuse")->AsVector()->
			SetFloatVector((float*)&lightDiffuseColor);

		//Für Glanzlichter (geht aber auch ohne (mit WorldView und WorldViewProjection))
		//außerdem für SkinnedInstancing, Proj auch für Cubemapping
		if(pView)
		{
			ID3DX11EffectMatrixVariable* pmView =
				m_pEffect->GetVariableByName("g_mView")->AsMatrix();
			if(pmView)pmView->SetMatrix((float*)pView);
		}
		if(pProj)
		{
			ID3DX11EffectMatrixVariable* pmProj =
				m_pEffect->GetVariableByName("g_mProj")->AsMatrix();
			if(pmProj)pmProj->SetMatrix((float*)pProj);
		}
	}
	void raMaterial::Setup()
	{
		if(m_pMaterialDiffuseColorVariable)
			m_pMaterialDiffuseColorVariable->SetFloatVector(m_DiffuseColor);
		if(m_pMaterialAmbientVariable)
			m_pMaterialAmbientVariable->SetFloatVector(m_AmbientColor);
		if(m_pSelectedVariable)
			m_pSelectedVariable->SetBool(m_bSelected);
	}
};