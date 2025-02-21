#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <chrono>
#include "MyMath.h"
#include "TextureManager.h"
using namespace MyMath;

class SpriteCommon;
class TextureManager;

class Sprite {
public:
	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};
	void Initialize(SpriteCommon* spriteCommon, std::string textureFilePath);

	void Update();

	void Draw();

	const Vector2& GetPosition() const { return position; }
	void SetPosition(const Vector2& position) { this->position = position; }

	float GetRotation()const { return rotation; }
	void SetRoration(float rotation) { this->rotation = rotation; }

	const Vector4& GetColor()const { return materialData->color; }
	void SetColor(const Vector4& color) { materialData->color = color; }

	const Vector2& GetSize()const { return size; }
	void SetSize(const Vector2& size) { this->size = size; }

	const Vector2& GetAnchorPoint() const { return anchorPoint; }
	void SetAnchorPoint(const Vector2& anchorpoint) { this->anchorPoint = anchorpoint; }

	const bool& IsFlipX()const { return isFlipX_; }
	void SetIsFlipX(const bool& isFlipX) { isFlipX_ = isFlipX; }

	const bool& IsFlipY()const { return isFlipY_; }
	void SetIsFlipY(const bool& isFlipY) { isFlipY_ = isFlipY; }

	const Vector2& GetTextureLeftTop()const { return textureLeftTop; }
	void SetTextureLeftTop(const Vector2& textureLeftTop) { this->textureLeftTop = textureLeftTop; }

	const Vector2& GetTextureSize()const { return textureSize; }
	void SetGetTextureSize(const Vector2& textureSize) { this->textureSize = textureSize; }

private:
	//テクスチャーサイズをイメージに合わせる
	void AdjustTextureSize();

	SpriteCommon* spriteCommon_ = nullptr;
	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* transformationMatrixData;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	Vector2 position = { 0.0f,0.0f };
	float rotation = 0.0f;
	Vector2 size = { 120.0f,120.0f };

	// テクスチャ番号
	uint32_t textureIndex = 0;

	//基準点
	Vector2 anchorPoint = { 0.0f,0.0f };

	//左右フリップ
	bool isFlipX_ = false;

	//上下フリップ
	bool isFlipY_ = false;

	// テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };

	// テクスチャ切り出しサイズ
	Vector2 textureSize = { 100.0f,100.0f };

};
