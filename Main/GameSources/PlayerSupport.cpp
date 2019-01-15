#include"stdafx.h"
#include"Project.h"

namespace basecross {
	//---------------------------------------------------------------------------------------------
	//電波塔との衝突判定を行う当たり判定のコンストラクタ
	//---------------------------------------------------------------------------------------------
	RadioTowerHitJudgment::RadioTowerHitJudgment(const shared_ptr<Stage>& StagePtr, weak_ptr<GameObject> player)
		:GameObject(StagePtr), m_player(player)
	{
	}
	void RadioTowerHitJudgment::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(10.0f, 10.0f, 10.0f);

		//球形の当たり判定をセット
		auto col = AddComponent<CollisionSphere>();
		//オブジェクトと当たっても反発しない
		col->SetAfterCollision(AfterCollision::None);
		//当たり判定が見えるようにする
		//col->SetDrawActive(true);
		//Playerとぶつからないようにするためのタグ
		AddTag(L"PlayerUse");
		//コンストラクタで取得したプレイヤーにアクセス
		auto obj = m_player.lock();
		//playerと親子関係になる
		ptrTrans->SetParent(obj);
		//playerと同位置になる
		ptrTrans->SetWorldPosition(obj->GetComponent<Transform>()->GetWorldPosition());
	}
	//-------------------------------------------------------------------------------------------------------------
	//ヒットしている最中
	//-------------------------------------------------------------------------------------------------------------
	void RadioTowerHitJudgment::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		//衝突したものが電波塔か調べる
		//auto radioTower = dynamic_pointer_cast<RadioTower>(Other);
		////電波塔だった
		//if (radioTower) {
		//	//Player位置から電波塔の位置を引く
		//	Vec3 d = this->GetComponent<Transform>()->GetWorldPosition() - radioTower->GetComponent<Transform>()->GetWorldPosition();
		//	//位置の差から距離を出す
		//	float leng = d.length();
		//	//導き出した距離が現在持っている距離よりも近かったら
		//	if (m_length > leng) {
		//		//一番近い距離を更新する
		//		m_length = leng;
		//		//速度を取得する
		//		m_Acceleration = radioTower->GetAcceleration();
		//	}
		//}
	}

	//-------------------------------------------------------------------------------------
	//ファイル
	//-------------------------------------------------------------------------------------
	File::File(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}
	//-------------------------------------------------------------------------------------
	//初期化
	//-------------------------------------------------------------------------------------
	void File::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//drawComp->SetFogEnabled(true);
		//自分に影が映りこむようにする
		drawComp->SetOwnShadowActive(true);
		Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
		drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		//タグ付け
		AddTag(L"File");
	}
	void File::SetOnPlayer(weak_ptr<GameObject> obj) {
		auto trans = GetComponent<Transform>();
		//オブジェクトにアクセス
		auto player = obj.lock();
		//オブジェクトを親にする
		trans->SetParent(player);
		//親の上に乗る
		trans->SetPosition(Vec3(0, player->GetComponent<Transform>()->GetScale().y / 2 + m_scale.y / 2, 0));
	}
	void File::LookFile() {
		auto drawComp = GetComponent<DrawComponent>();
		drawComp->SetDrawActive(true);
	}
	void File::UnLookFile() {
		auto drawComp = GetComponent<DrawComponent>();
		drawComp->SetDrawActive(false);
	}

	AddlessCertificate::AddlessCertificate(const shared_ptr<Stage>& stage, Vec3 pos, Vec3 scale)
		:OBBObject(stage, pos, scale)
	{
	}
	void AddlessCertificate::OnCreate() {
		//親オブジェクトのコンストラクタを呼ぶ、ここで形の形成をす
		OBBObject::OnCreate();
		Col4 Color(1.0f, 0.3f, 0.7f, 0.7f);
		GetComponent<PNTStaticDraw>()->SetDiffuse(Color);
		GetComponent<CollisionObb>()->SetDrawActive(true);
	}
	void AddlessCertificate::FindGoal() {
		auto goal = m_goal.lock();
		//goal->OpenGoal();
		m_isUnlockGoal = true;
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
	//チェインの文字を見せるオブジェクト
	//-------------------------------------------------------------------------------------------------
	ViewChainLetter::ViewChainLetter(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"chain_TX", Vec2(124, 32))
	{

	}

	void ViewChainLetter::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(740, 640));
		SetDrawLayer(1);
	}
	void ViewChainLetter::OnUpdate() {
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
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
				GetStage(), L"Number2_TX", Vec2(640,128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThisでThisオブジェクトのshared_ptrを取ってこれる
			transComp->SetParent(GetThis<ViewChainNum>());	// 基準点が画面の左上からScoreUIの場所になった
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			m_numbers.push_back(number);
		}

		auto flyingChain = GetStage()->AddGameObject<FlyingChain>();
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
	}

	void ViewChainNum::OnDraw() {
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			for (auto number : m_numbers) {
				number->OnDraw();
			}
		}
	}

	//-------------------------------------------------------------------------------------------------
	//スローの残り時間を見せるオブジェクト
	//-------------------------------------------------------------------------------------------------
	SlowTimeUI::SlowTimeUI(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"SlowTimeGage_TX", Vec2(32, 124))
	{

	}

	void SlowTimeUI::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(540, 600));
		SetDrawLayer(2);
		//SLOWの文字のスプライト
		m_text = GetStage()->AddGameObject<Sprite>(L"SlowText_TX", Vec2(32,124));
		m_text.lock()->SetPosition(Vec2(540,600));
		m_text.lock()->SetDrawLayer(3);
		//ゲージバーの背後の土台
		m_gageBase = GetStage()->AddGameObject<Sprite>(L"SlowTimeGageBase_TX", Vec2(32, 124));
		m_gageBase.lock()->SetPosition(Vec2(540, 600));
		m_gageBase.lock()->SetDrawLayer(1);
	}
	void SlowTimeUI::OnUpdate() {
		auto& manager = GameManager::GetInstance();
		float remainingGage = 1.0f - manager.GetSlowPassage();
		GetComponent<Transform>()->SetScale(1.0f, remainingGage, 1.0f);
		SetPosition(Vec2(540.0f, 662.0f - (62.0f * remainingGage)));

		if (manager.GetOnSlow()) {
			SetDrawActive(true);
			m_text.lock()->SetDrawActive(true);
			m_gageBase.lock()->SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
			m_text.lock()->SetDrawActive(false);
			m_gageBase.lock()->SetDrawActive(false);
		}
	}
	//--------------------------------------------------------------------------------------
	//チェインの数字
	//--------------------------------------------------------------------------------------
	FlyingChain::FlyingChain(const shared_ptr<Stage>& stagePtr)
		: GameObject(stagePtr) {
		m_places = 2;
	}
	//------------------------------------------------------------------------------------
	//構築
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
				GetStage(), L"Number2_TX", Vec2(640, 128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThisでThisオブジェクトのshared_ptrを取ってこれる
			transComp->SetParent(GetThis<FlyingChain>());	// 基準点が画面の左上からScoreUIの場所になった
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			m_numbers.push_back(number);
		}
		GetStage()->SetSharedGameObject(L"FlyingChain", GetThis<FlyingChain>());
	}

	void FlyingChain::OnUpdate() {
		int chain = m_chain;

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
			m_leap += App::GetApp()->GetElapsedTime() * 2.0f;
			auto pos = (1 - m_leap)*(1 - m_leap)*p0 + 2 * (1 - m_leap)*m_leap*p1 + m_leap * m_leap*p2;
			GetComponent<Transform>()->SetPosition(pos);
			if (m_leap >= 1.0f) {
				//スコアについたのでスコアの値を増加させる
				GameManager::GetInstance().AddScore(m_chain * 30 + m_chain * 10);
				m_leap = 0;
				m_isFly = false;
			}
		}

		OnDraw();
	}

	void FlyingChain::OnDraw() {
		if (m_isFly) {
			for (auto number : m_numbers) {
				number->OnDraw();
			}
		}
	}
	void FlyingChain::FlySet(int chain) {
		m_chain = chain;
		m_isFly = true;
	}
}