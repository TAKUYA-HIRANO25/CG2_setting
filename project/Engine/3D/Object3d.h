#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include "Mymath.h"
#include "Camera.h"
using namespace MyMath;

class ObJect3dCommon;
class Model;

// 3Dオブジェクト
class Object3d {
public:

public:
	// 初期化
	void Initialize(ObJect3dCommon* object3dCommon);

	void Updata();

	void Draw();

	// setter
	void SetModel(const std::string& filePath);
	void SetScale(const Vector3& scale) { transform.scale = scale; }
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetCamera(Camera* camera) { this->camera = camera; }

	// Getter
	const Vector3& GetScale()const { return transform.scale; }
	const Vector3& GetRotate()const { return transform.rotate; }
	const Vector3& GetTranslate()const { return transform.translate; }

private:
	ObJect3dCommon* object3dCommon = nullptr;
	Model* model = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;
	// バッファリソース内のデータをさすポインタ
	TransformationMatrix* transformationMatrixData = nullptr;
	DirectionalLight* directionalLightData = nullptr;

	Transform transform;
	Transform cameraTransform;

	Camera* camera = nullptr;

	Matrix4x4 worldMatrix;
	Matrix4x4 worldViewProjectionMatrix;
};