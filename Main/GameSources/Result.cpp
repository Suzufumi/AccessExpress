#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ResultStage::OnCreate() {
		CreateViewLight();
		CreateRemainingTime();
		CreateMaximumChain();
		CreateScore();
		auto clearSprite = AddGameObject<Sprite>(L"ResultStage_TX", Vec2(800.0f, 120.0f));
		clearSprite->SetPosition(Vec2(640.0f, 100.0f));
		m_push = AddGameObject<Sprite>(L"Title_BUTTON_TX", Vec2(1000, 100));
		m_push.lock()->SetPosition(Vec2(640.0f, 700.0f));
		//auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		//obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"ResultStage_TX");
	}
	void ResultStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		switch (m_progress){
		case progress::START:
			if (CntlVec[0].wPressedButtons) {
				m_progress = progress::ADD_SCORE;
				m_push.lock()->SetDrawActive(false);
				m_chackNumParameter.lock()->SetDrawActive(false);
			}
			break;
		case progress::ADD_SCORE :
			AddScore();
			break;
		case progress::SCORE_COUNTUP :
			if (m_scoreNum.lock()->GetFinishCountUp()) {
				m_push.lock()->SetDrawActive(true);
				m_chackText.lock()->SetDrawActive(false);
				m_chackNum.lock()->SetDrawActive(false);
				m_maxChainText.lock()->SetDrawActive(false);
				m_maxChainNum.lock()->SetDrawActive(false);
				m_scoreText.lock()->SetPosition(Vec2(720.0f, 340.0f));
				//m_scoreNum.lock()->GetComponent<Transform>()->SetPosition()
				CreatePlayer();
				m_progress = progress::END;
			}
		case progress::END :
			if (CntlVec[0].wPressedButtons) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			}
			//m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->UpdateAnimation(App::GetApp()->GetElapsedTime());
		default:
			break;
		}
	}
	void ResultStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}
	///-----------------------------------------------------------------------------
	//ゴール数の表示作成
	///-----------------------------------------------------------------------------
	void ResultStage::CreateRemainingTime() {
		m_chackText = AddGameObject<Sprite>(L"RemainingTimeText_TX", Vec2(410, 205));
		m_chackText.lock()->SetPosition(Vec2(320.0f, 250.0f));
		//数字
		m_chackNum = AddGameObject<NumberSprite>(1, 5);
		m_chackNum.lock()->GetComponent<Transform>()->SetPosition(820, -150, 0);
		m_chackP1 = m_chackNum.lock()->GetComponent<Transform>()->GetPosition();
		//分数の母数側
		m_chackNumParameter = AddGameObject<NumberSprite>(1, 5);
		m_chackNumParameter.lock()->GetComponent<Transform>()->SetPosition(940, -150, 0);
	};
	///-----------------------------------------------------------------------------
	//マックスチェインの表示作成
	///-----------------------------------------------------------------------------
	void ResultStage::CreateMaximumChain() {
		//文字
		m_maxChainText = AddGameObject<Sprite>(L"MaxChainText_TX", Vec2(461, 230));
		m_maxChainText.lock()->SetPosition(Vec2(320.0f, 400.0f));
		//数字
		m_maxChainNum = AddGameObject<NumberSprite>(2, 22);
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(880, -300, 0);
		m_maxChainP1 = m_maxChainNum.lock()->GetComponent<Transform>()->GetPosition();
	};
	///-----------------------------------------------------------------------------
	//スコアの表示作成
	///-----------------------------------------------------------------------------
	void ResultStage::CreateScore() {
		//文字
		m_scoreText = AddGameObject<Sprite>(L"SCORE_TX", Vec2(282, 68));
		m_scoreText.lock()->SetPosition(Vec2(320.0f, 540.0f));
		//数字
		m_scoreNum = AddGameObject<ScoreUI>();
		m_scoreNum.lock()->GetComponent<Transform>()->SetPosition(750, -450, 0);
		m_scoreP2 = m_scoreNum.lock()->GetComponent<Transform>()->GetPosition() + Vec3(50.0f,0,0);
	};
	///-----------------------------------------------------------------------------
	//スコアを演出後に増やす
	///-----------------------------------------------------------------------------
	void ResultStage::AddScore() {
		m_leap += App::GetApp()->GetElapsedTime();
		if (m_leap >= 1.0f) {
			m_leap = 1.0f;
			GameManager::GetInstance().AddScore((6 * 20)*10);
			//チェックポイントパーフェクトボーナスも入れる

			m_progress = progress::SCORE_COUNTUP;
			m_chackNum.lock()->SetDrawActive(false);
			m_maxChainNum.lock()->SetDrawActive(false);
		}
		Vec3 chackPos = (1 - m_leap) * m_chackP1 + m_leap * m_scoreP2;
		m_chackNum.lock()->GetComponent<Transform>()->SetPosition(chackPos);
		Vec3 chainPos = (1 - m_leap) * m_maxChainP1 + m_leap * m_scoreP2;
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(chainPos);
	}
	///-----------------------------------------------------------------------------
	//プレイヤーを画面に出す
	///-----------------------------------------------------------------------------
	void ResultStage::CreatePlayer(){
		m_player = AddGameObject<GameObject>();
		m_player.lock()->SetDrawLayer(100);
		auto transComp = m_player.lock()->GetComponent<Transform>();
		transComp->SetPosition(-4.0f, -4.0f, -5.0f);
		transComp->SetScale(1.5f, 1.5f, 1.5f);
		auto drawComp = m_player.lock()->AddComponent<BcPNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		drawComp->SetLightingEnabled(false);
		// アニメーションを追加する
		drawComp->AddAnimation(L"Default", 0, 110, true, 60.0f);
		// アニメーションの設定
		drawComp->ChangeCurrentAnimation(L"Default");
		drawComp->SetMultiMeshIsDraw(0, false); // 一番上の線
		drawComp->SetMultiMeshIsDraw(1, false);	// 真ん中の線
		drawComp->SetMultiMeshIsDraw(2, false);	// 一番下の線
		drawComp->SetMultiMeshIsDraw(3, true);	// 通常顔
		drawComp->SetMultiMeshIsDraw(4, false);	// うれしい
		drawComp->SetMultiMeshIsDraw(5, true); // 体
		drawComp->SetMultiMeshIsDraw(6, false);	// 寝てる顔
		drawComp->SetMultiMeshIsDraw(7, false);	// 悲しい顔
	}
}