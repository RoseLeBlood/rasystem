#include "..\include\raMain.h"

namespace System
{
static float factorial(float p)
{
	if ( p == 0.0f || p == 1.0f ) return 1.0f;
	else if ( p < 0.0f ) return 0.0f;

	float f = 1.0f ;

	while( p > 1.0f )
	{
		f *= p ;
		p-- ;
	}

	return f;
}
raQuaternion::raQuaternion(const raVector3 &angles)
{
	float cos_z_2 = cosf(0.5f*(float)angles.z);
	float cos_y_2 = cosf(0.5f*(float)angles.y);
	float cos_x_2 = cosf(0.5f*(float)angles.x);

	float sin_z_2 = sinf(0.5f*(float)angles.z);
	float sin_y_2 = sinf(0.5f*(float)angles.y);
	float sin_x_2 = sinf(0.5f*(float)angles.x);

	// and now compute quaternion
	s   = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
	v.x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
	v.y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
	v.z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
}
raQuaternion::raQuaternion(float fs, float fx, float fy, float fz)
{
	s = fs;
	v = raVector3(fx, fy, fz);
}
raQuaternion::raQuaternion(float *pfs)
{
	s = pfs[0];
	v = raVector3(pfs[1], pfs[2], pfs[3]);
}
raQuaternion::raQuaternion(float Angle, raVector3 Axis)
{
	(* this) = raQuaternionFromAxis(Angle, Axis);
}
raQuaternion::raQuaternion(raVector3 Axis, float Angle)
{
	(* this) = raQuaternionFromAxis(Angle, Axis);
}
raQuaternion::raQuaternion(raMatrix mat)
{
      float tmp = fabs(mat(1,1) + mat(2,2) + mat(3,3) + 1);
      s = 0.5f*sqrt(tmp);

      if(s > 0.0000001f)
      {
		  v.x = (mat.n[9]-mat.n[6])/(4*s);
          v.y = (mat.n[2]-mat.n[8])/(4*s);
          v.z = (mat.n[4]-mat.n[1])/(4*s);
      }
      else
      {
         // |w| <= 1/2
         static int s_iNext[3] = { 2, 3, 1 };
         int i = 1;
         if ( mat(2,2) > mat(1,1) )
            i = 2;
         if ( mat(3,3) > mat(2,2) )
            i = 3;
         int j = s_iNext[i-1];
         int k = s_iNext[j-1];

         float fRoot = sqrt(mat(i,i)-mat(j,j)-mat(k,k) + 1.0f);

		 float *tmp[3] = { (float*)&v.x, (float*)&v.y, (float*)&v.z };
         *tmp[i-1] = 0.5f*fRoot;
         fRoot = 0.5f/fRoot;
         s = (mat(k,j)-mat(j,k))*fRoot;
         *tmp[j-1] = (mat(j,i)+mat(i,j))*fRoot;
         *tmp[k-1] = (mat(k,i)+mat(i,k))*fRoot;
	  }
}
raQuaternion& raQuaternion::operator *= (const raQuaternion& q)
{
	*this = *this * q;
	return *this;
}
raQuaternion& raQuaternion::operator /= (const raQuaternion& q)
{
	raQuaternion temp = q;
	(* this) *= raQuaternionInvert(temp);
    return *this;
}
raQuaternion& raQuaternion::operator /= (const float f)
{
	s/=f; v.x/=f; v.y/=f; v.z/=f; return *this;
}
raQuaternion& raQuaternion::operator *= (const float f)
{
	s*=f; v.x*=f; v.y*=f; v.z*=f; return *this;
}
raVector3 raQuaternion::GetRotationAxis() const
{
	float sinThetaOver2Sq = 1.0f - s*s;

    if (sinThetaOver2Sq <= 0.0f)
	{
        return raVector3(1.0f, 0.0f, 0.0f);
    }

    // Compute 1 / sin(theta/2)

    float   oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);

    // Return axis of rotation

    return raVector3(
        (float)v.x * oneOverSinThetaOver2,
        (float)v.y * oneOverSinThetaOver2,
        (float)v.z * oneOverSinThetaOver2
    );
}
float raQuaternion::GetRotationAngle() const
{
	float thetaOver2 = acos(s);
    return thetaOver2 * 2.0f;
}
RAPI raQuaternion raQuaternionExp(const raQuaternion& v)
{
	float Mul;

	raQuaternion temp(v);
	float Length = raVector3Lenght(temp.v);

	if (Length > 1.0e-4)
		Mul = sin(Length)/Length;
	else
		Mul = 1.0;

	temp.s = cos(Length);

	temp.v.x *= Mul;
	temp.v.y *= Mul;
	temp.v.z *= Mul;

	return temp;
}
RAPI raQuaternion raQuaternionLog(const raQuaternion& v)
{
	float Length;
	raQuaternion temp(v);

	Length = raVector3Lenght(temp.v);
	Length = atan(Length/temp.s);

	temp.s = 0.0;

	temp.v.x *= Length;
	temp.v.y *= Length;
	temp.v.z *= Length;

	return temp;
}
RAPI raQuaternion raQuaternionNormalize(const raQuaternion& v)
{
	float norme = raQuaternionLenght(v);
	raQuaternion temp(v);

	if (norme == 0.0)
    {
		temp.s = 1.0f;
		temp.v = 0.0f;
	}
	else
    {
		float recip = 1.0f/norme;

		temp.s *= recip;
		temp.v.x *= recip;
		temp.v.y *= recip;
		temp.v.z *= recip;
	}
	return temp;
}
RAPI raQuaternion raQuaternionFromAxis(const float Angle, raVector3 Axis)
{
	float omega, s, c;
	raQuaternion temp;

	s = raVector3Lenght(Axis);

	if (fabs(s) > FLT_EPSILON)
	{
		c = 1.0f/s;

		Axis.x *= c;
		Axis.y *= c;
		Axis.z *= c;

		omega = -0.5f * Angle;
		s = (float)sin(omega);

		temp.v = raVector3(s*(float)Axis.x, s*(float)Axis.y, s*(float)Axis.z);
		temp.s = (float)cos(omega);
	}
	else
	{
		temp.v = raVector3(0.0f);
		temp.s = 1.0f;
	}
	temp = raQuaternionNormalize(temp);
	return temp;
}
RAPI raMatrix	  raQuaternionToMatrix(const raQuaternion& v)
{
	return raMatrix(v.s,		-v.v.x,			-v.v.y,		-v.v.z,
					v.v.x,		v.s,			-v.v.z,		v.v.y,
					v.v.y,		v.v.z,			v.s,		-v.v.x,
					v.v.z,		-v.v.y,			v.v.x,		v.s);
}
RAPI raQuaternion raQuaternionConjugate(const raQuaternion& v)
{
	raQuaternion temp(v.v, v.s);

	temp.s = -temp.s;
	temp.v = -temp.v;

	return temp;
}
RAPI raQuaternion operator / (const raQuaternion& a,const raQuaternion& b)
{
	raQuaternion z = a ;
	return z /= b;
}
RAPI raQuaternion operator / (const raQuaternion& a, const float b)
{
	raVector3 vt(a.v);
	float     ft(a.s);
	vt /= b;
	ft /= b;

	return raQuaternion(ft, vt);
}
RAPI raQuaternion operator / (const float a, const raQuaternion& b)
{
	raVector3 vt(b.v);
	float     ft(b.s);
	vt /= a;
	ft /= a;

	return raQuaternion(ft, vt);
}

RAPI raQuaternion operator * (const raQuaternion& a, const float b)
{
	raQuaternion temp(a);
	temp *= b ;

	return temp ;
}
RAPI raQuaternion operator * (const float a,const raQuaternion& b)
{
	raQuaternion temp(b);
	temp *= a ;

	return temp ;
}
RAPI raQuaternion operator * (const raQuaternion& a,const raQuaternion& b)
{
	raQuaternion qtmp;

	qtmp.s =   ((a.s * b.s) - (float)(a.v.x * b.v.x) - (float)(a.v.y * b.v.z) - (float)(a.v.z * b.v.z));
	qtmp.v.x = ((a.s * (float)b.v.x) + ((float)a.v.x * b.s) + ((float)a.v.y * (float)b.v.y) - ((float)a.v.z * (float)b.v.y));
	qtmp.v.y = ((a.s * (float)b.v.y) - ((float)a.v.x * (float)b.v.z) + ((float)a.v.y * b.s) + ((float)a.v.z * (float)b.v.x));
	qtmp.v.z = ((a.s * (float)b.v.z) + ((float)a.v.x * (float)b.v.y) - ((float)a.v.y * (float)b.v.x) + ((float)a.v.z * b.s));

	return qtmp;
}
RAPI raQuaternion operator + (const raQuaternion& a,const raQuaternion& b)
{
	raQuaternion at(a), bt(b);

	at += bt;
	return at;
}
RAPI raQuaternion operator - (const raQuaternion& a,const raQuaternion& b)
{
	raQuaternion at(a), bt(b);

	at -= bt;
	return at;
}

RAPI raQuaternion operator - (const raQuaternion& a)
{
	return raQuaternion(-a.s, -a.v);
}

RAPI raQuaternion raQuaternionSlerp(const raQuaternion &q0, const raQuaternion &q1, float t)
{
    if (t <= 0.0f) return q0;
    if (t >= 1.0f) return q1;

	float cosOmega = raQuaternionDot(q0, q1);

    float q1w = q1.s;
    float q1x = q1.v.x;
    float q1y = q1.v.y;
    float q1z = q1.v.z;
    if (cosOmega < 0.0f) {
        q1w = -q1w;
        q1x = -q1x;
        q1y = -q1y;
        q1z = -q1z;
        cosOmega = -cosOmega;
    }

    assert(cosOmega < 1.1f);

    // Compute interpolation fraction, checking for quaternions
    // almost exactly the same

    float k0, k1;
    if (cosOmega > 0.9999f)
	{
        k0 = 1.0f-t;
        k1 = t;
    } else {
        // Compute the sin of the angle using the
        // trig identity sin^2(omega) + cos^2(omega) = 1

        float sinOmega = sqrt(1.0f - cosOmega*cosOmega);

        // Compute the angle from its sin and cosine

        float omega = atan2(sinOmega, cosOmega);

        // Compute inverse of denominator, so we only have
        // to divide once

        float oneOverSinOmega = 1.0f / sinOmega;

        // Compute interpolation parameters

        k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
        k1 = sin(t * omega) * oneOverSinOmega;
    }

    // Interpolate

    raQuaternion result;
    result.v.x = k0*(float)q0.v.x + k1*q1x;
    result.v.y = k0*(float)q0.v.y + k1*q1y;
    result.v.z = k0*(float)q0.v.z + k1*q1z;
    result.s = k0*q0.s + k1*q1w;

    // Return it

    return result;
}
RAPI raQuaternion raQuaternionPow(const raQuaternion& v, float Exp)
{
	if (fabs(v.s) > .9999f)
	{
        return v;
    }

    // Erhalte halbe angle alpha (alpha = theta/2)
    float   alpha = acos(v.s);

    // Berechne neuen alpha Wert
    float   newAlpha = alpha * Exp;

    // Berechne neuen s wert
    raQuaternion result;
    result.s = cos(newAlpha);

    // Berechne neue xyz Werte

    float   mult = sin(newAlpha) / sin(alpha);
    result.v.x = (float)v.v.x * mult;
    result.v.y = (float)v.v.y * mult;
    result.v.z = (float)v.v.z * mult;

    return result;
}
RAPI float raQuaternionDot(const raQuaternion &a, const raQuaternion &b)
{
	 return a.s*b.s + (float)a.v.x*(float)b.v.x + (float)a.v.y*(float)b.v.y + (float)a.v.z*(float)b.v.z;
}
RAPI raQuaternion raQuaternionInvert(const raQuaternion &q)
{
	float temp = raQuaternionLenghtSq(q);
	raQuaternion tq = q;

	tq.s /= temp;
	tq.v.x /= -temp;
	tq.v.y /= -temp;
	tq.v.z /= -temp;
	return q;       // Okay, same norm.
}

RAPI raQuaternion raQuaternionPower(const raQuaternion& qu, float degree)
{
	if ( degree == 0 ) return raQuaternion(1.0f, 0.0f, 0.0f, 0.0f);

	raQuaternion tmp_qu = qu ;

	for ( float f = 1 ; f < abs(degree) ; f++ )
	{
		tmp_qu *= qu ;
	}

	if ( degree < 0 ) return ( 1.0f / tmp_qu );

	return tmp_qu ;
}
RAPI raQuaternion raQuaternionSin(const raQuaternion &q)
{
	raQuaternion s;

	for( float n = 0; n < 6.0f; n++ )
	{
		s += pow( -1.0f ,n ) * (raQuaternionPower( q , 2.0f * n + 1.0f ) ) / ( factorial( 2.0f * n + 1.0f ) );
	}

	return s ;
}
RAPI raQuaternion raQuaternionCos(const raQuaternion &q)
{
	raQuaternion s;

	for( float n = 1.0f; n <= 6.0f; n++ )
	{
		s += pow( -1.0f ,n ) * raQuaternionPower( q , 2.0f * n ) / factorial( 2.0f * n ) ;
	}

	return s ;
}
RAPI raQuaternion raQuaternionTan(const raQuaternion &q)
{
	if ( raQuaternionLenghtSq(q) == 0.0 ) return raQuaternion(1.0, 0.0, 0.0, 0.0) ;
	return raQuaternionSin( q ) / raQuaternionCos( q ) ;
}
RAPI raQuaternion raQuaternionCTan(const raQuaternion &q)
{
	if ( raQuaternionLenghtSq(q) == 0.0 ) return raQuaternion(1.0, 0.0, 0.0, 0.0) ;
	return raQuaternionCos( q ) / raQuaternionSin( q ) ;
}
};