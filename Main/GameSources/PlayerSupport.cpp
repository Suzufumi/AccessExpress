#include"stdafx.h"
#include"Project.h"

namespace basecross {
	//---------------------------------------------------------------------------------------------
	//Rayの判定範囲を見せるオブジェ
	//---------------------------------------------------------------------------------------------
	RayRangeViewObj::RayRangeViewObj(const shared_ptr<Stage>& StagePtr)
		:GameObject(StagePtr)
	{
	}
	void RayRangeViewObj::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(10.0f, 10.0f, 10.0f);

		auto drawComp = AddComponent<BcPNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");
		//Col4 Color(0.4f, 1.0f, 0.7f, 0.1f);
		//drawComp->SetDiffuse(Color);
		drawComp->SetRasterizerState(RasterizerState::CullFront);
		drawComp->SetTextureResource(L"RayRange_TX");
		SetAlphaActive(true);
		SetDrawLayer(-1);

		//プレイヤーにアクセス
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		//playerと親子関係になる
		ptrTrans->SetParent(player);
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto m_tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		//照準位置と同位置になる
		ptrTrans->SetWorldPosition(
			player->GetComponent<Transform>()->GetWorldPosition() + Vec3(0, m_tpsCamera->GetCameraLookUp(),0));
	}
	void RayRangeViewObj::OnUpdate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		//Rayの射程を取得して球の大きさ変更
		float scale = player->GetRayRange()*2.1f;
		GetComponent<Transform>()->SetScale(scale, scale, scale);
		if (GameManager::GetInstance().GetGameStart() == true) {
			SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
		}
	}


	//--------------------------------------------------------------------------------
	//照準
	//--------------------------------------------------------------------------------
	SightingDevice::SightingDevice(const shared_ptr<Stage>& stagePtr)
		:GameObject(stagePtr)
	{
	}
	void SightingDevice::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(Vec3(0, 0, 0));
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(2, 2, 2);
		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//影が映りこまない
		drawComp->SetOwnShadowActive(false);
		//ライトを無視して光る
		drawComp->SetLightingEnabled(false);
		//アルファ値適応
		drawComp->SetBlendState(BlendState::AlphaBlend);
		//テクスチャ
		drawComp->SetTextureResource(L"cursor_TX");
		//照準を通して世界を見るとほかのオブジェクトが透明になってしまうので
		//レイヤー位置を変えることで対応
		SetDrawLayer(1);
	}
	void SightingDevice::OnUpdate() {
		auto drawComp = GetComponent<BcPNTStaticDraw>();
		if (m_captureLink) {
			Col4 Color(1.0f, 0.2f, 0.2f, 1.0f);
			drawComp->SetDiffuse(Color);
		}
		else {
			Col4 Color(1.0f, 1.0f, 1.0f, 1.0f);
			drawComp->SetDiffuse(Color);
		}
	}
	//-------------------------------------------------------------------------------------------------
	//チェインの文字とバーを見せるオブジェクト
	//-------------------------------------------------------------------------------------------------
	ViewChainLetter::ViewChainLetter(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"chain_gauge_TX", Vec2(155, 155)), m_maxGageLength(1.0f)
	{

	}

	void ViewChainLetter::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(790, 640));
		SetDrawLayer(1);
		//ゲージバー
		m_gage = GetStage()->AddGameObject<Sprite>(L"chainbar_TX", Vec2(155,155));
		m_gage.lock()->SetPosition(Vec2(790, 640));
		m_gage.lock()->SetDrawLayer(1);
	}
	void ViewChainLetter::OnUpdate() {
		auto& manager = GameManager::GetInstance();
		float remainingGage = m_maxGageLength - (m_maxGageLength * manager.GetSlowPassage() * manager.GetControlGageSpeed());
		m_gage.lock()->GetComponent<Transform>()->SetScale(remainingGage,1.0f , 1.0f);
		m_gage.lock()->SetPosition(Vec2(718.0f + (74*remainingGage), 640));
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			SetDrawActive(true);
			m_gage.lock()->SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
			m_gage.lock()->SetDrawActive(false);
		}
	}
	//-------------------------------------------------------------------------------------------------
	//チェインの数字を見せるオブジェクト
	//-------------------------------------------------------------------------------------------------
	ViewChainNum::ViewChainNum(const shared_ptr<Stage>& stagePtr)
		: GameObject(stagePtr){
		m_places = 2;
	}
	//------------------------------------------------------------------------------------
	//構築
	//------------------------------------------------------------------------------------
	void ViewChainNum::OnCreate() {
		// 数字ごとの範囲を設定する
		for (int i = 0; i < 10; i++) {
			m_numRects.push_back({
				static_cast<float>(64 * i),			// left
				static_cast<float>(0),				// top
				static_cast<float>(64 * (i + 1)),	// right
				static_cast<float>(128)				// bottom
				});
		}

		float start_x = m_numRects[5].left / 640.0f;
		float end_x = m_numRects[5].right / 640.0f;
		float start_y = m_numRects[5].top / 128.0f;
		float end_y = m_numRects[5].bottom / 128.0f;

		for (int i = 0; i < m_places; i++) {
			m_vertices.push_back({
				{ Vec3(0.0f,128.0f,0.0f),Vec2(start_x,start_y) },
				{ Vec3(64.0f,128.0f,0.0f),Vec2(end_x,start_y) },
				{ Vec3(0.0f,0.0f,0.0f),Vec2(start_x,end_y) },
				{ Vec3(64.0f,0.0f,0.0f),Vec2(end_x,end_y) }
				});
		}

		for (int i = 0; i < m_places; i++) {
			auto number = ObjectFactory::Create<Sprite>(
				GetStage(), L"Number_TX", Vec2(128,128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThisでThisオブジェクトのshared_ptrを取ってこれる
			transComp->SetParent(GetThis<ViewChainNum>());	// 基準点が画面の左上からScoreUIの場所になった
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			transComp->SetScale(1.2f, 0.9f, 0.9f);
			m_numbers.push_back(number);
		}

	}

	void ViewChainNum::OnUpdate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		int chain = player->GetChain();

		//数字を並べる
		for (int i = 0; i < m_places; i++) {
			int num = chain % 10;	// 一の位を抜き出す
			chain /= 10;			// 一の位を切り捨てる

			float start_x = m_numRects[num].left / 640.0f;
			float end_x = m_numRects[num].right / 640.0f;
			float start_y = m_numRects[num].top / 128.0f;
			float end_y = m_numRects[num].bottom / 128.0f;

			m_vertices[i][0].textureCoordinate = Vec2(start_x, start_y);
			m_vertices[i][1].textureCoordinate = Vec2(end_x, start_y);
			m_vertices[i][2].textureCoordinate = Vec2(start_x, end_y);
			m_vertices[i][3].textureCoordinate = Vec2(end_x, end_y);

			auto drawComp = m_numbers[i]->GetComponent<PTSpriteDraw>();
			drawComp->UpdateVertices(m_vertices[i]);	// 位置は変えずにポリゴンの中身だけ変える
		}
		OnDraw();
		//auto ptrEffect = GetStage()->GetSharedGameObject<GetEffect>(L"GetEffect", false);
		//if (ptrEffect)
		//{
		//	ptrEffect->InsertGetEffect(Vec3(GetComponent<Transform>()->GetPosition()));
		//}

	}

	void ViewChainNum::OnDraw() {
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			for (auto number : m_numbers) {
				number->OnDraw();
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//スコアへ飛んでいく数字数字
	//--------------------------------------------------------------------------------------
	FlyingChain::FlyingChain(const shared_ptr<Stage>& stagePtr)
		: GameObject(stagePtr) {
		m_places = 4;
	}
	//------------------------------------------------------------------------------------
	//スコアへ飛んでいく数字
	//------------------------------------------------------------------------------------
	void FlyingChain::OnCreate() {
		// 数字ごとの範囲を設定する
		for (int i = 0; i < 10; i++) {
			m_numRects.push_back({
				static_cast<float>(64 * i),			// left
				static_cast<float>(0),				// top
				static_cast<float>(64 * (i + 1)),	// right
				static_cast<float>(128)				// bottom
				});
		}

		float start_x = m_numRects[5].left / 640.0f;
		float end_x = m_numRects[5].right / 640.0f;
		float start_y = m_numRects[5].top / 128.0f;
		float end_y = m_numRects[5].bottom / 128.0f;

		for (int i = 0; i < m_places; i++) {
			m_vertices.push_back({
				{ Vec3(0.0f,128.0f,0.0f),Vec2(start_x,start_y) },
				{ Vec3(64.0f,128.0f,0.0f),Vec2(end_x,start_y) },
				{ Vec3(0.0f,0.0f,0.0f),Vec2(start_x,end_y) },
				{ Vec3(64.0f,0.0f,0.0f),Vec2(end_x,end_y) }
				});
		}

		for (int i = 0; i < m_places; i++) {
			auto number = ObjectFactory::Create<Sprite>(
				GetStage(), L"Number_TX", Vec2(640, 128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThisでThisオブジェクトのshared_ptrを取ってこれる
			transComp->SetParent(GetThis<FlyingChain>());	// 基準点が画面の左上からScoreUIの場所になった
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			transComp->SetScale(1.2f, 0.9f, 0.9f);

			m_numbers.push_back(number);
		}
	}

	void FlyingChain::OnUpdate() {
		int chain = m_score;

		//数字を並べる
		for (int i = 0; i < m_places; i++) {
			int num = chain % 10;	// 一の位を抜き出す
			chain /= 10;			// 一の位を切り捨てる

			float start_x = m_numRects[num].left / 640.0f;
			float end_x = m_numRects[num].right / 640.0f;
			float start_y = m_numRects[num].top / 128.0f;
			float end_y = m_numRects[num].bottom / 128.0f;

			m_vertices[i][0].textureCoordinate = Vec2(start_x, start_y);
			m_vertices[i][1].textureCoordinate = Vec2(end_x, start_y);
			m_vertices[i][2].textureCoordinate = Vec2(start_x, end_y);
			m_vertices[i][3].textureCoordinate = Vec2(end_x, end_y);

			auto drawComp = m_numbers[i]->GetComponent<PTSpriteDraw>();
			drawComp->UpdateVertices(m_vertices[i]);	// 位置は変えずにポリゴンの中身だけ変える
		}
		//飛んでいる状態での処理
		if (m_isFly) {
			m_wait += App::GetApp()->GetElapsedTime();
			if (m_wait >= 0.5f) {
				m_leap += App::GetApp()->GetElapsedTime() * 2.0f;
			}
			auto pos = (1 - m_leap)*(1 - m_leap)*p0 + 2 * (1 - m_leap)*m_leap*p1 + m_leap * m_leap*p2;
			GetComponent<Transform>()->SetPosition(pos);
			if (m_leap >= 1.0f) {
				//スコアについたのでスコアの値を増加させる
				GameManager::GetInstance().AddScore(m_score);
				m_leap = 0;
				m_isFly = false;
				m_wait = 0;
				GetStage()->RemoveGameObject<FlyingChain>(GetThis<FlyingChain>());
			}
		}

		OnDraw();
	}

	void FlyingChain::OnDraw() {
		if (m_isFly) {
			int score = m_score;
			int places = 0;
			//何桁表示するかを算出する
			while (score > 0) {
				score /= 10;
				places++;
			}
			//各桁の表示の切替
			int count = 0;
			for (auto number : m_numbers) {
				if (count < places || count < 3) {
					number->OnDraw();

					number->SetDrawActive(true);
				}
				else {
					number->SetDrawActive(false);
				}
				count++;
			}
		}
	}
	void FlyingChain::FlySet(int chain) {
		m_score = (chain * 30) + (chain * 10);
		m_isFly = true;
	}
}