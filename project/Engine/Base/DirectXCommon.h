#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <format>
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include <dxcapi.h>
#include "StringUility.h"
#include "Logger.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex//d3dx12.h"
#include <vector>
#include <chrono>
#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "dxcompiler.lib")

class DirectXCommon 
{
public:
	void Initialize(WinApp* winApp);

	/// DepthStencilTextureの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const int32_t& width, const int32_t& height);
	
	/// デスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, const UINT& numDescriptors, bool shaderVisible);

	/// 指定番号のCPUデスクリプタハンドルを取得する
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const uint32_t& descriptorSize, const uint32_t& index);

	/// 指定番号のGPUデスクリプタハンドルを取得する
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const uint32_t& descriptorSize, const uint32_t& index);

	/// SRVのしてh番号のCPUデスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

	/// SRVのしてh番号のGPUデスクリプタハンドルを取得する
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);
	
	//ゲッター
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

	// シェーダーコンパイル
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath,const wchar_t* profile);

	/// バッファリソースの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
	
	//テクスチャーリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResourece(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);
	
	//リソースのデータ転送
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

	//テクスチャー関数
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	void PreDraw();

	void PostDrow();

	//最大テクスチャ枚数
	static const uint32_t kMaxSRVCount;

private:
	/// デバイスの初期化
	void DeviceInitialize();

	/// コマンド関係の初期化
	void CommandInitialize();

	/// スワップチェーンの生成
	void SwapChainInitialize();

	/// 深度バッファの生成
	void DepthBufferInitialize();

	/// 各種デスクリプタヒープの生成
	void DescriptorHeapInitialize();

	/// レンダーターゲットビューの初期化
	void RenderTargetViewInitialize();
	
	/// 深度ステンシルビューの初期化
	void DepthStencilViewInitialize();

	/// フェンスの生成
	void FenceInitialize();

	/// ビューポート矩形の生成
	void ViewportInitialize();

	/// シザリング矩形の生成
	void ScissoringInitialize();

	/// DXCコンパイラの生成
	void DXCInitialize();

	/// ImGuiの生成
	void ImGuiInitialize();

	// 記録時間
	std::chrono::steady_clock::time_point reference_;

	// FPS固定初期化
	void InitialiseFixFPS();
	/// FPS固定更新
	void UpdateFixFPS();
private:
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	WinApp* winApp = nullptr;

	// RTV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;

	// SRV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;

	// DSV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;

	// 各種DescroptorSize
	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;

	// SRVのデスクリプタハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// スワップチェーンリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2];

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	HANDLE fenceEvent;

	// ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};

	// フェンス
	uint64_t fenceValue = 0;

	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;

	//中間
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> intermediateResources_;
};
