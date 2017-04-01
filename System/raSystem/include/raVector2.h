#pragma once

	class RAPI raVector2
	{
	public:
		union
		{
			struct
			{
				raFloat x;
				raFloat y;
			};
			float c[2];
		};
		raVector2() 														{}
		raVector2(raFloat _x, raFloat _y)		: x(_x), y(_y)				{}
		raVector2(raFloat _f)					: x(_f), y(_f)				{}
		raVector2(const raVector2& vec)			: x(vec.x), y(vec.y)			{}
		raVector2(const raFloat* lpvec)			: x(lpvec[0]), y(lpvec[1])	{}
		raVector2(const D3DXVECTOR2 vec)		: x(vec.x), y(vec.y)		{}

		operator float* ()				{ return (float*)(c); }
		operator void* ()				{ return (void*)(c); }
		operator D3DXVECTOR2 ()			{ return D3DXVECTOR2(x, y); }

		raVector2& operator =  (const raVector2& v)					{ x = v.x; y = v.y; return *this; }
		raVector2& operator += (const raVector2& v)					{ x += v.x; y += v.y; return *this; }
		raVector2& operator -= (const raVector2& v)					{ x -= v.x; y -= v.y; return *this; }
		raVector2& operator *= (const raVector2& v)					{ x *= v.x; y *= v.y; return *this; }
		raVector2& operator /= (const raVector2& v)					{ x /= v.x; y /= v.y; return *this; }

		raVector2& operator /= (const raFloat& f)						{ x /= f; y /= f; return *this; }
		raVector2& operator *= (const raFloat& f)						{ x *= f; y *= f; return *this; }
	};

	inline raVector2 operator + (const raVector2& a, const raVector2& b)	{ return raVector2(a.x + b.x, a.y + b.y); }
	inline raVector2 operator - (const raVector2& a, const raVector2& b)	{ return raVector2(a.x - b.x, a.y - b.y); }
	inline raVector2 operator * (const raVector2& a, const raVector2& b)	{ return raVector2(a.x * b.x, a.y * b.y); }
	inline raVector2 operator / (const raVector2& a, const raVector2& b)	{ return raVector2(a.x / b.x, a.y / b.y); }
	inline raVector2 operator + (const raFloat& f, const raVector2& b)		{ return raVector2(f + b.x, f + b.y); }
	inline raVector2 operator - (const raFloat& f, const raVector2& b)		{ return raVector2(f - b.x, f - b.y); }
	inline raVector2 operator * (const raFloat& f, const raVector2& b)		{ return raVector2(f * b.x, f * b.y); }
	inline raVector2 operator / (const raFloat& f, const raVector2& b)		{ return raVector2(f / b.x, f / b.y); }
	inline raVector2 operator - (const raVector2& a, const raFloat& f)		{ return raVector2(a.x - f, a.y - f); }
	inline raVector2 operator / (const raVector2& a, const raFloat& f)		{ return raVector2(a.x / f, a.y / f); }

	inline bool operator == (const raVector2& a, const raVector2& b)		{ return ((a.x == b.x) && (a.y == b.y)); }
	inline bool operator != (const raVector2& a, const raVector2& b)		{ return ((a.x != b.x) && (a.y != b.y)); }
	inline bool operator <= (const raVector2& a, const raVector2& b)		{ return ((a.x <= b.x) && (a.y <= b.y)); }
	inline bool operator >= (const raVector2& a, const raVector2& b)		{ return ((a.x >= b.x) && (a.y >= b.y)); }
	inline bool operator <  (const raVector2& a, const raVector2& b)		{ return ((a.x < b.x) && (a.y < b.x)); }
	inline bool operator >  (const raVector2& a, const raVector2& b)		{ return ((a.x > b.x) && (a.y > b.y)); }

	inline raFloat	 raVector2Lenght(const raVector2& v)									{ return (v.x * v.x + v.y * v.y ); }
	inline raFloat	 raVector2LenghtSq(const raVector2& v)									{ return sqrtf(v.x * v.x + v.y * v.y ); }
	inline raVector2 raVector2Normalize(const raVector2& v)									{ return v / (raFloat)sqrtf(v.x * v.x + v.y * v.y ); }
	inline raVector2 raVector2NormalizeEx(const raVector2& v)								{ return v / (raFloat)sqrtf((raFloat)(v.x * v.x + v.y * v.y ) + (raFloat)0.0001f); }

	inline raFloat	 raVector2Dot(const raVector2& v1, const raVector2& v2)					{ return (raFloat)(v1.x * v2.x + v1.y * v2.y);}
	inline raFloat	 raVector2Angle(const raVector2& v1, const raVector2& v2)				{ return (raFloat)acosf((v1.x * v2.x + v1.y * v2.y ) /
																										   (raFloat)sqrtf((v1.x * v1.x + v1.y * v1.y ) * (v2.x * v2.x + v2.y * v2.y))); }
	inline raVector2 raVector2InterpolateCoords(const raVector2& v1, const raVector2& v2,
												const raFloat p)								{ return v1 + p * (v2 - v1); }
	inline raVector2 raVector2InterpolateNormal(const raVector2& v1, const raVector2& v2,
												const raFloat p)								{ return raVector2Normalize(v1 + p * (v2 - v1)); }
	inline bool		 raVector2NearEqual(const raVector2& v1, raVector2& v2,
										const raVector2& epsilon)							{ return (( abs((float)(v1.x - v2.x )) <= epsilon) && (abs((float)(v1.y - v2.y)) <= epsilon)); }