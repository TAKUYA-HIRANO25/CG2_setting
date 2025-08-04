#pragma once
#include "Mymath.h"

using namespace MyMath;

class Camera
{
public:
	Camera();

	// 初期化
	void Update();

	// Setter(Rotate)
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetFarClipDistance(const float& farClip) { farClipDistance = farClip; }
	void SetFovY(const float& fov) { fovY = fov; }
	void SetTransform(const Transform& transform) { this->transform = transform; }

	// Getter
	const Matrix4x4& GetWorldMatrix() const {
		Matrix4x4 world = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		return world;
	}
	const Matrix4x4& GetViewMatrix() const { return viewMatrix; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
	const Vector3& GetRotate() const { return transform.rotate; }
	const Vector3& GetTranslate() const { return transform.translate; }
	const float& GetFarClipDistance() const { return farClipDistance; }
	const float& GetFovY() const { return fovY; }
	const Transform& GetTransform() const { return transform; }

private:
	Transform transform;
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;

	Matrix4x4 projectionMatrix;
	float fovY;
	float aspect;
	float nearClipDistance;
	float farClipDistance;

	Matrix4x4 viewProjectionMatrix;
};
