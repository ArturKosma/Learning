#include <iostream>
#include <iomanip>
#include <cmath>

#include "vectors.h"

AFVector::AFVector(float InX, float InY, float InZ)
    : X(InX), Y(InY), Z(InZ)
{

}

AFVector::AFVector(const AFVector& InOther)
    : X(InOther.X), Y(InOther.Y), Z(InOther.Z)
{

}

AFVector AFVector::operator+(const AFVector& InOther)
{
    X += InOther.X;
    Y += InOther.Y;
    Z += InOther.Z;

    return *this;
}

AFVector AFVector::operator-(const AFVector& InOther)
{
	X -= InOther.X;
	Y -= InOther.Y;
	Z -= InOther.Z;

	return *this;
}

AFVector AFVector::operator/(float InDivider)
{
    X /= InDivider;
    Y /= InDivider;
    Z /= InDivider;

    return *this;
}

double AFVector::GetLength() const
{
    return sqrt(GetLengthSq());
}

double AFVector::GetLengthSq() const
{
    return (pow(X, 2.0f) + pow(Y, 2.0f) + pow(Z, 2.0f));
}

float AFVector::GetDot(const AFVector& InOther) const
{
    return (X * InOther.X) + (Y * InOther.Y) + (Z * InOther.Z);
}

AFVector AFVector::GetCross(const AFVector& InOther) const
{
    return AFVector(Y * InOther.Z - Z * InOther.Y, Z * InOther.X - X * InOther.Z, X * InOther.Y - Y * InOther.X);
}

AFVector AFVector::GetNormalised() const
{
    return *const_cast<AFVector*>(this) / (float)GetLength();
}

float AFVector::GetX() const
{
    return X;
}

float AFVector::GetY() const
{
    return Y;
}

float AFVector::GetZ() const
{
    return Z;
}

std::ostream& operator<<(std::ostream& InOstream, const AFVector& InVector)
{
    return InOstream << "(" << InVector.X << "," << InVector.Y << "," << InVector.Z << ")";
}

float AFMathFunctionLibrary::RadToDeg(float InRad)
{
    return InRad * (180.0f / 3.14159f);
}

float AFMathFunctionLibrary::DegToRad(float InDeg)
{
    return InDeg * (3.14159f / 180.0f);
}
