#include "..\include\raMain.h"
namespace System
{
	raBillboard::raBillboard(raSmartPointer<raDirectX> dx,  LPCSTR textureFileName,  bool bAdditive):
	raTexturedQuad(dx, new raTexturedMaterial(dx.get(),
		"raEffects\\NoNormals.fx", "RenderWithTexture", textureFileName), true)
	{
		m_bAdditive = bAdditive;
	}

	void raBillboard::SetupEffectVariables(const raMatrix& pView,
			const raMatrix& pProj, const raLight* praLight)
	{
		raMatrix worldTmp;
		D3DXMatrixInverse((D3DXMATRIX*)&worldTmp, NULL, (D3DXMATRIX*)&pView);

		//Skalierung aus der Original-World-Matrix (legt die Größe des Objekts fest)
		raMatrix ms;
		ms = raMatrixScaling(raVector3(m_World.m11, m_World.m22, m_World.m33));
		worldTmp = ms * worldTmp;

		//konstante Position im Welt-Koordinatensystem
		worldTmp.m41 = m_World.m41;
		worldTmp.m42 = m_World.m42;
		worldTmp.m43 = m_World.m43;

		raMatrix mWorldView, mWorldViewProjection;
		mWorldView = worldTmp * pView;
		mWorldViewProjection = mWorldView * pProj;

		for(DWORD matNo = 0; matNo < m_numMaterials; matNo++)
		{
			m_pMaterials[matNo]->SetupPerObject(worldTmp, mWorldView,
				mWorldViewProjection);
		}
	}

	raHimmelkoerper::raHimmelkoerper(raSmartPointer<raDirectX> dx,  LPCSTR textureFileName,  bool bAdditive):
	raTexturedQuad(dx, new raTexturedMaterial(dx.get(),
		"raEffects\\MyEngine.fx",
		"RenderWithTexture", textureFileName), true)
	{
		m_bAdditive = bAdditive;
	}

	void raHimmelkoerper::SetupEffectVariables(const raMatrix& pView,
		const raMatrix& pProj, const raLight* praLight)
	{
		raMatrix worldTmp;
		D3DXMatrixInverse((D3DXMATRIX*)&worldTmp, NULL, (D3DXMATRIX*)&pView);

		//Skalierung aus der Original-World-Matrix (legt die Größe des Objekts fest)
		raMatrix ms;
		ms = raMatrixScaling(raVector3(m_World.m11, m_World.m22, m_World.m33));
		worldTmp = ms * worldTmp;

		//konstante Position im Kamera-Koordinatensystem
		//Das entspricht der Multiplikation unten
		float zf =  pProj.m43/ (1 - pProj.m33) ;
		float scale = 0.04f * zf;

		worldTmp.m41 += scale * m_World.m41;
		worldTmp.m42 += scale * m_World.m42;
		worldTmp.m43 += scale * m_World.m43;

		//Jetzt könnte man noch von rechts eine Sklalierung anmultiplizieren,
		//um die Entfernung vom Beobachter zu skalieren

		raMatrix mWorldView, mWorldViewProjection;
		mWorldView = worldTmp * pView;
		mWorldViewProjection = mWorldView * pProj;

		//Für die Sonne Entferung zum Betrachter konstant halten
		raMatrix mScale;
		mScale = raMatrixScaling(raVector3(scale, scale, scale));

	   /* raMatrix mInvView;
		D3DXMatrixInverse((D3DXMATRIX*)&mInvView, NULL, (D3DXMATRIX*)&pView);
		raMatrix worldSkyBox(mScale * (*pWorld));
		worldSkyBox.m41 = mInvView.m41;
		worldSkyBox.m42 = mInvView.m42 - scale * 0.2f;
		worldSkyBox.m43 = mInvView.m43;

		raMatrix world(worldTmp * worldSkyBox);*/

		//m_pmWorldViewProj->SetMatrix((float*)&mWorldViewProjection);
		//m_pmWorld->SetMatrix((float*)&worldTmp);

		for(DWORD matNo = 0; matNo < m_numMaterials; matNo++)
		{
			//if(pView)
			m_pMaterials[matNo]->SetupPerFrame(praLight);
			m_pMaterials[matNo]->SetupPerObject(worldTmp, mWorldView,
				mWorldViewProjection);
		}

		//View-Matrix wird aber vom Scenemanager gesetzt
		//Sollte aber nichts machen, da die hauptsächlich für Transformation der
		//Normalen in den View Space genutzt wird (Richtungslicht per Pixel)
	}

	raCrosshair::raCrosshair(raSmartPointer<raDirectX> dx,  LPCSTR textureFileName,  bool bAdditive):
		raTexturedQuad(dx, new raTexturedMaterial(dx.get(),
			"raEffects\\MyEngine.fx",
			"RenderWithTextureCenterScreen", textureFileName), true)
	{
		m_bAdditive = bAdditive;
	}
};