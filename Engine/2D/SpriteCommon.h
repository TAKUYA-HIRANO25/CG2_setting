#pragma once
#include "DirectXCommon.h"

class SpriteCommon{
public:
	void Initialize(DirectXCommon* dxCommon);

	void SettingCommonDraw();

	DirectXCommon* getDxCommon()const { return dxCommon_; }

private:

	void RootSignatureInitialize();

	void GeneratePipelineInitialize();

	DirectXCommon* dxCommon_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
};
