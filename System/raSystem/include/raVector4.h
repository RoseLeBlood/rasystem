#pragma once

	class RAPI raVector4
	{
	public:
		union
		{
			struct
			{
				raFloat x;
				raFloat y;
				raFloat z;
				raFloat w;
			};
			float c[4];
		};
		raVector4(void)																		: x(0), y(0), z(0), w(0)							{}
		raVector4(const raFloat _x, const raFloat _y, const raFloat _z, const raFloat _w)	: x(_x), y(_y), z(_z), w(_w)					{}
		raVector4(const raFloat f)															: x(f), y(f), z(f), w(f)							{}
		raVector4(const raVector4& vec)														: x(vec.x), y(vec.y), z(vec.z), w(vec.w)			{}
		raVector4(const raFloat *lpf)														: x(lpf[0]), y(lpf[1]), z(lpf[2]), w(lpf[3])		{}
		raVector4(const D3DXVECTOR4 vec)													: x(vec.x), y(vec.y), z(vec.z), w(vec.w)			{}

		operator float*	()														{ return (float*)(c); }
		operator void*  ()														{ return (void*)(c); }
		operator const float* () const											{ return (float*)c; }
		operator D3DXVECTOR4  ()												{ return D3DXVECTOR4(x, y,z, w); }

		raVector4& operator =  (const raVector4& v)								{ x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
		raVector4& operator += (const raVector4& v)								{ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		raVector4& operator -= (const raVector4& v)								{ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		raVector4& operator *= (const raVector4& v)								{ x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
		raVector4& operator /= (const raVector4& v)								{ x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

		raVector4& operator /= (const raFloat& f)								{ x /= f; y /= f; z /= f; w /= f; return *this; }
		raVector4& operator *= (const raFloat& f)								{ x *= f; y *= f; z *= f; w *= f; return *this; }
	};
	inline raVector4 operator + (const raVector4& a, const raVector4& b)					{ return raVector4 (a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
	inline raVector4 operator - (const raVector4& a, const raVector4& b)					{ return raVector4 (a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.z); }
	inline raVector4 operator - (const raVector4& v)										{ return raVector4 (-v.x, -v.y, -v.x, -v.w); }
	inline raVector4 operator * (const raVector4& a,const raVector4& b)						{ return raVector4 (a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
	inline raVector4 operator * (const raVector4& a, const raFloat& b)						{ return raVector4 (a.x * b, a.y * b, a.z * b, a.w * b); }
	inline raVector4 operator * (const raFloat& a,const raVector4& b)						{ return raVector4 (a * b.x, a * b.y, a * b.z, a * b.w); }
	inline raVector4 operator / (const raVector4& a,const raVector4& b)						{ return raVector4 (a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
	inline raVector4 operator / (const raFloat& a,const raVector4& b)						{ return raVector4 (a / b.x, a / b.y, a / b.z, a / b.w); }
	inline raVector4 operator / (const raVector4& a,const raFloat& b)						{ return raVector4 (a.x / b, a.y / b, a.z / b, a.w / b); }

	inline bool operator == (const raVector4& a, const raVector4& b)						{ return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w)); }
	inline bool operator != (const raVector4& a, const raVector4& b)						{ return ((a.x != b.x) && (a.y != b.y) && (a.z != b.z) && (a.w != b.w)); }
	inline bool operator <= (const raVector4& a, const raVector4& b)						{ return ((a.x <= b.x) && (a.y <= b.y) && (a.z <= b.z) && (a.w <= b.w)); }
	inline bool operator >= (const raVector4& a, const raVector4& b)						{ return ((a.x >= b.x) && (a.y >= b.y) && (a.z >= b.z) && (a.w >= b.w)); }
	inline bool operator < (const raVector4& a, const raVector4& b)							{ return ((a.x < b.x) && (a.y < b.y) && (a.z < b.z) && (a.w < b.w)); }
	inline bool operator > (const raVector4& a, const raVector4& b)							{ return ((a.x > b.x) && (a.y > b.y) && (a.z > b.z) && (a.w > b.w)); }

	inline raFloat	 raVector4LenghtSq(const raVector4& v)									{ return (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); }
	inline raFloat	 raVector4Lenght(const raVector4& v)									{ return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); }
	inline raVector4 raVector4Normalize(const raVector4& v)									{ return v / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); }
	inline raVector4 raVector4NormalizeEx(const raVector4& v)								{ return v / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w) + 0.0001f); }

	inline raFloat	 raVector4Dot(const raVector4& v1, const raVector4& v2)					{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;}
	inline raFloat	 raVector4Angle(const raVector4& v1, const raVector4& v2)				{ return (raFloat)acosf((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w) /
																										   (raFloat)sqrtf((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z + v1.w * v1.w) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z + v2.w * v2.w))); }
	inline raVector4 raVector4InterpolateCoords(const raVector4& v1, const raVector4& v2,
												const raFloat p)								{ return v1 + p * (v2 - v1); }
	inline raVector4 raVector4InterpolateNormal(const raVector4& v1, const raVector4& v2,
												const raFloat p)								{ return raVector4Normalize(v1 + p * (v2 - v1)); }
	inline bool		 raVector4NearEqual(const raVector4& v1, raVector4& v2,
										const raVector2& epsilon)							{ return ( abs((float)(v1.x - v2.x) ) <= epsilon && ( abs((float)(v1.y - v2.y)) <= epsilon) && (abs((float)(v1.z - v2.z)) <= epsilon) && (abs((float)(v1.w - v2.w)) <= epsilon)); }