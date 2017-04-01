#pragma once

#define raCOLORCON (0.003921568627450980392156862745098f)

	class RAPI raColor
	{
	public:
		union
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
			struct
			{
				float red;
				float green;
				float blue;
				float alpa;
			};
			float c[4];
			//D3DCOLORVALUE	D3DColorValue;
		};
		raColor(void) 																																															{}
		raColor(float _r, float _g, float _b, float _a)	: r(_r), g(_g), b(_b), a(_a)																															{}
		raColor(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f)																																	{}
		raColor(float f) : r(f), g(f), b(f), a(f)																																								{}
		raColor(float *com) : r(com[0]), g(com[1]), b(com[2]), a(com[3])																																	{}
		raColor(const int _r, const int _g, const int _b) : r((float)(_r) * raCOLORCON), g((float)(_g) * raCOLORCON), b((float)(_b) * raCOLORCON), a(1.0f)														{}
		raColor(const int _r, const int _g, const int _b, const int _a) : r((float)(_r) * raCOLORCON), g((float)(_g) * raCOLORCON), b((float)(_b) * raCOLORCON), a((float)(_a) * raCOLORCON)					{}
		raColor(const int* pComponent) : r((float)(pComponent[0]) * raCOLORCON), g((float)(pComponent[1]) * raCOLORCON), b((float)(pComponent[2]) * raCOLORCON), a((float)(pComponent[3]) * raCOLORCON)		{}
		raColor(const int c) : r(raCOLORCON * (float)(int)(c >> 16)), g(raCOLORCON * (float)(int)(c >> 8)), b(raCOLORCON * (float)(int)(c)), a(raCOLORCON * (float)(int)(c >> 24))							{}

		operator unsigned long () const
		{
			return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (DWORD)(a * 255.0f)) << 24) |
				   ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (DWORD)(r * 255.0f)) << 16) |
				   ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (DWORD)(g * 255.0f)) << 8)  |
				   (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (DWORD)(b * 255.0f));
		}
		operator float* ()			{return (float*)(c);}
		//operator D3DCOLORVALUE& ()	{return D3DColorValue;}

		raColor& operator = (const raColor& c)	{a = c.a; b = c.b; g = c.g; r = c.r; return *this;}
		raColor& operator += (const raColor& c)	{r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
		raColor& operator -= (const raColor& c)	{r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
		raColor& operator *= (const raColor& c)	{r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
		raColor& operator *= (const float f)	{r *= f; g *= f; b *= f; a *= f; return *this;}
		raColor& operator /= (const raColor& c)	{r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
		raColor& operator /= (const float f)	{r /= f; g /= f; b /= f; a /= f; return *this;}
	};
	inline raColor operator + (const raColor& a, const raColor& b)	{return raColor(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);}
	inline raColor operator - (const raColor& a, const raColor& b)	{return raColor(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);}
	inline raColor operator - (const raColor& c)					{return raColor(-c.r, -c.g, -c.b, c.a);}
	inline raColor operator * (const raColor& a, const raColor& b)	{return raColor(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);}
	inline raColor operator * (const raColor& c, const float f)		{return raColor(c.r * f, c.g * f, c.b * f, c.a * f);}
	inline raColor operator * (const float f, const raColor& c)		{return raColor(c.r * f, c.g * f, c.b * f, c.a * f);}
	inline raColor operator / (const raColor& a, const raColor& b)	{return raColor(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);}
	inline raColor operator / (const raColor& c, const float f)		{return raColor(c.r / f, c.g / f, c.b / f, c.a / f);}

	// Vergleichsoperatoren
	inline bool operator == (const raColor& a, const raColor& b) {if(a.r != b.r) return false; if(a.g != b.g) return false; if(a.b != b.b) return false; return a.a == b.a;}
	inline bool operator != (const raColor& a, const raColor& b) {if(a.r != b.r) return true; if(a.g != b.g) return true; if(a.b != b.b) return true; return a.a != b.a;}

	// ******************************************************************
	// Funktionen deklarieren
	inline raColor	raColorNegate(const raColor& c)											{return raColor(1.0f - c.r, 1.0f - c.g, 1.0f - c.b, 1.0f - c.a);}
	inline float	raColorBrightness(const raColor& c)										{return c.r * 0.299f + c.g * 0.587f + c.b * 0.114f;}
	inline raColor	raColorInterpolate(const raColor& c1, const raColor& c2, const float p)	{return c1 + p * (c2 - c1);}
	inline raColor	raColorMin(const raColor& c1, const raColor& c2)						{return raColor(RAMIN(c1.r, c2.r), RAMIN(c1.g, c2.g), RAMIN(c1.b, c2.b), RAMIN(c1.a, c2.a));}
	inline raColor	raColorMax(const raColor& c1, const raColor& c2)						{return raColor(RAMAX(c1.r, c2.r), RAMAX(c1.g, c2.g), RAMAX(c1.b, c2.b), RAMAX(c1.a, c2.a));}

	RAPI raColor raColorFromYUV(float y, float u, float v);
	RAPI raColor raColorFromHSV(float h, float s, float v);
	RAPI raColor raColorFromCMY(float c, float m, float y);