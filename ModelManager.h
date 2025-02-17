#pragma once

#include "DirectXCommon.h"
#include <wrl.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <map>

class Model;

class ModelCommon;

// モデルマネージャー
class ModelManager {
private:
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager& other) = delete;

public:
	// シングルトンインスタンスの取得
	static ModelManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	///  <summary>{
	///  モデルファイルの読み込み
	/// </summary>
	//<param name="filepath">モデルファイルのパス</param>
	void LoadModel(const std::string& filePath);

	/// <summary>
	/// モデルの検索
	/// </summary>
	/// <param name="filePath">モデルファイルのパス</param>
	/// <return>モデル</return>
	Model* FindModel(const std::string& filePath);

private:
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models;

	ModelCommon* modelCommon = nullptr;
};