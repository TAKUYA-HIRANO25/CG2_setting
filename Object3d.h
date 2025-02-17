#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include "MyMath.h"

using namespace MyMath;

class ObJect3dCommon;

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct MaterialData {
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};

// 3Dオブジェクト
class Object3d {
public:

	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

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

	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	ObJect3dCommon* object3dCommon = nullptr;

	ModelData modelData;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;
	// バッファリソース内のデータをさすポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* transformationMatrixData = nullptr;
	DirectionalLight* directionalLightData = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	Transform transform;
	Transform cameraTransform;

};