#include <iostream>

#include "../../vectors/vectors/vectors.h"
#include "matrixes.h"


int main()
{
    AFMatrix3x3 myMatrix = AFMatrix3x3(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );

    AFVector myVector = AFVector(1.0f, 0.5f, 2.5f);

    std::cout << AFMath2FunctionLibrary::TransformVector(myVector, myMatrix) << std::endl;

    std::cout << AFMath2FunctionLibrary::TransformVectorMv(myMatrix.GetTranspose(), myVector) << std::endl;
}

std::ostream& operator<<(std::ostream& InOstream, const AFMatrix3x3& InMatrix)
{
    return std::cout 
        << "|" << InMatrix.m11 << ", " << InMatrix.m12 << ", " << InMatrix.m13 << "|" << std::endl
        << "|" << InMatrix.m21 << ", " << InMatrix.m22 << ", " << InMatrix.m23 << "|" << std::endl
        << "|" << InMatrix.m31 << ", " << InMatrix.m32 << ", " << InMatrix.m33 << "|";
}

AFMatrix3x3::AFMatrix3x3(float InM11, float InM12, float InM13, float InM21, float InM22, float InM23, float InM31, float InM32, float InM33)
    : m11(InM11), m12(InM12), m13(InM13), m21(InM21), m22(InM22), m23(InM23), m31(InM31), m32(InM32), m33(InM33)
{

}

AFMatrix3x3 AFMatrix3x3::GetTranspose() const
{
    // MT.

    return AFMatrix3x3(
        m11, m21, m32,
        m12, m22, m32,
        m13, m23, m33
    );
}

bool AFMatrix3x3::GetIsIdentity() const
{
    if (m11 != 1.0f || m22 != 1.0f || m33 != 1.0f)
    {
        return false;
    }

    if (m12 || m13 || m21 || m23 || m31 || m32)
    {
        return false;
    }

    return true;
}

AFVector AFMath2FunctionLibrary::TransformVector(const AFVector& InVector, const AFMatrix3x3& InMatrix)
{
    // vM.

    //        [m11 m12 m12]
    // [x y z][m21 m22 m23] = [xm11 + ym21 + z31 xm12 + ym22 + zm23 xm13 + ym23 + zm33]
    //        [m31 m32 m33]

    return AFVector((InVector.GetX() * InMatrix.m11 + InVector.GetY() * InMatrix.m21 + InVector.GetZ() * InMatrix.m31),
        (InVector.GetX() * InMatrix.m12 + InVector.GetY() * InMatrix.m22 + InVector.GetZ() * InMatrix.m32),
        (InVector.GetX() * InMatrix.m13 + InVector.GetY() * InMatrix.m23 + InVector.GetZ() * InMatrix.m33));
}

AFVector AFMath2FunctionLibrary::TransformVectorMv(const AFMatrix3x3& InMatrix, const AFVector& InVector)
{
	// Mv.

	// [m11 m12 m12][x]   [m11x + m12y + m13z]
	// [m21 m22 m23][y] = [m21x + m22y + m23z]
	// [m31 m32 m33][z]   [m31x + m32y + m33z]

    return AFVector((InMatrix.m11 * InVector.GetX() + InMatrix.m12 * InVector.GetY() + InMatrix.m13 * InVector.GetZ()),
        (InMatrix.m21 * InVector.GetX() + InMatrix.m22 * InVector.GetY() + InMatrix.m23 * InVector.GetZ()),
        (InMatrix.m31 * InVector.GetX() + InMatrix.m32 * InVector.GetY() + InMatrix.m33 * InVector.GetZ()));
}
