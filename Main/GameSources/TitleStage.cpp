#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleStage::OnCreate() {

		CreateViewLight();
		// プレイヤーの作成
		CreateCharacter();
		// タイトルのロゴ等の配置
		CreateTitleResources();
	}
	void TitleStage::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();
		m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->UpdateAnimation(delta);
		// フェードの再生
		FadePlay();

	}
	void TitleStage::CreateViewLight(){
		auto ptrView = CreateView<SingleView>();
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	void TitleStage::CreateCharacter()
	{
		// 初期座標の定義
		const Vec3 StartPosition = Vec3(0.0f, -4.5f, -5.0f);
		// 大きさの定義
		const float Player_Scale = 1.5f;
		// アニメーションの長さを定義
		const int Player_AnimLength = 110;
		// アニメーションのフレーム数を定義
		const float AnimationFlame = 60.0f;

		m_player = AddGameObject<GameObject>();
		// キャラクターが前に描画されるようにレイヤーの設定
		m_player.lock()->SetDrawLayer(10);
		// Transformコンポーネントを取得
		auto transComp = m_player.lock()->GetComponent<Transform>();
		// 座標の設定
		transComp->SetPosition(StartPosition);
		// 大きさの設定
		transComp->SetScale(Vec3(Player_Scale));
		// 描画コンポーネントの追加
		auto drawComp = m_player.lock()->AddComponent<BcPNTBoneModelDraw>();
		// モデルメッシュの読み込み
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		// テクスチャの読み込み
		drawComp->SetTextureResource(L"PLAYER_TX");
		// ライティングを入れるかどうか
		drawComp->SetLightingEnabled(false);
		// アニメーションを追加する
		drawComp->AddAnimation(L"Default", 0, Player_AnimLength, true, AnimationFlame);
		// アニメーションの設定
		drawComp->ChangeCurrentAnimation(L"Default");
		// マルチメッシュの設定
		drawComp->SetMultiMeshIsDraw(0, false); // 一番上の線
		drawComp->SetMultiMeshIsDraw(1, false);	// 真ん中の線
		drawComp->SetMultiMeshIsDraw(2, false);	// 一番下の線
		drawComp->SetMultiMeshIsDraw(3, true);	// 通常顔
		drawComp->SetMultiMeshIsDraw(4, false);	// うれしい
		drawComp->SetMultiMeshIsDraw(5, true); // 体
		drawComp->SetMultiMeshIsDraw(6, false);	// 寝てる顔
		drawComp->SetMultiMeshIsDraw(7, false);	// 悲しい顔
	}

	void TitleStage::CreateTitleResources()
	{
		// 背景画像のサイズ
		const Vec2 BackGround_Size = Vec2(1300, 800);
		// 背景画像の座標
		const Vec2 BackGround_Pos = Vec2(640, 400);
		// タイトルロゴのサイズ
		const Vec2 TitleLogo_Size = Vec2(819, 205);
		// タイトルロゴの座標
		const Vec2 TitleLogo_Pos = Vec2(640, 140);
		// ボタンの初期サイズ
		const Vec2 PushAnyButton_Size = Vec2(1000, 100);
		// ボタンの初期位置
		const Vec2 PushAnyButton_Pos = Vec2(0, -300);

		// 背景画像の追加
		auto titleBack = AddGameObject<Sprite>(L"SkyBox_Back_TX", BackGround_Size);
		// 座標の設定
		titleBack->SetPosition(BackGround_Pos);
		// タイトルロゴの追加
		auto titleSprite = AddGameObject<Sprite>(L"TITLE_TX", TitleLogo_Size);
		// 座標の設定
		titleSprite->SetPosition(TitleLogo_Pos);
		// 点滅するボタンの追加
		auto buttonSprite = AddGameObject<AnimSprite>(L"Title_BUTTON_TX", true, PushAnyButton_Size, PushAnyButton_Pos);
		buttonSprite->SetSelect(true);

	}

	void TitleStage::FadePlay()
	{
		// フェードの座標を定義
		const Vec2 FadePos = Vec2(640, 400);
		// フェードの大きさを定義
		const Vec2 FadeSize = Vec2(1280, 800);
		// SEの大きさを定義
		const float SE_Volume = 1.0f;
	
		auto& gm = GameManager::GetInstance();
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		// ボタンが押されたら
		if (CntlVec[0].wPressedButtons) {
			auto scenePtr = App::GetApp()->GetScene<Scene>();
			scenePtr->MusicOnceStart(L"Decision_SE", SE_Volume);
			// フェードの作成
			auto fade = AddGameObject<FadeInOut>(FadePos, FadeSize);
			fade->SetIsFadeOut(true);
		}
		else
		{
			return;
		}
		// フェード中かどうか
		if (!gm.GetIsFade())
		{
			gm.SetIsFade(true);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
		}

	}
}