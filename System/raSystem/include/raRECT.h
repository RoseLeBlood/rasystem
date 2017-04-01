#pragma once

	class RAPI raRECT
	{
	public:
		union
		{
			struct
			{
				long left;
				long top;
				long right;
				long bottom;
			};
			struct
			{
				long x1, x2;
				long y1, y2;
			};
			long	r[4];
		};
		raRECT(void) 																										{}
		raRECT(const raRECT& _r)	: left(_r.left), top(_r.top), right(_r.right), bottom(_r.bottom)						{}
		raRECT(const long& _r)	: left(_r), top(_r), right(_r), bottom(_r)													{}
		raRECT(const long& _l, const long& _t, const long& _r, const long& _b) : left(_l), top(_t), right(_r), bottom(_b)	{}
		raRECT(const long* _lpl) : left(_lpl[0]), top(_lpl[1]), right(_lpl[2]), bottom(_lpl[3])							{}
		raRECT(const RECT rec)	 : left(rec.left), right(rec.right), top(rec.top), bottom(rec.bottom)						{}

		operator long* ()							{ return (long*)(r); }
		operator void* ()							{ return (void*)(r); }
		operator RECT  ()							{ RECT rec = {left, top, right, bottom}; return rec; }

		raRECT operator =  (const raRECT& r)		{ left = r.left; top = r.top; right = r.right; bottom = r.bottom; return *this; }
		raRECT operator += (const raRECT& r)		{ left += r.left; top += r.top; right += r.right; bottom += r.bottom; return *this; }
		raRECT operator -= (const raRECT& r)		{ left -= r.left; top -= r.top; right -= r.right; bottom -= r.bottom; return *this; }
		raRECT operator *= (const raRECT& r)		{ left *= r.left; top *= r.top; right *= r.right; bottom *= r.bottom; return *this; }
		raRECT operator /= (const raRECT& r)		{ left /= r.left; top /= r.top; right /= r.right; bottom /= r.bottom; return *this; }

		raRECT operator =  (const long l)			{ left = l; top = l; right = l; bottom = l; return *this; }
		raRECT operator += (const long l)			{ left += l; top += l; right += l; bottom += l; return *this; }
		raRECT operator -= (const long l)			{ left -= l; top -= l; right -= l; bottom -= l; return *this; }
		raRECT operator *= (const long l)			{ left *= l; top *= l; right *= l; bottom *= l; return *this; }
		raRECT operator /= (const long l)			{ left /= l; top /= l; right /= l; bottom /= l; return *this; }
	};