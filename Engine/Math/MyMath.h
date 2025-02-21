#pragma once


namespace MyMath {
	struct Vector4
	{
		float x;
		float y;
		float z;
		float w;
	};
	struct Vector3
	{
		float x;
		float y;
		float z;
	};
	struct Vector2
	{
		float x;
		float y;
	};
	struct Matrix4x4
	{
		float m[4][4];
	};
	struct Matrix3x3
	{
		float m[3][3];
	};
	//単位行列
	Matrix4x4 MakeIdentity4x4();
	//積
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	//スカラ
	Matrix4x4 MakeScalematrix(const Vector3& scale);
	//移動
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	// x軸
	Matrix4x4 MakeRotateXMatrix(float radian);
	// Y軸
	Matrix4x4 MakeRotateYMatrix(float radian);
	// Z軸
	Matrix4x4 MakeRotateZMatrix(float radian);
	//座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	//SRTの合成
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	//透視投影
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip);
	//ビューポート
	Matrix4x4 MakeViewportmatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	//逆行列
	Matrix4x4 Inverse(const Matrix4x4& m);
	//平行投影
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	//ノーマライズ
	float Length(const Vector3& v);
	Vector3 Normalize(const Vector3& v);
}