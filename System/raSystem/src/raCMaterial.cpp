#include "..\include\raMain.h"

namespace System
{
	raCMaterial::raCMaterial(raSmartPointer<raDirectX> dx) : raMaterial(dx, ".\\Effects\\Cellular.fx", "Cellular")
	{
		srand((unsigned int)time(NULL));

        m_pFeaturePointsVariable = NULL;
        for(int i = 0; i < 3 * nPoints; i++)
        {
            if(i % 3 == 2)
                m_pFeaturePoints[i] = 0; // z = 0 für flache Objekte (TexturedQuad)
            else
                m_pFeaturePoints[i] = raGetRandFloat(-1.0f, 1.0f);
        }

        m_pCoefficientsVariable = NULL;
        m_Coefficients[3] = 0;
        m_Coefficients[2] = 0;
        m_Coefficients[1] = 1;
        m_Coefficients[0] = -1;
	}
	bool raCMaterial::Create()
    {
        raMaterial::Create();

        m_pFeaturePointsVariable = m_pEffect->
            GetVariableByName("g_FeaturePoints" )->AsVector();
        m_pFeaturePointsVariable->SetFloatVectorArray(m_pFeaturePoints, 0, nPoints); //muss nicht in jedem Frame neu gemacht werden

        m_pCoefficientsVariable = m_pEffect->
            GetVariableByName("g_Coefficients")->AsScalar();

		return true;
    }

    void raCMaterial::Setup()
    {
        m_pCoefficientsVariable->SetFloatArray(m_Coefficients, 0, 4);
    }

    void raCMaterial::makeCoefficients()
    {
        m_Coefficients[3] = raGetRandFloat(0.0f, 1.0f);
        m_Coefficients[2] = raGetRandFloat(-m_Coefficients[3], 1.0f);
        m_Coefficients[1] = raGetRandFloat(-m_Coefficients[3]-m_Coefficients[2], 1.0f);
        m_Coefficients[0] = raGetRandFloat(-m_Coefficients[3]-m_Coefficients[2]-m_Coefficients[1], 1.0f);
    }

	raCMaterial::~raCMaterial(void)
	{
	}
};