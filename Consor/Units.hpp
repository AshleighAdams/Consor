#ifndef UNITS_H
#define UNITS_H

#include <iostream>
#include <ostream>
#include <istream>

namespace Consor
{
	class Size
	{
	public:
		double Width;
		double Height;

		Size(double Width, double Height);
		Size();

		Size operator+(const Size& A) const;
		Size operator-(const Size& A) const;
		Size operator/(const Size& A) const;
		Size operator*(const Size& A) const;
		Size operator/(double A) const;
		Size operator*(double A) const;
		Size& operator+=(const Size& A);
		Size& operator-=(const Size& A);
		Size& operator*=(const Size& A);
		Size& operator/=(const Size& A);
		Size& operator*=(double A);
		Size& operator/=(double A);
	};

	class Vector
	{
	public:
		double X;
		double Y;

		Vector(double X, double Y);
		Vector();

		Vector operator+(const Vector& A) const;
		Vector operator-(const Vector& A) const;
		Vector operator/(const Vector& A) const;
		Vector operator*(const Vector& A) const;
		Vector operator/(double A) const;
		Vector operator*(double A) const;
		Vector& operator+=(const Vector& A);
		Vector& operator-=(const Vector& A);
		Vector& operator*=(const Vector& A);
		Vector& operator/=(const Vector& A);
		Vector& operator*=(double A);
		Vector& operator/=(double A);
		bool operator==(const Vector& A) const;
		bool operator!=(const Vector& A) const;
	};

	class Colour
	{
	public:
		double R, G, B, A;
		Colour();
		Colour(double R, double G, double B);
		Colour(double R, double G, double B, double A);

		Colour& Normalize();
		
		static Colour Blend(const Colour& A, const Colour& B);
		static double Distance(const Colour& A, const Colour& B);

		static const Colour& None();

		Colour operator+(const Colour& A) const;
		Colour operator-(const Colour& A) const;
		Colour& operator+=(const Colour& A);
		Colour& operator-=(const Colour& A);
		bool operator==(const Colour& A) const;
		bool operator!=(const Colour& A) const;
	};



	// literals
	/*
	Size operator "" _w(long double Value);
	Size operator "" _h(long double Value);

	Vector operator "" _x(long double Value);
	Vector operator "" _t(long double Value);
	*/
};

// outside namespace
// tostring:
std::ostream& operator<<(std::ostream& stream, const Consor::Size& value);
std::ostream& operator<<(std::ostream& stream, const Consor::Vector& value);
std::ostream& operator<<(std::ostream& stream, const Consor::Colour& value);

#endif