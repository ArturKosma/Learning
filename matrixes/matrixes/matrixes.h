#pragma once

class AFVector;
class AFMathFunctionLibrary;

class AFMatrix3x3
{

public:

	AFMatrix3x3(float InM11, float InM12, float InM13, float InM21, float InM22, float InM23, float InM31, float InM32, float InM33);

	friend std::ostream& operator<<(std::ostream& InOstream, const AFMatrix3x3& InMatrix);

	AFMatrix3x3 GetTranspose() const;
	bool GetIsIdentity() const;

	// Identity matrix.

	float m11 = 1.0f;
	float m12 = 0.0f;
	float m13 = 0.0f;

	float m21 = 0.0f;
	float m22 = 1.0f;
	float m23 = 0.0f;

	float m31 = 0.0f;
	float m32 = 0.0f;
	float m33 = 1.0f;
};

class AFMath2FunctionLibrary : public AFMathFunctionLibrary
{

public:

	static AFVector TransformVector(const AFVector& InVector, const AFMatrix3x3& InMatrix);
	static AFVector TransformVectorMv(const AFMatrix3x3& InMatrix, const AFVector& InVector);
};