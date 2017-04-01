#pragma once

	class RAPI raMatrix;

	class RAPI raVector3
	{
	public:
		union
		{
			struct
			{
				raFloat x;
				raFloat y;
				raFloat z;
			};
			raFloat c[3];
		};

		raVector3(void)															: x(0), y(0), z(0)					{}
		raVector3(raFloat _x, raFloat _y, raFloat _z)							: x(_x), y(_y), z(_z)				{}
		raVector3(const raFloat f)												: x(f), y(f), z(f)					{}
		raVector3(const raVector3& vec)											: x(vec.x), y(vec.y), z(vec.z)		{}
		raVector3(const raFloat *lpf)											: x(lpf[0]), y(lpf[1]), z(lpf[2])	{}
		raVector3(D3DXVECTOR3 vec)												: x(vec.x), y(vec.y), z(vec.z)		{}

		operator float*	()														{ return (float*)(c); }
		operator void*  ()														{ return (void*)(c); }
		operator const float* () const											{ return (float*)c; }
		operator D3DXVECTOR3 ()													{ return  D3DXVECTOR3((float)x, (float)y, (float)z); }
		operator D3DXVECTOR3*()													{ return &D3DXVECTOR3((float)x, (float)y, (float)z); }

		raVector3& operator =  (const raVector3& v)								{ x = v.x; y = v.y; z = v.z; return *this; }
		raVector3& operator += (const raVector3& v)								{ x += v.x; y += v.y; z += v.z; return *this; }
		raVector3& operator -= (const raVector3& v)								{ x -= v.x; y -= v.y; z -= v.z; return *this; }
		raVector3& operator *= (const raVector3& v)								{ x *= v.x; y *= v.y; z *= v.z; return *this; }
		raVector3& operator /= (const raVector3& v)								{ x /= v.x; y /= v.y; z /= v.z; return *this; }

		raVector3& operator /= (const float& f)									{ x /= f; y /= f; z /= f; return *this; }
		raVector3& operator *= (const float& f)									{ x *= f; y *= f; z *= f; return *this; }
	};
	inline raVector3 operator + (const raVector3& a, const raVector3& b)					{ return raVector3 (a.x + b.x, a.y + b.y, a.z + b.z); }
	inline raVector3 operator - (const raVector3& a, const raVector3& b)					{ return raVector3 (a.x - b.x, a.y - b.y, a.z - b.z); }
	inline raVector3 operator - (const raVector3& a)										{ return raVector3 (-a.x, -a.y, -a.z); }
	inline raVector3 operator * (const raVector3& a,const raVector3& b)						{ return raVector3 (a.x * b.x, a.y * b.y, a.z * b.z); }
	inline raVector3 operator * (const raVector3& a, const raFloat& b)						{ return raVector3 (a.x * b, a.y * b, a.z * b); }
	inline raVector3 operator * (const raFloat& a,const raVector3& b)						{ return raVector3 (a * b.x, a * b.y, a * b.z); }
	inline raVector3 operator / (const raVector3& a,const raVector3& b)						{ return raVector3 (a.x / b.x, a.y / b.y, a.z / b.z); }
	inline raVector3 operator / (const raFloat& a,const raVector3& b)						{ return raVector3 (a / b.x, a / b.y, a / b.z); }
	inline raVector3 operator / (const raVector3& a,const raFloat& b)						{ return raVector3 (a.x / b, a.y / b, a.z / b); }

	inline bool operator == (const raVector3& a, const raVector3& b)						{ return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z)); }
	inline bool operator != (const raVector3& a, const raVector3& b)						{ return ((a.x != b.x) && (a.y != b.y) && (a.z != b.z)); }
	inline bool operator <= (const raVector3& a, const raVector3& b)						{ return ((a.x <= b.x) && (a.y <= b.y) && (a.z <= b.z)); }
	inline bool operator >= (const raVector3& a, const raVector3& b)						{ return ((a.x >= b.x) && (a.y >= b.y) && (a.z >= b.z)); }
	inline bool operator < (const raVector3& a, const raVector3& b)							{ return ((a.x < b.x) && (a.y < b.y) && (a.z < b.z)); }
	inline bool operator > (const raVector3& a, const raVector3& b)							{ return ((a.x > b.x) && (a.y > b.y) && (a.z > b.z)); }

	inline raFloat	 raVector3LenghtSq(const raVector3& v)									{ return (v.x * v.x + v.y * v.y + v.z * v.z); }
	inline raFloat	 raVector3Lenght(const raVector3& v)									{ return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
	inline raVector3 raVector3Normalize(const raVector3& v)									{ return v / sqrtf(raFloat(v.x * v.x + v.y * v.y + v.z * v.z)); }
	inline raVector3 raVector3NormalizeEx(const raVector3& v)								{ return v / (sqrtf(raFloat(v.x * v.x + v.y * v.y + v.z * v.z)) + 0.0001f); }
	inline raVector3 raVector3Cross(const raVector3& v1, raVector3& v2)						{ return raVector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
																										   v1.x * v2.y - v1.y * v2.x); }
	inline raFloat	 raVector3Dot(const raVector3& v1, const raVector3& v2)					{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
	inline raFloat	 raVector3Angle(const raVector3& v1, const raVector3& v2)				{ return acosf(float((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z)) /
																										   sqrtf(float((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z)) * float((v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)))); }
	inline raVector3 raVector3InterpolateCoords(const raVector3& v1, const raVector3& v2,
												const raFloat p)								{ return v1 + p * (v2 - v1); }
	inline raVector3 raVector3InterpolateNormal(const raVector3& v1, const raVector3& v2,
												const raFloat p)								{ return raVector3Normalize(v1 + p * (v2 - v1)); }
	inline bool		 raVector3NearEqual(const raVector3& v1, raVector3& v2,
										const raVector2& epsilon)							{ return ( abs(float(v1.x - v2.x )) <= epsilon && ( abs(float(v1.y - v2.y)) <= epsilon) && (abs(float(v1.z - v2.z)) <= epsilon)); }