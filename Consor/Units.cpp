#include "Units.hpp"
#include <cmath>

using namespace Consor;

// CSize

CSize::CSize(double W, double H)
{
	Width = W;
	Height = H;
}

CSize::CSize()
{
	Width = 0;
	Height = 0;
}

CSize CSize::operator+(const CSize& A) const
{
	return CSize(this->Width + A.Width, this->Height + A.Height);
}

CSize CSize::operator-(const CSize& A) const
{
	return CSize(this->Width - A.Width, this->Height - A.Height);
}

CSize CSize::operator/(const CSize& A) const
{
	return CSize(this->Width / A.Width, this->Height / A.Height);
}

CSize CSize::operator*(const CSize& A) const
{
	return CSize(this->Width * A.Width, this->Height * A.Height);
}

CSize CSize::operator/(double A) const
{
	return CSize(this->Width / A, this->Height / A);
}

CSize CSize::operator*(double A) const
{
	return CSize(this->Width * A, this->Height * A);
}

CSize& CSize::operator+=(const CSize& A)
{
	this->Width += A.Width;
	this->Height += A.Height;
	return *this;
}

CSize& CSize::operator-=(const CSize& A)
{
	this->Width -= A.Width;
	this->Height -= A.Height;
	return *this;
}

CSize& CSize::operator*=(const CSize& A)
{
	this->Width *= A.Width;
	this->Height *= A.Height;
	return *this;
}

CSize& CSize::operator/=(const CSize& A)
{
	this->Width /= A.Width;
	this->Height /= A.Height;
	return *this;
}

CSize& CSize::operator*=(double A)
{
	this->Width *= A;
	this->Height *= A;
	return *this;
}

CSize& CSize::operator/=(double A)
{
	this->Width /= A;
	this->Height /= A;
	return *this;
}

// CVector


CVector::CVector(double X, double Y)
{
	this->X = X;
	this->Y = Y;
}

CVector::CVector()
{
	X = 0;
	Y = 0;
}

CVector CVector::operator+(const CVector& A) const
{
	return CVector(this->X + A.X, this->Y + A.Y);
}

CVector CVector::operator-(const CVector& A) const
{
	return CVector(this->X - A.X, this->Y - A.Y);
}

CVector CVector::operator/(const CVector& A) const
{
	return CVector(this->X / A.X, this->Y / A.Y);
}

CVector CVector::operator*(const CVector& A) const
{
	return CVector(this->X * A.X, this->Y * A.Y);
}

CVector CVector::operator/(double A) const
{
	return CVector(this->X / A, this->Y / A);
}

CVector CVector::operator*(double A) const
{
	return CVector(this->X * A, this->Y * A);
}

CVector& CVector::operator+=(const CVector& A)
{
	this->X += A.X;
	this->Y += A.Y;
	return *this;
}

CVector& CVector::operator-=(const CVector& A)
{
	this->X -= A.X;
	this->Y -= A.Y;
	return *this;
}

CVector& CVector::operator*=(const CVector& A)
{
	this->X *= A.X;
	this->Y *= A.Y;
	return *this;
}

CVector& CVector::operator/=(const CVector& A)
{
	this->X /= A.X;
	this->Y /= A.Y;
	return *this;
}

CVector& CVector::operator*=(double A)
{
	this->X *= A;
	this->Y *= A;
	return *this;
}

CVector& CVector::operator/=(double A)
{
	this->X /= A;
	this->Y /= A;
	return *this;
}

// CColour

CColour::CColour()
{
	R = 0;
	G = 0;
	B = 0;
	A = 1;
}

CColour::CColour(double r, double g, double b)
{
	R = r;
	G = g;
	B = b;
	A = 1;
}

CColour::CColour(double r, double g, double b, double a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

template<class T>
T min(T A, T B)
{
	return A<B?A:B;
}

template<class T>
T max(T A, T B)
{
	return A>B?A:B;
}

template<class T>
T clamp(T Value, T Min, T Max)
{
	
	return min<T>(max<T>(Min, Value), Max);
}

CColour& CColour::Normalize()
{
	R = clamp(R, 0.0, 1.0);
	G = clamp(G, 0.0, 1.0);
	B = clamp(B, 0.0, 1.0);
	A = clamp(A, 0.0, 1.0);

	return *this;
}

CColour CColour::Blend(const CColour& A, const CColour& B)
{
	if(A.A >= 1.0)
	{
		CColour ret = A;
		return ret.Normalize();
	}
	else if(A.A <= 0.0)
	{
		CColour ret = B;
		return ret.Normalize();
	}

	CColour src = A;
	CColour dst = B;

	src.Normalize();
	dst.Normalize();

	CColour out;

	out.A = src.A + dst.A * (1 - src.A);

	out.R = (src.R*src.A + dst.R*dst.A * (1.0 - src.A)) / out.A;
	out.G = (src.G*src.A + dst.G*dst.A * (1.0 - src.A)) / out.A;
	out.B = (src.B*src.A + dst.B*dst.A * (1.0 - src.A)) / out.A;

	return out.Normalize();
}

const CColour& CColour::None()
{
	static CColour none = CColour(0, 0, 0, 0);

	return none;
}

CColour CColour::operator+(const CColour& A) const
{
	return CColour(this->R + A.R, this->G + A.G, this->B + A.B, this->A + A.A).Normalize();
}

CColour CColour::operator-(const CColour& A) const
{
	return CColour(this->R - A.R, this->G - A.G, this->B - A.B, this->A - A.A).Normalize();
}

CColour& CColour::operator+=(const CColour& A)
{
	this->R += A.R;
	this->G += A.G;
	this->B += A.B;
	this->A += A.A;

	return this->Normalize();
}

CColour& CColour::operator-=(const CColour& A)
{
	this->R -= A.R;
	this->G -= A.G;
	this->B -= A.B;
	this->A -= A.A;

	return this->Normalize();
}