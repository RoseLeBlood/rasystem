#pragma once

class System::raMatrix;

	class RAPI raLight
	{
	public:
		struct
		{
			raVector3 m_Position;
			raVector3 m_Direction;
			raColor	  m_DiffuseColor;
			raColor   m_AmbientColor;

			//PointLight
			raVector4 m_Attenuation;

			//SpotLight
			float m_cosHalfPhi;
			float m_cosHalfTheta;
		};

		raLight();
		~raLight();

		raMatrix GetViewMatrix() const;
		raMatrix GetProjectionMatrix() const;

		virtual void SetProjParams(float fFov, float fAspect, float fNearPlane, float fFarPlane);
	protected:
		raLight(const GUID& raguid);

	private:
		raMatrix	m_mProj;
		float		m_fFOV;
		float		m_fAspect;
		float		m_fNearPlane;
		float		m_fFarPlane;
	};