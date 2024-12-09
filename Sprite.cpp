#include "Sprite.h"
#include "SpriteCommon.h"


void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	this->spriteCommon_ = spriteCommon;

	// VertexResouceを作る
	vertexResource = spriteCommon->getDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);
	// VertexBufferViewを作成
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	// アドレスを取得して書き込む
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	// IndexResourceを作る
	indexResource = spriteCommon->getDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);
	// IndexBufferViewを作成
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	// アドレスを取得して書き込む
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	// マテリアルリソースを作る
	materialResource = spriteCommon->getDxCommon()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// マテリアルデータの初期値を書き込む
	Vector4 color{ 1.0f,1.0f,1.0f,1.0f };
	materialData->color = color;
	materialData->enableLighting = false;
	materialData->uvTransform = MakeIdentity4x4();
	// 座標変換行列リソースを作る
	transformationMatrixResource = spriteCommon->getDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	// データを書き込む
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();
	textureSrvHandleGPU = spriteCommon->getDxCommon()->GetSRVGPUDescriptorHandle(1);
}

void Sprite::Update()
{
	// 頂点リソースにデータを書き込む
	vertexData[0].position = { 0.0f,1.0f,0.0f,1.0f };// 左下
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };
	vertexData[1].position = { 0.0f,0.0f,0.0f,1.0f };// 左上
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };
	vertexData[2].position = { 1.0f,1.0f,0.0f,1.0f };// 右下
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
	vertexData[3].position = { 1.0f,0.0f,0.0f,1.0f };// 右上
	vertexData[3].texcoord = { 1.0f,0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };

	// インデックスリソースにデータを書き込む
	indexData[0] = 0;    indexData[1] = 1;    indexData[2] = 2;
	indexData[3] = 1;    indexData[4] = 3;    indexData[5] = 2;

	Transform transform;
	transform.translate = { position.x,position.y ,0.0f };
	transform.rotate = { 0.0f,0.0f,rotation };
	transform.scale = { size.x,size.y,1.0f };
	transformationMatrixData->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(transformationMatrixData->World, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
}

void Sprite::Draw()
{
	// VertexBufferViewを設定
	spriteCommon_->getDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// IndexBufferViewを設定
	spriteCommon_->getDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	spriteCommon_->getDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// 座標変換行列CBufferの場所を設定
	spriteCommon_->getDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	// SRVのDescriptorTarbleの先頭を設定
	spriteCommon_->getDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	spriteCommon_->getDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
