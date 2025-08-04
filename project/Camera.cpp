#include "Camera.h"
#include "WinApp.h"

Camera::Camera()
	: transform({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} })
	, fovY(0.45f)
	, aspect(float(WinApp::kClientWidth / float(WinApp::kClientHeight)))
	, nearClipDistance(0.1f)
	, farClipDistance(100.0f)
	, worldMatrix(MakeAffineMatrix(transform.scale, transform.rotate, transform.translate))
	, viewMatrix(Inverse(worldMatrix))
	, projectionMatrix(MakePerspectiveFovMatrix(fovY, aspect, nearClipDistance, farClipDistance))
	, viewProjectionMatrix(Multiply(viewMatrix, projectionMatrix))
{}


void Camera::Update() 
{
	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspect, nearClipDistance, farClipDistance);

	viewMatrix = Inverse(worldMatrix);

	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}

