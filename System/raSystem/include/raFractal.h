#pragma once

	class RAPI raFractal : public raEntity<VERTEXPOSITIONCOLOR, false>
	{
	public:
		raFractal(raSmartPointer<raDirectX> dx, int Level, bool bRandom = true, raMaterial* mat = (NULL));

		virtual void SetupVertices();

		float m_max_angle;
		float m_max_lenght;

	private:
		bool m_bRandom;
		int	 m_level;

		void makeChildren(VERTEXPOSITIONCOLOR* vertices,
			raVector3 v0, raVector3 v1, int& n, int level);
	};