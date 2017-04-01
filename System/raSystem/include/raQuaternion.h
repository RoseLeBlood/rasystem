#pragma once

	class RAPI raQuaternion
	{
	  public:
		  union
		  {
			  struct
			  {
				  raVector3       v;
				  float			  s;
			  };
			  float qu[4];
		  };
		raQuaternion() : s(1.0f), v(raVector3(0.0f))						{}
		raQuaternion(const raVector3 &angles);
		raQuaternion(float Angle, raVector3 Axis);
		raQuaternion(raVector3 Axis, float Angle);
		raQuaternion(float fs, float fx, float fy, float fz);
		raQuaternion(float *pfs);
		raQuaternion(raMatrix mat);

		operator float*	()														{ return (float*)(qu); }
		operator void*  ()														{ return (void*)(qu); }
		operator const float* () const											{ return (float*)qu; }

		raQuaternion& operator =  (const raQuaternion& q)								{ v = q.v; s = q.s; return *this; }
		raQuaternion& operator += (const raQuaternion& q)								{ v += q.v; s += q.s; return *this; }
		raQuaternion& operator -= (const raQuaternion& q)								{ v -= q.v; s -= q.s; return *this; }
		raQuaternion& operator *= (const raQuaternion& q);		//
		raQuaternion& operator *= (const float f);
		raQuaternion& operator /= (const raQuaternion& q);      //
		raQuaternion& operator /= (const float f);

		raVector3 GetRotationAxis() const;
		float	  GetRotationAngle() const;
	};
	RAPI raQuaternion operator * (const raQuaternion& a,const raQuaternion& b); //
	RAPI raQuaternion operator * (const float a,const raQuaternion& b);
	RAPI raQuaternion operator * (const raQuaternion& a,const float b);			//

	RAPI raQuaternion operator + (const raQuaternion& a,const raQuaternion& b); //
	RAPI raQuaternion operator - (const raQuaternion& a,const raQuaternion& b); //
	RAPI raQuaternion operator - (const raQuaternion& a); //
	RAPI raQuaternion operator / (const raQuaternion& a,const raQuaternion& b);
	RAPI raQuaternion operator / (const raQuaternion& a, const float b);	//
	RAPI raQuaternion operator / (const float a, const raQuaternion& b);

	inline bool operator == (const raQuaternion& a, const raQuaternion& b)							{ return ((a.v == b.v) && (a.s == b.s)); }
	inline bool operator != (const raQuaternion& a, const raQuaternion& b)							{ return !(a==b); }
	inline bool operator <= (const raQuaternion& a, const raQuaternion& b)							{ return ((a.v <= b.v) && (a.s <= b.s)); }
	inline bool operator >= (const raQuaternion& a, const raQuaternion& b)							{ return ((a.v >= b.v) && (a.s >= b.s)); }
	inline bool operator < (const raQuaternion& a, const raQuaternion& b)							{ return ((a.v < b.v) && (a.s < b.s)); }
	inline bool operator > (const raQuaternion& a, const raQuaternion& b)							{ return ((a.v > b.v) && (a.s > b.s)); }

	inline raQuaternion raQuaternionIdenty()														{ return raQuaternion(1.0f, 0.0f, 0.0f, 0.0f); }
	inline float raQuaternionLenght(const raQuaternion& v)											{ return sqrtf(v.s * v.s + (float)v.v.x * (float)v.v.x + (float)v.v.y * (float)v.v.y + (float)v.v.z * (float)v.v.z); }
	inline float raQuaternionLenghtSq(const raQuaternion& v)										{ return (v.s * v.s + (float)v.v.x * (float)v.v.x + (float)v.v.y * (float)v.v.y + (float)v.v.z * (float)v.v.z); }

	RAPI raQuaternion raQuaternionExp(const raQuaternion& v); //
	RAPI raQuaternion raQuaternionLog(const raQuaternion& v); //
	RAPI raQuaternion raQuaternionPow(const raQuaternion& v, float Exp);
	RAPI float		  raQuaternionDot(const raQuaternion &a, const raQuaternion &b);		// Dot Product

	RAPI raQuaternion raQuaternionInvert(const raQuaternion &q);

	RAPI raQuaternion raQuaternionNormalize(const raQuaternion& v);
	RAPI raQuaternion raQuaternionFromAxis(const float Angle, raVector3 Axis);
	RAPI raMatrix	  raQuaternionToMatrix(const raQuaternion& v);
	RAPI raQuaternion raQuaternionConjugate(const raQuaternion& v);
	RAPI raQuaternion raQuaternionSlerp(const raQuaternion &q0, const raQuaternion &q1, float t);	// Spherical linear interpolation.

	RAPI raQuaternion raQuaternionPower(const raQuaternion& qu, float degree);

	RAPI raQuaternion raQuaternionSin(const raQuaternion &q);
	RAPI raQuaternion raQuaternionCos(const raQuaternion &q);
	RAPI raQuaternion raQuaternionTan(const raQuaternion &q);
	RAPI raQuaternion raQuaternionCTan(const raQuaternion &q);