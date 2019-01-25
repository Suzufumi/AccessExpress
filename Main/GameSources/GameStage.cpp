/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();

		//auto ptrCamera = ObjectFactory::Create<Camera>();
		// デバッグ用アングル(ステージ全体を見る)
		//ptrView->SetCamera(ptrCamera);
		//ptrCamera->SetEye(Vec3(0.0f, 60.0f, -100.0f));
		//ptrCamera->SetAt(Vec3(0.0f, -30.0f, 50.0f));

		//ビューのカメラの設定
		auto ptrCamera = ObjectFactory::Create<TpsCamera>();
		ptrView->SetCamera(ptrCamera);
		//カメラが判定を持てないので、個別に判定を作成
		AddGameObject<TpsCameraJudgment>();
		//ptrCamera->SetEye(Vec3(0.0f, 10.0f, -5.0f));
		//ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定x
		ptrMultiLight->SetDefaultLighting();


	}


	//----------------------------------------------------------------
	//床の作成
	//----------------------------------------------------------------
	void GameStage::CreateFloor() {
		auto ptr = AddGameObject<GameObject>();
		auto ptrTrans = ptr->GetComponent<Transform>();
		//四角形のコリジョンセット
		auto col = ptr->AddComponent<CollisionObb>();
		//動かないようにする
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetScale(100.0f, 0.5f, 100.0f);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetWorldPosition(Vec3(0.0f, -0.25f, 0.0f));
		//描画コンポーネントの追加
		auto drawComp = ptr->AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetFogEnabled(true);
		//自分に影が映りこむようにする
		drawComp->SetOwnShadowActive(false);
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetColorAndAlpha(Color);
	}
	//----------------------------------------------------------------
	//プレイヤー関係の作成
	//----------------------------------------------------------------
	void GameStage::CreatePlayerRelationship() {
		GameObjecttXMLBuilder builder;
		// プレイヤーをXmlに登録
		builder.Register<Player>(L"Player");
		// XmlファイルからL"Player"を探す
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Player");

		//プレイヤー関連
		AddGameObject<ViewChainLetter>();
		auto chainNum = AddGameObject<ViewChainNum>();
		chainNum->GetComponent<Transform>()->SetPosition(Vec3(780, -480, 0));
		AddGameObject<RayRangeViewObj>();
	}

	void GameStage::CreateBill()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Wall>(L"Wall");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Wall");
	}

	void GameStage::CreateLinkObject()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<LinkObject>(L"RadioTower");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/RadioTower");
	}

	void GameStage::CreateDrone()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Drone>(L"Enemy");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Enemy");
	}

	void GameStage::CreateGoal()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<CheckPoint>(L"Goal");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Goal");
	}
	void GameStage::CreateMail()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<MailObject>(L"Mail");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Mails");
	}

	void GameStage::CreateFire()
	{
		auto multiFirePtr = AddGameObject<FireEffect>();
		//共有オブジェクトに炎を登録
		SetSharedGameObject(L"FireEffect", multiFirePtr);
		auto multiSparkPtr = AddGameObject<SparkEffect>();
		SetSharedGameObject(L"SparkEffect", multiSparkPtr);

	}


	void GameStage::OnCreate() {
		try {
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			m_stageXmlPath = dataDir + L"Stage\\" + L"1.xml";
			auto& gm = GameManager::GetInstance();

			XmlDocReader xmlReader(m_stageXmlPath);
			//ゲームが始まっているフラグを切る
			gm.SetGameStart(false);
			//スローフラグをオフにしておく
			gm.SetOnSlow(false);
			//スコアを初期化する
			gm.ResetNowScore();
			//gm.ResetCheckPointNum();
			//マックスチェイン初期化
			gm.ResetMaxChain();
			//メール数初期化
			gm.ResetMail();
			//・ｽ^・ｽC・ｽ・ｽ・ｽA・ｽb・ｽv・ｽt・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
			gm.ResetTimeUp();
			CreateSharedObjectGroup(L"Link");
			CreateSharedObjectGroup(L"Drone");
			CreateSharedObjectGroup(L"CheckPoints");
			CreateSharedObjectGroup(L"Mails");
			//物理計算有効
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			//床
			//CreateFloor();
			//プレイヤー関係
			CreatePlayerRelationship();
			// 建物の配置
			CreateBill();
			CreateLinkObject();

			CreateDrone();
			CreateMail();
			//メール数を表示するUI
			AddGameObject<MailNumUI>();
			//制限時間を作成（中央上）
			auto timer = AddGameObject<RemainingTimerSprite>(40);
			timer->GetComponent<Transform>()->SetPosition(1280 - 64 * 10, 0, 0);
			//スコアUIを作成
			auto score = AddGameObject<ScoreUI>();
			score->GetComponent<Transform>()->SetPosition(1280 - 64 * 4, 0, 0);

			CreateGoal();
			AddGameObject<SkyBox>();
			auto scoreSprite = AddGameObject<Sprite>(L"SCORE_TX", Vec2(100, 40));
			scoreSprite->SetPosition(Vec2(1120, 30));
			auto timeSprite = AddGameObject<Sprite>(L"TIME_TX", Vec2(100, 35));
			timeSprite->SetPosition(Vec2(640, 27));
			CreateFire();
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate() {
		//マネージャーが持つパッド情報を更新
		GameManager::GetInstance().UpDatePadData();

		//スタートボタンを押すことでリザルトに行く
		if (GameManager::GetInstance().GetPad().wPressedButtons & XINPUT_GAMEPAD_START){
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
			//auto fade = AddGameObject<FadeInOut>(Vec2(1.0f, 1.0f));
			//fade->SetIsAction(true);
		}
	}
}
//end basecross
