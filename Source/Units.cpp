#include "Units.hpp"
#include <cmath>

using namespace Consor;

// Size

Size::Size(double W, double H)
{
	Width = W;
	Height = H;
}

Size::Size()
{
	Width = 0;
	Height = 0;
}

Size Size::operator+(const Size& A) const
{
	return Size(this->Width + A.Width, this->Height + A.Height);
}

Size Size::operator-(const Size& A) const
{
	return Size(this->Width - A.Width, this->Height - A.Height);
}

Size Size::operator/(const Size& A) const
{
	return Size(this->Width / A.Width, this->Height / A.Height);
}

Size Size::operator*(const Size& A) const
{
	return Size(this->Width * A.Width, this->Height * A.Height);
}

Size Size::operator/(double A) const
{
	return Size(this->Width / A, this->Height / A);
}

Size Size::operator*(double A) const
{
	return Size(this->Width * A, this->Height * A);
}

Size& Size::operator+=(const Size& A)
{
	this->Width += A.Width;
	this->Height += A.Height;
	return *this;
}

Size& Size::operator-=(const Size& A)
{
	this->Width -= A.Width;
	this->Height -= A.Height;
	return *this;
}

Size& Size::operator*=(const Size& A)
{
	this->Width *= A.Width;
	this->Height *= A.Height;
	return *this;
}

Size& Size::operator/=(const Size& A)
{
	this->Width /= A.Width;
	this->Height /= A.Height;
	return *this;
}

Size& Size::operator*=(double A)
{
	this->Width *= A;
	this->Height *= A;
	return *this;
}

Size& Size::operator/=(double A)
{
	this->Width /= A;
	this->Height /= A;
	return *this;
}
bool Size::operator==(const Size& A) const
{
	return this->Width == A.Width && this->Height == A.Height;
}

bool Size::operator!=(const Size& A) const
{
	return !(*this == A);
}

// Vector


Vector::Vector(double X, double Y)
{
	this->X = X;
	this->Y = Y;
}

Vector::Vector()
{
	X = 0;
	Y = 0;
}

Vector Vector::operator+(const Vector& A) const
{
	return Vector(this->X + A.X, this->Y + A.Y);
}

Vector Vector::operator-(const Vector& A) const
{
	return Vector(this->X - A.X, this->Y - A.Y);
}

Vector Vector::operator/(const Vector& A) const
{
	return Vector(this->X / A.X, this->Y / A.Y);
}

Vector Vector::operator*(const Vector& A) const
{
	return Vector(this->X * A.X, this->Y * A.Y);
}

Vector Vector::operator/(double A) const
{
	return Vector(this->X / A, this->Y / A);
}

Vector Vector::operator*(double A) const
{
	return Vector(this->X * A, this->Y * A);
}

Vector& Vector::operator+=(const Vector& A)
{
	this->X += A.X;
	this->Y += A.Y;
	return *this;
}

Vector& Vector::operator-=(const Vector& A)
{
	this->X -= A.X;
	this->Y -= A.Y;
	return *this;
}

Vector& Vector::operator*=(const Vector& A)
{
	this->X *= A.X;
	this->Y *= A.Y;
	return *this;
}

Vector& Vector::operator/=(const Vector& A)
{
	this->X /= A.X;
	this->Y /= A.Y;
	return *this;
}

Vector& Vector::operator*=(double A)
{
	this->X *= A;
	this->Y *= A;
	return *this;
}

Vector& Vector::operator/=(double A)
{
	this->X /= A;
	this->Y /= A;
	return *this;
}

bool Vector::operator==(const Vector& A) const
{
	return this->X == A.X && this->Y == A.Y;
}

bool Vector::operator!=(const Vector& A) const
{
	return !(*this == A);
}

// Colour

Colour::Colour()
{
	R = 0;
	G = 0;
	B = 0;
	A = 1;
}

Colour::Colour(double r, double g, double b)
{
	R = r;
	G = g;
	B = b;
	A = 1;
}

Colour::Colour(double r, double g, double b, double a)
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

Colour& Colour::Normalize()
{
	R = clamp(R, 0.0, 1.0);
	G = clamp(G, 0.0, 1.0);
	B = clamp(B, 0.0, 1.0);
	A = clamp(A, 0.0, 1.0);

	return *this;
}

Colour Colour::Blend(const Colour& A, const Colour& B)
{
	if(A.A >= 1.0)
	{
		Colour ret = A;
		return ret.Normalize();
	}
	else if(A.A <= 0.0)
	{
		Colour ret = B;
		return ret.Normalize();
	}

	Colour src = A;
	Colour dst = B;

	src.Normalize();
	dst.Normalize();

	Colour out;

	out.A = src.A + dst.A * (1 - src.A);

	out.R = (src.R*src.A + dst.R*dst.A * (1.0 - src.A)) / out.A;
	out.G = (src.G*src.A + dst.G*dst.A * (1.0 - src.A)) / out.A;
	out.B = (src.B*src.A + dst.B*dst.A * (1.0 - src.A)) / out.A;

	return out.Normalize();
}

// ugh, not sure why, but the abs() func returns an int...
inline double absolute(double x)
{
	return x >= 0.0 ? x : -x;
}

double Colour::Distance(const Colour& A, const Colour& B)
{
	double dist_r = absolute(A.R - B.R);
	double dist_g = absolute(A.G - B.G);
	double dist_b = absolute(A.B - B.B);
	double dist_a = absolute(A.A - B.A);

	return dist_r + dist_g + dist_b + dist_a;
}

const Colour& Colour::None()
{
	static Colour none = Colour(0, 0, 0, 0);

	return none;
}

Colour Colour::operator+(const Colour& A) const
{
	return Colour(this->R + A.R, this->G + A.G, this->B + A.B, this->A + A.A).Normalize();
}

Colour Colour::operator-(const Colour& A) const
{
	return Colour(this->R - A.R, this->G - A.G, this->B - A.B, this->A - A.A).Normalize();
}

Colour& Colour::operator+=(const Colour& A)
{
	this->R += A.R;
	this->G += A.G;
	this->B += A.B;
	this->A += A.A;

	return this->Normalize();
}

Colour& Colour::operator-=(const Colour& A)
{
	this->R -= A.R;
	this->G -= A.G;
	this->B -= A.B;
	this->A -= A.A;

	return this->Normalize();
}

bool Colour::operator==(const Colour& A) const
{
	return this->R == A.R &&
		this->G == A.G &&
		this->B == A.B &&
		this->A == A.A;
}

bool Colour::operator!=(const Colour& A) const
{
	return !(*this == A);
}


// tostring

std::ostream& operator<<(std::ostream& stream, const Size& value)
{
	stream << value.Width << ", " << value.Height;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Vector& value)
{
	stream << value.X << ", " << value.Y;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Colour& value)
{
	if(value.A == 1.0)
		stream << "rgb(" << value.R << ", " << value.G << ", " << value.B << ")";
	else
		stream << "rgba(" << value.R << ", " << value.G << ", " << value.B << ", " << value.A << ")";
	
	return stream;
}
