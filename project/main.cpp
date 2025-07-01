#define _USE_MATH_DEFINES
#include <math.h>
#include "Engine/Math/MyMath.h"
#include "Engine/Base/Input.h"
#include "Engine/Base/WinApp.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/2D/Sprite.h"
#include "Engine/2D/SpriteCommon.h"
#include "Engine/Base/D3DResourceLeakChecker.h"
#include "Engine/Base/TextureManager.h"
#include "Engine/3D/Object3dCommon.h"
#include "Engine/3D/Object3d.h"
#include "Engine/3D/Model.h"
#include "Engine/3D/ModelCommon.h"
#include "Engine/3D/ModelManager.h"

#pragma comment(lib,"dxcompiler.lib")

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
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");

#pragma endregion
	//スプライト
#pragma region
	SpriteCommon* spriteCommon;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon, "resources/uvChecker.png");

#pragma endregion
	//モデル
#pragma region
	ModelCommon* modelCommon = nullptr;
	modelCommon = new ModelCommon;
	modelCommon->Initialize(dxCommon);

	Model* model = nullptr;
	model = new Model;
	model->Initialize(modelCommon, "resources", "plane.obj");

	ObJect3dCommon* object3dCommon = nullptr;
	object3dCommon = new ObJect3dCommon;
	object3dCommon->Initialize(dxCommon);

	ModelManager::GetInstance()->Initialize(dxCommon);

	// .ojbファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	// 異なるモデルを持つオブジェクトを生成
	Object3d* planeObject = new Object3d;
	planeObject->Initialize(object3dCommon);
	planeObject->SetModel("plane.obj");
	planeObject->SetTranslate(Vector3(-2.0f, 0.0f, 0.0f));

	Object3d* axisObject = new Object3d;
	axisObject->Initialize(object3dCommon);
	axisObject->SetModel("axis.obj");
	axisObject->SetTranslate(Vector3(2.0f, 0.0f, 0.0f));
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
	/*Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = dxCommon->CreateBufferResource(sizeof(DirectiomalLight));
	DirectiomalLight* directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;*/

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

			/*for (Sprite* sprite : sprites) {
				sprite->Update();
			}*/
			sprite->Update();

			object3dCommon->SettingCommonDraw();

			Vector3 currentRotate[2];
			currentRotate[0] = planeObject->GetRotate();
			currentRotate[1] = axisObject->GetRotate();

			currentRotate[0].y += 0.05f;
			currentRotate[1].y = 0.0f;
			currentRotate[1].z += 0.05f;

			planeObject->SetRotate(currentRotate[0]);
			planeObject->Updata();
			axisObject->SetRotate(currentRotate[1]);
			axisObject->Updata();

			//uvTransform
			Matrix4x4 uvTransformMatrix = MakeScalematrix(uvTransformSprite.scale);
			uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
			uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));

			

			//球の３次元化 WVPスフィア用
			/*transformSphere.rotate.y += 0.03f;
			Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transformSphere.scale, transformSphere.rotate, transformSphere.translate);
			Matrix4x4 viewMatrixSphere = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrixSphere = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
			Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrixSphere, projectionMatrixSphere));
			transformationMatrixDataSphere->WVP = worldViewProjectionMatrixSphere;
			transformationMatrixDataSphere->World = worldMatrixSphere;*/
			
			ImGui::Render();

			//画面色変更
#pragma region

			dxCommon->PreDraw();

			spriteCommon->SettingCommonDraw();

			//スフィア描画
			/*dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere);
			dxCommon->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
			dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere->GetGPUVirtualAddress());
			//dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonsterball ? texturSrvHandleGPU2 : texturSrvHandleGPU);
			//dxCommon->GetCommandList()->DrawInstanced(Subdivision * Subdivision * 6, 1, 0, 0);*/

			//スプライト描画
			/*for (Sprite* sprite : sprites) {
				sprite->Draw();
			}*/
			sprite->Draw();

			planeObject->Draw();
			axisObject->Draw();

			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandList());
			dxCommon->PostDrow();
#pragma endregion
		}
	}
	//解放
	/*for (Sprite* sprite : sprites) {
		delete sprite;
	}*/
	delete sprite;
	delete spriteCommon;
	delete input;
	delete dxCommon;
	delete axisObject;
	delete planeObject;
	ModelManager::GetInstance()->Finalize();
	delete object3dCommon;
	delete model;
	delete modelCommon;
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