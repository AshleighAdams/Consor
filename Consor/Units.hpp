#ifndef UNITS_H
#define UNITS_H

namespace Consor
{
	class CSize
	{
	public:
		double Width;
		double Height;

		CSize(double Width, double Height);
		CSize();

		CSize operator+(const CSize& A) const;
		CSize operator-(const CSize& A) const;
		CSize operator/(const CSize& A) const;
		CSize operator*(const CSize& A) const;
		CSize operator/(double A) const;
		CSize operator*(double A) const;
		CSize& operator+=(const CSize& A);
		CSize& operator-=(const CSize& A);
		CSize& operator*=(const CSize& A);
		CSize& operator/=(const CSize& A);
		CSize& operator*=(double A);
		CSize& operator/=(double A);
	};

	class CVector
	{
	public:
		double X;
		double Y;

		CVector(double X, double Y);
		CVector();

		CVector operator+(const CVector& A) const;
		CVector operator-(const CVector& A) const;
		CVector operator/(const CVector& A) const;
		CVector operator*(const CVector& A) const;
		CVector operator/(double A) const;
		CVector operator*(double A) const;
		CVector& operator+=(const CVector& A);
		CVector& operator-=(const CVector& A);
		CVector& operator*=(const CVector& A);
		CVector& operator/=(const CVector& A);
		CVector& operator*=(double A);
		CVector& operator/=(double A);
	};

	class CColour
	{
	public:
		double R, G, B, A;
		CColour();
		CColour(double R, double G, double B);
		CColour(double R, double G, double B, double A);

		CColour& Normalize();
		static CColour Blend(const CColour& A, const CColour& B);

		CColour operator+(const CColour& A) const;
		CColour operator-(const CColour& A) const;
		CColour& operator+=(const CColour& A);
		CColour& operator-=(const CColour& A);
	};

	// literals
	/*
	CSize operator "" _w(long double Value);
	CSize operator "" _h(long double Value);

	CVector operator "" _x(long double Value);
	CVector operator "" _t(long double Value);
	*/
};

#endif