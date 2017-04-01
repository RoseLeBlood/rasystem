#include "..\include\raMain.h"

namespace System
{
	raRender::raRender(raSmartPointer<raDirectX> dx, const raMatrix* mWorld, LPCSTR Name) : m_Name(Name)
	{
		m_dx = dx;

		if(mWorld) m_World = *mWorld;
		else m_World = raMatrixIdentity();
	}

	void raRender::SetWorldMatrix(const raMatrix& value){ m_World = value; }
	raMatrix* raRender::GetWorldMatrix(void) { return &m_World; }
};