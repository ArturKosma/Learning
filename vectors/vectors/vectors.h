#pragma once

class AFMathFunctionLibrary
{
public:

	static inline float RadToDeg(float InRad);

	static inline float DegToRad(float InDeg);
};

class AFVector
{
public:

	AFVector(float InX, float InY, float InZ);
	AFVector(const AFVector& InOther);

	AFVector operator+(const AFVector& InOther);
	AFVector operator-(const AFVector& InOther);
	AFVector operator/(float InDivider);

	inline double GetLength() const;
	inline double GetLengthSq() const;
	inline float GetDot(const AFVector& InOther) const;
	inline AFVector GetCross(const AFVector& InOther) const;
	AFVector GetNormalised() const;

	float GetX() const;
	float GetY() const;
	float GetZ() const;

	friend std::ostream& operator<<(std::ostream& InOstream, const AFVector& InVector);

private:

	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;

};

