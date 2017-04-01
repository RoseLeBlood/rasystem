#include "..\include\raMain.h"

namespace System
{
RAPI raMatrix raMatrixTranslation(const raVector3& v)
{
	return raMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					v.x,  v.y,  v.z,  1.0f);
}
RAPI raMatrix raMatrixRotationX(const float f)
{
	raMatrix mResult;

	// Rotationsmatrix berechnen
	mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
	mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m22 = mResult.m33 = cosf(f);
	mResult.m23 = sinf(f);
	mResult.m32 = -mResult.m23;

	return mResult;
}
RAPI raMatrix raMatrixRotationY(const float f)
{
	raMatrix mResult;

	                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
	mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m33 = cosf(f);
	mResult.m31 = sinf(f);
	mResult.m13 = -mResult.m31;

	return mResult;
}
RAPI raMatrix raMatrixRotationZ(const float f)
{
	raMatrix mResult;

	// Rotationsmatrix berechnen
	                                        mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	                                        mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m22 = cosf(f);
	mResult.m12 = sinf(f);
	mResult.m21 = -mResult.m12;

	return mResult;
}
RAPI raMatrix raMatrixRotation(const float x,
									  const float y,
									  const float z)
{
	return raMatrixRotationZ(z) *
		   raMatrixRotationX(x) *
		   raMatrixRotationY(y);
}
RAPI raMatrix raMatrixRotation(const raVector3& v)
{
	return raMatrixRotationZ(v.z) *
		   raMatrixRotationX(v.x) *
		   raMatrixRotationY(v.y);
}

RAPI raMatrix raMatrixRotationAxis(const raVector3& v,
										  const float f)
{
	const float fSin = sinf(-f);
	const float fCos = cosf(-f);
	const float fOneMinusCos = 1.0f - fCos;

	const raVector3 vAxis(raVector3Normalize(v));

	return raMatrix((float)(vAxis.x * vAxis.x) * fOneMinusCos + fCos,
		            (float)(vAxis.x * vAxis.y) * fOneMinusCos - ((float)vAxis.z * fSin),
				    (float)(vAxis.x * vAxis.z) * fOneMinusCos + ((float)vAxis.y * fSin),
					0.0f,
					(float)(vAxis.y * vAxis.x) * fOneMinusCos + ((float)vAxis.z * fSin),
					(float)(vAxis.y * vAxis.y) * fOneMinusCos + fCos,
					(float)(vAxis.y * vAxis.z) * fOneMinusCos - ((float)vAxis.x * fSin),
					0.0f,
					(float)(vAxis.z * vAxis.x) * fOneMinusCos - ((float)vAxis.y * fSin),
					(float)(vAxis.z * vAxis.y) * fOneMinusCos + ((float)vAxis.x * fSin),
					(float)(vAxis.z * vAxis.z) * fOneMinusCos + fCos,
					0.0f,
					0.0f,
					0.0f,
					0.0f,
					1.0f);
}
RAPI raMatrix raMatrixScaling(const raVector3& v)
{
	return raMatrix(v.x,  0.0f, 0.0f, 0.0f,
		            0.0f, v.y,  0.0f, 0.0f,
					0.0f, 0.0f, v.z,  0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}
RAPI raMatrix raMatrixAxes(const raVector3& vXAxis,
								  const raVector3& vYAxis,
								  const raVector3& vZAxis)
{
	return raMatrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		            vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
					vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}
RAPI float raMatrixDet(const raMatrix& m)
{
	return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
           m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
           m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}
RAPI raMatrix raMatrixInvert(const raMatrix& m)
{
	float fInvDet = raMatrixDet(m);
	if(fInvDet == 0.0f) return raMatrixIdentity();
	fInvDet = 1.0f / fInvDet;

	raMatrix mResult;
	mResult.m11 =  fInvDet * (m.m22 * m.m33 - m.m23 * m.m32);
	mResult.m12 = -fInvDet * (m.m12 * m.m33 - m.m13 * m.m32);
	mResult.m13 =  fInvDet * (m.m12 * m.m23 - m.m13 * m.m22);
	mResult.m14 =  0.0f;
	mResult.m21 = -fInvDet * (m.m21 * m.m33 - m.m23 * m.m31);
	mResult.m22 =  fInvDet * (m.m11 * m.m33 - m.m13 * m.m31);
	mResult.m23 = -fInvDet * (m.m11 * m.m23 - m.m13 * m.m21);
	mResult.m24 =  0.0f;
	mResult.m31 =  fInvDet * (m.m21 * m.m32 - m.m22 * m.m31);
	mResult.m32 = -fInvDet * (m.m11 * m.m32 - m.m12 * m.m31);
	mResult.m33 =  fInvDet * (m.m11 * m.m22 - m.m12 * m.m21);
	mResult.m34 =  0.0f;
	mResult.m41 = -(m.m41 * mResult.m11 + m.m42 * mResult.m21 + m.m43 * mResult.m31);
	mResult.m42 = -(m.m41 * mResult.m12 + m.m42 * mResult.m22 + m.m43 * mResult.m32);
	mResult.m43 = -(m.m41 * mResult.m13 + m.m42 * mResult.m23 + m.m43 * mResult.m33);
	mResult.m44 =  1.0f;

	return mResult;
}
RAPI raMatrix raMatrixTranspose(const raMatrix& m)
{
	// Matrix transponieren
	return raMatrix(m.m11, m.m21, m.m31, m.m41,
		            m.m12, m.m22, m.m32, m.m42,
					m.m13, m.m23, m.m33, m.m43,
					m.m14, m.m24, m.m34, m.m44);
}
RAPI raMatrix raMatrixProjection(const float fFOV,
										const float fAspect,
										const float fNearPlane,
										const float fFarPlane)
{
	const float s = 1.0f / tanf(fFOV * 0.5f);
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return raMatrix(s / fAspect, 0.0f, 0.0f, 0.0f,
		            0.0f, s, 0.0f, 0.0f,
					0.0f, 0.0f, Q, 1.0f,
					0.0f, 0.0f, -Q * fNearPlane, 0.0f);
}

RAPI raMatrix raMatrixCamera(const raVector3& vPos,
									const raVector3& vLookAt,
									const raVector3& vUp)
{
	raVector3 vZAxis(raVector3Normalize(vLookAt - vPos));

	raVector3 vXAxis(raVector3Normalize(raVector3Cross(vUp, vZAxis)));

	raVector3 vYAxis(raVector3Normalize(raVector3Cross(vZAxis, vXAxis)));
	return raMatrixTranslation(-vPos) *
	       raMatrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		            vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
				    vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}
RAPI raMatrix raMatrixToTex2DMatrix(const raMatrix& m)
{
	return raMatrix(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);
}
};