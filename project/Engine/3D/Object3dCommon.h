#pragma once
#include "DirectXCommon.h"
#include "Camera.h"

// 3Dオブジェクト共通部
class ObJect3dCommon {
public:
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDxCommon()const { return dxCommon_; }

	void SettingCommonDraw();

	// Setter(Camera)
	void SetDefaultCamera(Camera* camera) { defaultCamera = camera; }

	// Getter(Camera)
	Camera* GetDefaultCamera() const { return defaultCamera; }

private:
	// ルートシグネチャの作成
	void createRootSignature();
	// グラフィックスパイプラインの生成
	void GeneratePipeline();

private:
	DirectXCommon* dxCommon_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
	Camera* defaultCamera = nullptr;
};