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
		//プレイヤー
		auto player = AddGameObject<Player>(Vec3(-35.0f, 9.0f, -35.0f), Quat(0, 0, 0, 1), Vec3(1, 2, 1));
		//プレイヤーが使う電波塔との判定専門の当たり判定
		auto radioTowerHitJudgment = AddGameObject<RadioTowerHitJudgment>(player);
		//プレイヤーに電波塔との当たり判定を認知させる
		player->SetRadioTowerHitJudgment(radioTowerHitJudgment);
	}

	void GameStage::CreateBill()
	{
		struct BillTrans
		{
			Vec3 m_pos;
			Vec3 m_scale;
		};
		// 配列のサイズ(建物の数)
		const int BILL_COUNT = 35;
		BillTrans billParam[BILL_COUNT] = 
		{
			{ Vec3(-35.0f, 4.0f, -35.0f), Vec3(5.0f, 8.0f, 7.0f) }, //0
			{ Vec3(-35.0f, 4.0f, -20.0f), Vec3(4.0f, 8.0f, 5.0f) }, //1
			{ Vec3(-35.0f, 2.5f, -10.0f), Vec3(4.0f, 5.0f, 3.0f) },	//2
			{ Vec3(-35.0f, 3.0f, 0.0f),   Vec3(4.0f, 6.0f, 4.0f) },	//3
			{ Vec3(-35.0f, 4.0f, 10.0f),  Vec3(4.0f, 8.0f, 5.0f) }, //4
			{ Vec3(-35.0f, 2.5f, 25.0f),  Vec3(4.0f, 5.0f, 4.0f) }, //5

			{ Vec3(-23.0f, 3.0f, -35.0f), Vec3(4.0f, 6.0f, 4.0f) }, //6 
			{ Vec3(-23.0f, 3.5f, -25.0f), Vec3(5.0f, 7.0f, 3.0f) }, //7 
			{ Vec3(-23.0f, 4.0f, -15.0f), Vec3(4.0f, 8.0f, 4.0f) }, //8 
			{ Vec3(-23.0f, 2.5f, -5.0f),  Vec3(4.0f, 5.0f, 3.0f) }, //9 
			{ Vec3(-23.0f, 3.5f, 5.0f),   Vec3(5.0f, 7.0f, 3.0f) }, //10 
			{ Vec3(-23.0f, 3.0f, 15.0f),  Vec3(4.0f, 6.0f, 3.0f) }, //11

			{ Vec3(-10.0f, 3.0f, -35.0f), Vec3(4.0f, 6.0f, 3.0f) }, //13
			{ Vec3(-10.0f, 2.5f, -25.0f), Vec3(5.0f, 5.0f, 4.0f) }, //14
			{ Vec3(-10.0f, 4.0f, -10.0f), Vec3(5.0f, 8.0f, 5.0f) }, //15
			{ Vec3(-10.0f, 3.0f, 0.0f),   Vec3(6.0f, 6.0f, 4.0f) }, //16
			{ Vec3(-10.0f, 3.5f, 10.0f),  Vec3(5.0f, 7.0f, 3.0f) }, //17
			{ Vec3(-10.0f, 3.0f, 20.0f),  Vec3(4.0f, 6.0f, 4.0f) }, //18

			{ Vec3(7.0f, 4.0f, -35.0f), Vec3(5.0f, 8.0f, 5.0f) }, //20
			{ Vec3(7.0f, 3.5f, -25.0f), Vec3(5.0f, 7.0f, 5.0f) }, //21
			{ Vec3(7.0f, 3.0f, -15.0f), Vec3(4.0f, 6.0f, 4.0f) }, //22
			{ Vec3(7.0f, 4.0f, -5.0f),  Vec3(6.0f, 8.0f, 5.0f) }, //23
			{ Vec3(7.0f, 3.0f, 5.0f),   Vec3(4.0f, 6.0f, 6.0f) }, //24
			{ Vec3(7.0f, 2.5f, 20.0f),  Vec3(5.0f, 5.0f, 5.0f) }, //25

			{ Vec3(20.0f, 3.0f, -35.0f),  Vec3(6.0f, 6.0f, 6.0f) }, //26
			{ Vec3(20.0f, 2.5f, -25.0f),  Vec3(6.0f, 5.0f, 4.0f) }, //27
			{ Vec3(20.0f, 3.0f, -5.0f),   Vec3(4.0f, 6.0f, 4.0f) }, //29
			{ Vec3(20.0f, 4.0f, -15.0f), Vec3(4.0f, 8.0f, 5.0f) }, //30
			{ Vec3(20.0f, 3.5f, 15.0f),   Vec3(5.0f, 7.0f, 5.0f) }, //31

			{ Vec3(33.0f, 4.0f, -35.0f), Vec3(6.0f, 8.0f, 5.0f) }, //32
			{ Vec3(33.0f, 4.0f, -25.0f), Vec3(6.0f, 8.0f, 5.0f) }, //33
			{ Vec3(33.0f, 3.0f, -15.0f), Vec3(5.0f, 6.0f, 6.0f) }, //34
			{ Vec3(33.0f, 3.5f, -5.0f),  Vec3(5.0f, 7.0f, 4.0f) }, //35
			{ Vec3(33.0f, 4.0f, 5.0f),   Vec3(4.0f, 8.0f, 5.0f) }, //36
			{ Vec3(33.0f, 3.0f, 15.0f),  Vec3(6.0f, 6.0f, 4.0f) }  //37
		};

		for (auto bill : billParam)
		{
			AddGameObject<Wall>(bill.m_pos, bill.m_scale);
		}
	}

	void GameStage::CreateLinkObject()
	{
		auto linkA = AddGameObject<LinkObject>(Vec3(-35.0f, 8.5f, -33.0f), Vec3(1.0f, 1.0f, 1.0f));
		auto linkB = AddGameObject<LinkObject>(Vec3(-35.0f, 8.5f, -20.0f), Vec3(1.0f, 1.0f, 1.0f));
		linkA->SetGoPosition(linkB->GetComponent<Transform>()->GetWorldPosition());
		auto linkC = AddGameObject<LinkObject>(Vec3(-35.0f, 5.5f, -10.0f), Vec3(1.0f, 1.0f, 1.0f));
		linkB->SetGoPosition(linkC->GetComponent<Transform>()->GetWorldPosition());
		auto linkD = AddGameObject<LinkObject>(Vec3(-23.0f, 7.5f, 5.0f), Vec3(1.0f, 1.0f, 1.0f));
		linkC->SetGoPosition(linkD->GetComponent<Transform>()->GetWorldPosition());



	}

	void GameStage::OnCreate() {
		try {
			//物理計算有効
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			//床
			CreateFloor();
			//プレイヤー関係
			CreatePlayerRelationship();
			auto goal = AddGameObject<Goal>(Vec3(20.0f, 3.5f, 5.0f), Vec3(6.0f, 7.0f, 5.0f));
			// 建物の配置
			CreateBill();
			auto addless = AddGameObject<AddlessCertificate>(Vec3(5.0f, 5.5f, 20.0f), Vec3(1, 1, 1));
			// ゴールの設定
			addless->SetGoal(goal);

			CreateLinkObject();

			//Quat qt;
			//AddGameObject<SearchObject>(Vec3(-3.0f, 1.0f, 5.0f), Quat(qt.identity()), Vec3(1.0f, 1.0f, 1.0f));


			AddGameObject<RemainingTimer>();
		}
		catch (...) {
			throw;
		}
	}
}
//end basecross
