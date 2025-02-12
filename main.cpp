#include <cmath>
#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "MyMath.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "D3DResourceLeakChecker.h"
#include "TextureManager.h"

#pragma comment(lib,"dxcompiler.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct DirectiomalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};
struct MaterialData {
	std::string textureFilePath;
};
struct ModelData {
	std::vector<Sprite::VertexData> vertices;
	MaterialData material;
};
//球
struct Sphere {
	Vector3 center;
	float radius;
};
//球
void DrawSphere(Sprite::VertexData* vertexData, uint32_t Subdivision) {
	const uint32_t kSubdivision = Subdivision;
	const float kLonEvery = float(M_PI) * 2.0f / float(kSubdivision);//経度 φ
	const float kLatEvery = float(M_PI) / float(kSubdivision);	//緯度 θ


	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = float(M_PI) / 2.0f + kLatEvery * latIndex;//θ

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//φ

			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			Sprite::VertexData vertA = {
				{
					cos(lat) * cos(lon) ,
					sin(lat),
					cos(lat) * sin(lon),
					1.0f
				},
				{ float(lonIndex) / float(kSubdivision), 1.0f + float(latIndex) / float(kSubdivision) },
				{
					cos(lat) * cos(lon) ,
					sin(lat),
					cos(lat) * sin(lon),
				},
			};
			Sprite::VertexData vertB = {
				{
					cos(lat + kLatEvery) * cos(lon) ,
					sin(lat + kLatEvery),
					cos(lat + kLatEvery) * sin(lon),
					1.0f
				} ,
				{ float(lonIndex) / float(kSubdivision), 1.0f + float(latIndex + 1) / float(kSubdivision) },
				{
					cos(lat + kLatEvery) * cos(lon) ,
					sin(lat + kLatEvery),
					cos(lat + kLatEvery) * sin(lon),
				} ,
			};
			Sprite::VertexData vertC = {
				{
					cos(lat) * cos(lon + kLonEvery) ,
					sin(lat),
					cos(lat) * sin(lon + kLonEvery),
					1.0f
				},
				{ float(lonIndex + 1) / float(kSubdivision), 1.0f + float(latIndex) / float(kSubdivision) },
				{
					cos(lat) * cos(lon + kLonEvery) ,
					sin(lat),
					cos(lat) * sin(lon + kLonEvery),
				},
			};
			Sprite::VertexData vertD = {
				{
					cos(lat + kLatEvery) * cos(lon + kLonEvery),
					sin(lat + kLatEvery),
					cos(lat + kLatEvery) * sin(lon + kLonEvery),
					1.0f
				},
				{ float(lonIndex + 1) / float(kSubdivision), 1.0f + float(latIndex + 1) / float(kSubdivision) } ,
				{
					cos(lat + kLatEvery) * cos(lon + kLonEvery),
					sin(lat + kLatEvery),
					cos(lat + kLatEvery) * sin(lon + kLonEvery),
				},
			};

			vertexData[start + 5] = vertA;  //左下 A

			vertexData[start + 4] = vertB;  //上 B

			vertexData[start + 3] = vertC;  //右下 C

			vertexData[start + 2] = vertC;  //左下2 C

			vertexData[start + 1] = vertB;  //上2 B

			vertexData[start + 0] = vertD;  //右下2 D

		}
	}
}
//Textureデータ
MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	MaterialData materialData;
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}
//objファイル読み込み 
ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	//宣言
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;
	//ファイル開け
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());
	//ファイル読み込み
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;
		//頂点情報
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.x *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			Sprite::VertexData triangle[3];
			//三角形を作る
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDifinition;
				s >> vertexDifinition;
				//頂点の要素Indexの取得
				std::istringstream v(vertexDifinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			modelData.vertices.push_back(triangle[2]); //2
			modelData.vertices.push_back(triangle[1]); //1
			modelData.vertices.push_back(triangle[0]); //0
		}
		else if (identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
	return modelData;
}
//Transform
Sprite::Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
Sprite::Transform cameraTransfprm{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakChek;
	//WinAPI
#pragma region 
	WinApp* winApp = nullptr;
	winApp = new WinApp();
	winApp->Initialize();

#pragma endregion
	//DirectCommon
#pragma region 
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

#pragma endregion
	//キー入力
#pragma region
	Input* input;
	input = new Input();
	input->Initialize(winApp);
#pragma endregion
	//テクスチャー
#pragma region
	TextureManager::GetInstance()->Initialize(dxCommon);
	// Textureを読んで転送する
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");

#pragma endregion
	//スプライト
#pragma region
	SpriteCommon* spriteCommon;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	std::vector<Sprite*> sprites;
	for (uint32_t i = 0; i < 5; ++i) {
		Sprite* sprite = new Sprite();
		std::string filePath;
		if (i % 2 == 0) {
			filePath = "resources/uvChecker.png";
		}
		else {
			filePath = "resources/monsterBall.png";
		}
		sprite->Initialize(spriteCommon, filePath);

		Vector2 newPosition = { float(i * 180), 0 };
		sprite->SetPosition(newPosition);

		sprites.push_back(sprite);
	}
#pragma endregion
	//頂点リソース作成
#pragma region
	ModelData modeData = LoadObjFile("resources", "plane.obj");
#pragma endregion
	//テクスチャー
#pragma region
	//読み込み3
	DirectX::ScratchImage mipImages3 = dxCommon->LoadTexture(modeData.material.textureFilePath);
	const DirectX::TexMetadata& metadata3 = mipImages3.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource3 = dxCommon->CreateTextureResourece(dxCommon->GetDevice(), metadata3);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource3 = dxCommon->UploadTextureData(textureResource3.Get(), mipImages3);

	//SRVの設定3
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc3{};
	srvDesc3.Format = metadata3.format;
	srvDesc3.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc3.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc3.Texture2D.MipLevels = UINT(metadata3.mipLevels);

	//SRVの場所決め3
	D3D12_CPU_DESCRIPTOR_HANDLE texturSrvHandleCPU3 = dxCommon->GetSRVCPUDescriptorHandle(3);
	D3D12_GPU_DESCRIPTOR_HANDLE texturSrvHandleGPU3 = dxCommon->GetSRVGPUDescriptorHandle(3);
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource3.Get(), &srvDesc3, texturSrvHandleCPU3);

	/*
	//読み込み2
	DirectX::ScratchImage mipImages2 = dxCommon->LoadTexture("resources/monsterBall.png");
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource2 = dxCommon->CreateTextureResourece(dxCommon->GetDevice(), metadata2);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource2 = dxCommon->UploadTextureData(textureResource2.Get(), mipImages2);

	//SRVの設定2
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	//SRVの場所決め2
	D3D12_CPU_DESCRIPTOR_HANDLE texturSrvHandleCPU2 = dxCommon->GetSRVCPUDescriptorHandle(2);
	D3D12_GPU_DESCRIPTOR_HANDLE texturSrvHandleGPU2 = dxCommon->GetSRVGPUDescriptorHandle(2);
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, texturSrvHandleCPU2);

	//読み込み
	DirectX::ScratchImage mipImages = dxCommon->LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = dxCommon->CreateTextureResourece(dxCommon->GetDevice(), metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = dxCommon->UploadTextureData(textureResource.Get(), mipImages);

	//SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVの場所決め
	D3D12_CPU_DESCRIPTOR_HANDLE texturSrvHandleCPU = dxCommon->GetSRVCPUDescriptorHandle(1);
	D3D12_GPU_DESCRIPTOR_HANDLE texturSrvHandleGPU = dxCommon->GetSRVGPUDescriptorHandle(1);
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, texturSrvHandleCPU);
	*/

#pragma endregion
#pragma region
	Sprite::TransformationMatrix* wvpData = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = dxCommon->CreateBufferResource(sizeof(Sprite::VertexData) * modeData.vertices.size());
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = dxCommon->CreateBufferResource(sizeof(Sprite::TransformationMatrix));
	//Sprite::TransformationMatrix* wvpData = nullptr;
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
#pragma endregion
	//頂点バッファビューを作成
#pragma region
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(Sprite::VertexData) * modeData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(Sprite::VertexData);
#pragma endregion
	//頂点リソースに書き込み
#pragma region
	Sprite::VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	std::memcpy(vertexData, modeData.vertices.data(), sizeof(Sprite::VertexData) * modeData.vertices.size());

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = dxCommon->CreateBufferResource(sizeof(Sprite::Material));
	Sprite::Material* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->enableLighting = 1;
	materialData->uvTransform = MakeIdentity4x4();


#pragma endregion
	//リソース用頂点リソース
#pragma region
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSprite = dxCommon->CreateBufferResource(sizeof(Sprite::VertexData) * 4);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	vertexBufferViewSprite.SizeInBytes = sizeof(Sprite::VertexData) * 4;
	vertexBufferViewSprite.StrideInBytes = sizeof(Sprite::VertexData);
	//頂点データ
	Sprite::VertexData* vertexDataSprite = nullptr;
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite[0].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite[1].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite[2].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[3].position = { 640.0f,0.0f,0.0f,1.0f };//右上
	vertexDataSprite[3].texcoord = { 1.0f,0.0f };
	vertexDataSprite[3].normal = { 0.0f,0.0f,-1.0f };


	//Transform
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResourceSprite = dxCommon->CreateBufferResource(sizeof(Sprite::TransformationMatrix));
	Matrix4x4* transformationMatrixDataSprite = nullptr;
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	*transformationMatrixDataSprite = MakeIdentity4x4();
	//CPU用Transform
	struct Sprite::Transform transformSprite { { 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f } };

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSprite = dxCommon->CreateBufferResource(sizeof(Sprite::Material));
	Sprite::Material* materialDataSprite = nullptr;
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	materialDataSprite->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataSprite->enableLighting = 1;
	materialDataSprite->uvTransform = MakeIdentity4x4();

	//WVPスプライト用
	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSorite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
	*transformationMatrixDataSprite = worldViewProjectionMatrixSorite;

#pragma endregion
	//インデックス
#pragma region
	/*Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite = dxCommon->CreateBufferResource(sizeof(uint32_t) * 6);
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;
	//データを送る
	uint32_t* indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;
	indexDataSprite[4] = 3;
	indexDataSprite[5] = 2;*/
#pragma endregion
	//スフィア用リソース
#pragma region
	/*const uint32_t Subdivision = 16;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSphere = dxCommon->CreateBufferResource( sizeof(Sprite::VertexData) * Subdivision * Subdivision * 6);
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResourceSphere = dxCommon->CreateBufferResource( sizeof(Matrix4x4));
	Matrix4x4* wvpDataSphere = nullptr;
	wvpResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataSphere));
	*wvpDataSphere = MakeIdentity4x4();
	//頂点バッファビューを作成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere{};
	vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();
	vertexBufferViewSphere.SizeInBytes = sizeof(Sprite::VertexData) * Subdivision * Subdivision * 6;
	vertexBufferViewSphere.StrideInBytes = sizeof(Sprite::VertexData);
	//頂点リソースに書き込み
	Sprite::VertexData* vertexDataSphere = nullptr;
	vertexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere));
	DrawSphere(vertexDataSphere, Subdivision);
	//Transform
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResourceSphere = dxCommon->CreateBufferResource( sizeof(Sprite::TransformationMatrix));
	Sprite::TransformationMatrix* transformationMatrixDataSphere = nullptr;
	transformationMatrixResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere));
	transformationMatrixDataSphere->World = MakeIdentity4x4();*/

	//平行光源
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = dxCommon->CreateBufferResource(sizeof(DirectiomalLight));
	DirectiomalLight* directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;

	//スフィア用Transform
	/*struct Sprite::Transform transformSphere { { 1.0f, 1.0f, 1.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,5.0f} };

	//スフィア用インデックス
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSphere = dxCommon->CreateBufferResource( sizeof(uint32_t) * 6);
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSphere{};
	indexBufferViewSphere.BufferLocation = indexResourceSphere->GetGPUVirtualAddress();
	indexBufferViewSphere.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferViewSphere.Format = DXGI_FORMAT_R32_UINT;
	//データを送る
	uint32_t* indexDataSphere = nullptr;
	indexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSphere));
	indexDataSphere[0] = 0;
	indexDataSphere[1] = 1;
	indexDataSphere[2] = 2;
	indexDataSphere[3] = 1;
	indexDataSphere[4] = 3;
	indexDataSphere[5] = 2;*/
#pragma endregion
	//デバッグ画面初期化
#pragma region
	float materialDataVector[4] = { 1,1,1,1 };
	float TransformScale[3] = { 1.0f,1.0f,1.0f };
	float TransformRotae[3] = { 0.0f, 3.14f, 0.0f };
	float TransformTranslate[3] = { 0.0f,0.0f,0.0f };
	float directionalLight[3] = { 0.0f,-1.0f,0.0f };
	//uvTransform
	struct Sprite::Transform uvTransformSprite {
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
	};
	bool useMonsterball = false;
#pragma endregion

	//ゲーム処理
	while (true)
	{
		if (winApp->ProcessMessage()) {
			break;
		}
		else {
			//imgui
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			//ImGui::ShowDemoWindow();
			ImGui::Checkbox("useMonsterBall", &useMonsterball);
			ImGui::DragFloat4("materialData", materialDataVector);
			ImGui::DragFloat3("Scale", TransformScale);
			ImGui::DragFloat3("Rotae", TransformRotae, 0.1f);
			ImGui::DragFloat3("Translate", TransformTranslate);
			ImGui::DragFloat3("directionalLight", directionalLight, 0.1f);
			//ImGui::DragFloat2("UVTransform", &uvTransformSprite.transform.x, 0.01f, -10.0f, 10.0f);
			//ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
			//ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);

			input->Update();
			if (input->TriggerKey(DIK_0)) {
				OutputDebugStringA("HIT0\n");
			}

			for (Sprite* sprite : sprites) {
				sprite->Update();
			}

			//TransformRotae[1] += 0.01f;
			transform.scale = { TransformScale[0],TransformScale[1],TransformScale[2] };
			transform.rotate = { TransformRotae[0],TransformRotae[1],TransformRotae[2] };
			transform.translate = { TransformTranslate[0],TransformTranslate[1],TransformTranslate[2] };
			directionalLightData->direction = { directionalLight[0],directionalLight[1] ,directionalLight[2] };
			directionalLightData->direction = Normalize(directionalLightData->direction);

			//uvTransform
			Matrix4x4 uvTransformMatrix = MakeScalematrix(uvTransformSprite.scale);
			uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
			uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
			materialDataSprite->uvTransform = uvTransformMatrix;

			//三角形３次元化
			//transform.rotate.y += 0.03f;
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransfprm.scale, cameraTransfprm.rotate, cameraTransfprm.translate);
			Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
			Matrix4x4 mulViewProjection = Multiply(viewMatrix, projectionMatrix);
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, mulViewProjection);
			wvpData->WVP = worldViewProjectionMatrix;
			wvpData->World = worldMatrix;

			//球の３次元化 WVPスフィア用
			/*transformSphere.rotate.y += 0.03f;
			Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transformSphere.scale, transformSphere.rotate, transformSphere.translate);
			Matrix4x4 viewMatrixSphere = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrixSphere = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
			Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrixSphere, projectionMatrixSphere));
			transformationMatrixDataSphere->WVP = worldViewProjectionMatrixSphere;
			transformationMatrixDataSphere->World = worldMatrixSphere;*/

			ImGui::Render();

			/*dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
			dxCommon->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());*/
			//画面色変更
#pragma region

			dxCommon->PreDraw();

			spriteCommon->SettingCommonDraw();

			////三角形描画
			//// phicsRootSignature(rootSignature.Get());
			////dxCommon->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());
			//dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
			////dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			////三角形の色変更
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
			////dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, texturSrvHandleGPU3);
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
			//dxCommon->GetCommandList()->DrawInstanced(UINT(modeData.vertices.size()), 1, 0, 0);

			//スフィア描画
			/*dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere);
			dxCommon->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
			dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere->GetGPUVirtualAddress());
			//dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonsterball ? texturSrvHandleGPU2 : texturSrvHandleGPU);
			//dxCommon->GetCommandList()->DrawInstanced(Subdivision * Subdivision * 6, 1, 0, 0);*/

			//スプライト描画
			for (Sprite* sprite : sprites) {
				sprite->Draw();
			}
			//sprites[0]->Draw();

			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandList());
			dxCommon->PostDrow();
#pragma endregion
		}
	}
	//解放
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	delete spriteCommon;
	delete input;
	delete dxCommon;
	TextureManager::GetInstance()->Finalize();
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#ifndef _DEBUG

#endif _DEBUG
	winApp->Finalize();
	delete winApp;

	return 0;
};