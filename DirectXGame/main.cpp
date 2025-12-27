#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// エンジンの初期化
	KamataEngine::Initialize(L"LC1A_31_ヤマモト_ルナ_AL2");

	// ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// ゲームシーンのインスタンス作成
	GameScene* gameScene = new GameScene();

	// ゲームシーンの初期化
	gameScene->Initialize();

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();	

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		imguiManager->Begin();

		// ゲームシーンの更新
		gameScene->Update();

		imguiManager->End();	

		// 描画開始
		dxCommon->PreDraw();

		// ゲームシーンの描画
		gameScene->Draw();

		AxisIndicator::GetInstance()->Draw();

		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// エンジンの終了処理
	KamataEngine::Finalize();

	// ゲームシーンの開放
	delete gameScene;

	// nullptrの代入
	gameScene = nullptr;

	return 0;
}
