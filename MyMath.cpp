#include "MyMath.h"

namespace MyMath {
	Matrix4x4 MyMath::MakeIdentity4x4()
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					result.m[i][j] = 1;
				}
				else {
					result.m[i][j] = 0;
				}
			}
		}
		return result;
	}

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m1.m[i][k] * m2.m[k][j];
				}
			}
		}
		return result;
	}

	Matrix4x4 MakeScalematrix(const Vector3& scale)
	{
		return Matrix4x4();
	}

	Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
	{
		return Matrix4x4();
	}

	Matrix4x4 MakeRotateXMatrix(float radian)
	{
		return Matrix4x4();
	}

	Matrix4x4 MakeRotateYMatrix(float radian)
	{
		return Matrix4x4();
	}

	Matrix4x4 MakeRotateZMatrix(float radian)
	{
		return Matrix4x4();
	}

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
	{
		return Vector3();
	}

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
	{
		return Matrix4x4();
	}

	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip)
	{
		return Matrix4x4();
	}

	Matrix4x4 MakeViewportmatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
	{
		return Matrix4x4();
	}

	Matrix4x4 Inverse(const Matrix4x4& m)
	{
		return Matrix4x4();
	}

	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
	{
		return Matrix4x4();
	}





}