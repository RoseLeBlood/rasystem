#pragma once

	// CellularMaterial
	class RAPI raCMaterial : public raMaterial
	{
	public:
		raCMaterial(raSmartPointer<raDirectX> dx);
		~raCMaterial(void);

		virtual bool Create();
        void makeCoefficients();

        virtual void Setup();

	private:
        static const int nPoints = 16;

        float m_pFeaturePoints[3 * nPoints]; //x, y, z
        float m_Coefficients[4];

        ID3DX11EffectVectorVariable* m_pFeaturePointsVariable;
        ID3DX11EffectScalarVariable* m_pCoefficientsVariable;
	};