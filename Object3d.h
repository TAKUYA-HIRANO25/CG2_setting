#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include "Mymath.h"

using namespace MyMath;

class ObJect3dCommon;
class Model;

// 3Dオブジェクト
class Object3d {
public:

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};
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
};