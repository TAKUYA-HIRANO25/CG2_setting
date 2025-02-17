#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include "MyMath.h"

using namespace MyMath;

class ModelCommon;

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
struct MaterialData {
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};

// 3Dモデル
class Model {
public:
	// 初期化
	void Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename);

	// 描画
	void Draw();

	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	ModelCommon* modelCommon = nullptr;

	ModelData modelData;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

};