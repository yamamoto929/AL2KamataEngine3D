#include "GameScene.h"
#include "2d/ImGuiManager.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	// delete sprite_;
	delete model_;
	delete debugCamera_;
};

// 初期化
void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");
	// sprite_ = Sprite::Create(textureHandle_, {100, 50});

	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_);

	playHandle_ = 0;

	//PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
};

// 更新処理
void GameScene::Update() {
	/*Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);*/

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(playHandle_);
	}

	if (!Audio::GetInstance()->IsPlaying(playHandle_)) {
		playHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, false);
	}

#ifdef _DEBUG
	//ImGui::ShowDemoWindow();
#endif
};

// 描画処理
void GameScene::Draw() {
	// Sprite::PreDraw();
	//  sprite_->Draw();
	// Sprite::PostDraw();
	debugCamera_->Update();

	Model::PreDraw();

	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);

	// 3Dモデル描画後処理
	Model::PostDraw();

	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
};