#include"Object3d.h"
#include "Object3dCommon.h"
#include <cassert>
#include "TextureManager.h"
#include "Model.h"
#include "ModelManager.h"

void Object3d::Initialize(ObJect3dCommon* object3dCommon)
{
	// 引数で受け取ってメンバ変数に記録する
	this->object3dCommon = object3dCommon;

	// 座標変換行列リソースを作る
	transformationMatrixResource = object3dCommon->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	// データを書き込む
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();

	// 平行光源用のリソースを作る
	directionalLightResource = object3dCommon->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	DirectionalLight light{ {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };
	directionalLightData->color = light.color;
	directionalLightData->direction = light.direction;
	directionalLightData->intensity = light.intensity;

	// Transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,185.3f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };
}

void Object3d::Updata()
{
	//transform.rotate.y += 0.05f;

	transformationMatrixData->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(transformationMatrixData->World, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
}

void Object3d::Draw()
{
	// 座標変換行列CBufferの場所を設定
	object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	// 平行光源CBufferの場所を設定
	object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	// 3Dモデルが割り当てられていれば描画する
	if (model) {
		model->Draw();
	}
}

void Object3d::SetModel(const std::string& filePath)
{
	// モデルを検索してセットする
	model = ModelManager::GetInstance()->FindModel(filePath);
}
