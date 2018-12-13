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
		//ptrCamera->SetEye(Vec3(0.0f, 10.0f, -5.0f));
		//ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定x
		ptrMultiLight->SetDefaultLighting();


	}

	void GameStage::SetBackGround()
	{
		struct backGRParam
		{
			Vec3 m_rot;
			Vec3 m_pos;
		};

		const int BACK_GROUND = 4;
		backGRParam backParam[BACK_GROUND] =
		{
			{ Vec3(0.0f), Vec3(0.0f, 37.5f, 50.0f)},
			{ Vec3(0.0f, Deg2Rad(90), 0.0f), Vec3(-50.0f, 37.5f, 0.0f)},
			{ Vec3(0.0f, Deg2Rad(-90), 0.0f), Vec3(50.0f, 37.5f, 0.0f)},
			{ Vec3(0.0f), Vec3(0.0f, 37.5f, -50.0f)}
		};

		for (auto back : backParam)
		{
			AddGameObject<SkySprite>(back.m_rot, back.m_pos);

		}
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

		//プレイヤー
		//auto player = AddGameObject<Player>(Vec3(pos), Quat(qt), Vec3(scale));
		//auto dev = AddGameObject<SightingDevice>();
		//player->SetSightingDevice(dev);
		//SetSharedGameObject(L"Player", player);
		//AddGameObject<ViewChainLetter>();
		//AddGameObject<ViewChainNum>();
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
		struct DroneParam
		{
			Vec3 m_pos;
			Drone::DroneMotion m_dir;
			int m_needChains;
		};

		DroneParam  drones[] = {
			{ Vec3(-25.0f, 25.0f, -30.0f), Drone::DroneMotion::CounterClockwise, 2},
			{ Vec3(-20.0f, 25.0f, -30.0f), Drone::DroneMotion::ClockWise, 4},
			{ Vec3(-10.0f, 21.0f, 28.0f), Drone::DroneMotion::VerticalMotion, 1},
			{ Vec3(-15.0f, 20.0f, 40.0f), Drone::DroneMotion::VerticalMotion, 2},
			{ Vec3(55.0f, 25.0f, 50.0f), Drone::DroneMotion::CounterClockwise, 3 },
			{ Vec3(5.0f, 18.0f, -23.0f), Drone::DroneMotion::Wave, 2 },
			{ Vec3(37.0f, 15.0f, -10.0f), Drone::DroneMotion::VerticalMotion, 4 },
			{ Vec3(5.0f, 18.0f, 5.0f), Drone::DroneMotion::Wave, 2 },
			{ Vec3(50.0f, 25.0f, 35.0f), Drone::DroneMotion::ClockWise, 2},
		};

		for (auto drone : drones)	{
			auto ptrDrone = AddGameObject<Drone>(drone.m_pos, drone.m_dir, drone.m_needChains);
		}

	}

	void GameStage::OnCreate() {
		try {
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			m_stageXmlPath = dataDir + L"Stage\\" + L"1.xml";

			XmlDocReader xmlReader(m_stageXmlPath);
			//ゲームが始まっているフラグを切る
			App::GetApp()->GetScene<Scene>()->SetGameStart(false);
			//スローフラグをオフにしておく
			GameManager::GetInstance().SetOnSlow(false);
			//スコアを初期化する
			GameManager::GetInstance().ResetNowScore();
			CreateSharedObjectGroup(L"Link");
			CreateSharedObjectGroup(L"Drone");
			//物理計算有効
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			//床
			//CreateFloor();
			//プレイヤー関係
			CreatePlayerRelationship();
			auto goal = AddGameObject<Goal>(Vec3(65.0f, 13.0f, 50.0f), Vec3(1.0f, 30.0f, 1.0f));
			// 建物の配置
			CreateBill();
			CreateLinkObject();

			//SetBackGround();

			//Quat qt;
			//AddGameObject<SearchObject>(Vec3(-3.0f, 1.0f, 5.0f), Quat(qt.identity()), Vec3(1.0f, 1.0f, 1.0f));
						
			//auto drone = AddGameObject<Drone>(Vec3(-35, 25, -35),Drone::RoopDirection::ClockWise);
			//drone->SetDeadChain(1);
			//auto drone = AddGameObject<Drone>(Vec3(-35, 25, -35));
			//drone->SetDeadChain(1);
			//AddGameObject<Drone>(Vec3(-35, 25, -35));
			CreateDrone();
			
			//制限時間を作成（中央上）
			auto timer = AddGameObject<RemainingTimerSprite>(60);
			timer->GetComponent<Transform>()->SetPosition(1280 - 64 * 10, 0, 0);
			//スコアUIを作成
			auto score = AddGameObject<ScoreUI>();
			score->GetComponent<Transform>()->SetPosition(1280 - 64 * 3, 0, 0);

			AddGameObject<SkySphere>();
			AddGameObject<EnergyGaugeUI>();
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate() {
		//マネージャーが持つパッド情報を更新
		GameManager::GetInstance().UpDatePadData();

		//スタートボタンを押すことでリザルトに行く
		if (GameManager::GetInstance().GetPad().wPressedButtons & XINPUT_GAMEPAD_START
			&& App::GetApp()->GetScene<Scene>()->GetGameStart()) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
			auto fade = AddGameObject<FadeInOut>(Vec2(1.0f, 1.0f));
			fade->SetIsAction(true);
		}
	}
}
//end basecross
