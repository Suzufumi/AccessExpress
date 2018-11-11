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
		//ビューのカメラの設定
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrView->SetCamera(ptrCamera);
		ptrCamera->SetEye(Vec3(0.0f, 10.0f, -5.0f));
		ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
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
		drawComp->SetOwnShadowActive(true);
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetColorAndAlpha(Color);
	}
	//----------------------------------------------------------------
	//プレイヤー関係の作成
	//----------------------------------------------------------------
	void GameStage::CreatePlayerRelationship() {
		//プレイヤー
		auto player = AddGameObject<Player>(Vec3(-3.0f, 1.0f, 0.0f), Quat(0, 0, 0, 1), Vec3(1, 2, 1));
		//プレイヤーが使う電波塔との判定専門の当たり判定
		auto radioTowerHitJudgment = AddGameObject<RadioTowerHitJudgment>(player);
		//プレイヤーに電波塔との当たり判定を認知させる
		player->SetRadioTowerHitJudgment(radioTowerHitJudgment);
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
			//壁
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					AddGameObject<Wall>(Vec3(-30.0 + ((float)i*10.0), 4.5, -30.0 + ((float)j * 10.0f)), Vec3(3, 8, 3));
				}
			}
			Quat qt;

			AddGameObject<SearchObject>(Vec3(-3.0f, 1.0f, 5.0f), Quat(qt.identity()), Vec3(1.0f, 1.0f, 1.0f));

			auto linkA = AddGameObject<LinkObject>(Vec3(-4, 1, 0), Vec3(1, 1, 1));
			auto linkB = AddGameObject<LinkObject>(Vec3(-4, 1, 10), Vec3(1, 1, 1));
			linkA->SetGoPosition(linkB->GetComponent<Transform>()->GetWorldPosition());
			linkB->SetGoPosition(linkA->GetComponent<Transform>()->GetWorldPosition());


			//電波塔
			//for (int i = 0; i < 3; i++)
			//{
			//	for (int j = 0; j < 3; j++)
			//	{
			//		AddGameObject<RadioTower>(Vec3(-10.0f + ((float)i * 10.0f), 5.0f, -10.0f + ((float)j * 10.0f)), Vec3(1.5, 1.5, 1.5), float(15.0f));
			//	}
			//}

			//ドライブ
			//AddGameObject<Drive>(Vec3(-3.0f, 1.0f, 2.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(-6.0f, 1.0f, 10.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(7.0f, 1.0f, 5.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(-12.0f, 1.0f, 10.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(-7.0f, 1.0f, 40.0f), Vec3(1, 1, 1));
			////ファイル
			//AddGameObject<File>(Vec3(-4.0f, 1.0f, 2.0f), Vec3(1, 1, 1));
		}
		catch (...) {
			throw;
		}
	}
}
//end basecross
