#include "..\include\raMain.h"

namespace System
{
RAPI raColor raColorFromYUV(float y, float u, float v)
{
	float r = 1.164f * (y - 16) + 1.596f*(v - 128);
    float g = 1.164f * (y - 16) - 0.813f*(v - 128) - 0.391f*(u - 128);
    float b = 1.164f * (y - 16) + 2.018f*(u - 128);

	return raColor(r, g, b);
}

RAPI raColor raColorFromCMY(float c, float m, float y)
{
	return raColor( 1.0f - c, 1.0f - m, 1.0f - y);
}

RAPI raColor raColorFromHSV(float h, float s, float v)
{
	float i;
	float f, p, q, t;
	if( s == 0 )
	{
		// achromatic (grey)
		return raColor(v,v,v);
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	if(i == 0)
		return raColor(v, t, p);
	if( i == 1.0f)
		return raColor(q, v, p);
	if(i == 2.0f)
		return raColor(p, v, t);
	if(i == 3.0f)
		return raColor(p, q, v);
	if(i == 4.0f)
		return raColor(t, p, v);

	return raColor(v, p, q);
}
};