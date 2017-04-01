#pragma once

	class RAPI raPlane
	{
	public:
		union
		{
			struct
			{
				raFloat a;
				raFloat b;
				raFloat c;
				raFloat d;
			};
			struct
			{
				raVector4 n;
			};
			raFloat v[4];
		};
		raPlane(void) 																									{}
		raPlane(const raPlane& p) : a(p.a), b(p.b), c(p.c), d(p.d)														{}
		raPlane(const raFloat _a, const raFloat _b, const raFloat _c, const raFloat _d) : a(_a), b(_b), c(_c), d(_d)		{}
		raPlane(const raFloat* pfValue) : a(pfValue[0]), b(pfValue[1]), c(pfValue[2]), d(pfValue[3])						{}
		raPlane(const raVector4& _n) : n(_n)																				{}
		raPlane(const raVector3& _n, const raFloat p) : a(_n.x), b(_n.y), c(_n.z), d(p)									{}
		operator raFloat* () {return (float*)(v);}

		raPlane& operator = (const raPlane& p) {a = p.a; b = p.b; c = p.c; d = p.d; return *this;}
	};
	inline raPlane operator + (const raPlane& a, const raPlane& b)						{ return raVector4 (a.a + b.a, a.b + b.b, a.c + b.c, a.d + b.d); }
	inline raPlane operator - (const raPlane& a, const raPlane& b)						{ return raVector4 (a.a - b.a, a.b - b.b, a.c - b.c, a.d - b.d); }
	inline raPlane operator - (const raPlane& v)										{ return raVector4 (-v.a, -v.b, -v.c, -v.d); }
	inline raPlane operator * (const raPlane& a,const raPlane& b)						{ return raVector4 (a.a * b.a, a.b * b.b, a.c * b.c, a.d * b.d); }
	inline raPlane operator * (const raPlane& a, const raFloat& b)						{ return raVector4 (a.a * b, a.b * b, a.c * b, a.d * b); }
	inline raPlane operator * (const raFloat& a,const raPlane& b)						{ return raVector4 (a * b.a, a * b.b, a * b.c, a * b.d); }
	inline raPlane operator / (const raPlane& a,const raPlane& b)						{ return raVector4 (a.a / b.a, a.b / b.b, a.c / b.c, a.d / b.d); }
	inline raPlane operator / (const raFloat& a,const raPlane& b)						{ return raVector4 (a / b.a, a / b.b, a / b.c, a / b.d); }
	inline raPlane operator / (const raPlane& a,const raFloat& b)						{ return raVector4 (a.a / b, a.b / b, a.c / b, a.d / b); }

	inline bool operator == (const raPlane& a, const raPlane& b)						{ return ((a.a == b.a) && (a.b == b.b) && (a.c == b.c) && (a.d == b.d)); }
	inline bool operator != (const raPlane& a, const raPlane& b)						{ return ((a.a != b.a) && (a.b != b.b) && (a.c != b.c) && (a.d != b.d)); }
	inline bool operator <= (const raPlane& a, const raPlane& b)						{ return ((a.a <= b.a) && (a.b <= b.b) && (a.c <= b.c) && (a.d <= b.d)); }
	inline bool operator >= (const raPlane& a, const raPlane& b)						{ return ((a.a >= b.a) && (a.b >= b.b) && (a.c >= b.c) && (a.d >= b.d)); }
	inline bool operator < (const raPlane& a, const raPlane& b)							{ return ((a.a < b.a) && (a.b < b.b) && (a.c < b.c) && (a.d < b.d)); }
	inline bool operator > (const raPlane& a, const raPlane& b)							{ return ((a.a > b.a) && (a.b > b.b) && (a.c > b.c) && (a.d > b.d)); }

	// ******************************************************************
	// Funktionen deklarieren
	inline raPlane	raPlaneNormalize(const raPlane& p)													{const float fLength = raVector4Lenght(p.n); return raPlane(p.n / fLength);}
	inline float	raPlaneDotNormal(const raPlane& p, const raVector3& v)								{return p.a * v.x + p.b * v.y + p.c * v.z;}
	inline float	raPlaneDotCoords(const raPlane& p, const raVector3& v)								{return p.a * v.x + p.b * v.y + p.c * v.z + p.d;}
	inline raPlane	raPlaneFromPointNormal(const raVector3& p, const raVector3& n)						{return raPlane(n, -n.x * p.x - n.y * p.y - n.z * p.z);}
	inline raPlane	raPlaneFromPoints(const raVector3& v1, const raVector3& v2, const raVector3& v3)	{return raPlaneFromPointNormal(v1, raVector3Cross(v3 - v2, v1 - v2));}