#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Wall::Wall(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode)
		: GameObject(StagePtr)
	{
		auto stage = GetTypeStage<GameStage>();
		if (stage)
		{
			// XmlからWallのパラメータを取得
			stage->LoadXmlParam(pNode, m_position, m_quat, m_scale);
		}

	}
	//--------------------------------------------------------------------------------------
	//初期化
	//-------------------------------------------------------------------------------------
	void Wall::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		// コリジョンの衝突応答をNoneにする
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quat);
		ptrTrans->SetScale(m_scale);
		SetDrawLayer(-2);
	}
	///-----------------------------------------------------------------------------------
	// 更新処理
	///-----------------------------------------------------------------------------------
	void Wall::OnUpdate() {
		// Wallの見える範囲
		const float VisibleRange = 80.0f;
		// Playerのポインタの取得
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		// Wallの座標を取得
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		// Playerの座標がWallの見える範囲を超えたら
		if (VisibleRange >= (pos - player->GetComponent<Transform>()->GetWorldPosition()).length()) {
			SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
		}
	}
	///-----------------------------------------------------------------------------------
	// 描画処理
	///-----------------------------------------------------------------------------------
	void Wall::OnDraw()
	{
		const Vec3 Position = Vec3(0.0f, -1.4f, 0.0f);
		auto stage = GetTypeStage<GameStage>();
		// モデルの変換行列設定
		auto spanMat = stage->SetModelMatrix(Vec3(0.5f), Vec3(0.0f), Vec3(0.0f), Position);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"BUILDING_MODEL");
		drawComp->SetTextureResource(L"Building_TX");
		drawComp->SetLightingEnabled(false);
		SetAlphaActive(true);
		drawComp->SetMeshToTransformMatrix(spanMat);

		GameObject::OnDraw();
	}
	///-----------------------------------------------------------------------------------
	// アンテナ
	///-----------------------------------------------------------------------------------
	Antenna::Antenna(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat quat, Vec3 scale)
		:GameObject(stagePtr),m_position(pos),m_qt(quat),m_scale(scale)
	{
	}

	void Antenna::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_qt);
		ptrTrans->SetScale(m_scale);

		SetAlphaActive(true);
	}

	void Antenna::OnDraw()
	{
		const Vec3 Position = Vec3(0.0f, -0.7f, 0.0f);
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f),
			Position
		);
		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		SetDrawLayer(-1);
		drawComp->SetMeshResource(L"CHECKPOINT_MODEL");
		drawComp->SetMeshToTransformMatrix(spanMat);

		GameObject::OnDraw();
	}

	//--------------------------------------------------------------------------------------------------------------
	// 背景用のスプライトを作成
	//--------------------------------------------------------------------------------------------------------------
	SkyBox::SkyBox(const shared_ptr<Stage>& ptrStage)
		: GameObject(ptrStage)
	{
	}

	void SkyBox::OnCreate()
	{
		const float VertexPosition = 0.5f;
		auto& app = App::GetApp();

		// スカイボックス用のテクスチャが入っているフォルダを指定する
		auto texturePath = app->GetDataDirWString() + L"textures/SkyBox/";
		vector <wstring> textureKeys;
		const int SkyBoxBack_Size = 4;	// スカイボックスの背景サイズ
		for (int i = 0; i < SkyBoxBack_Size; i++)
		{
			textureKeys.push_back(L"SkyBox_Back_TX");
		}
		textureKeys.push_back(L"SkyBox_TX");
		textureKeys.push_back(L"SkyBox_Plane_TX");

		// 頂点カラーはすべて白
		const Col4 white(1.0f, 1.0f, 1.0f, 1.0f);

		// ボックスの「角」にあたる頂点の座標だけをまとめておく
		const vector<Vec3> positions{
			{ -VertexPosition, +VertexPosition, +VertexPosition },	// 0
			{ +VertexPosition, +VertexPosition, +VertexPosition },	// 1
			{ -VertexPosition, -VertexPosition, +VertexPosition },	// 2
			{ +VertexPosition, -VertexPosition, +VertexPosition },	// 3

			{ -VertexPosition, +VertexPosition, -VertexPosition },	// 4
			{ +VertexPosition, +VertexPosition, -VertexPosition },	// 5
			{ -VertexPosition, -VertexPosition, -VertexPosition },	// 6
			{ +VertexPosition, -VertexPosition, -VertexPosition }	// 7
		};

		// 頂点の座標を並べて「面」を作る
		vector<vector<Vec3>> positionsArray = {
			{ positions[0], positions[1], positions[2], positions[3] },	// 正面
			{ positions[1], positions[5], positions[3], positions[7] },	// 右手
			{ positions[5], positions[4], positions[7], positions[6] },	// 手前
			{ positions[4], positions[0], positions[6], positions[2] },	// 左手
			{ positions[4], positions[5], positions[0], positions[1] },	// 上
			{ positions[2], positions[3], positions[6], positions[7] }	// 下
		};

		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		// 面の数(6面)だけループ
		int textureIndex = 0;
		for (auto vpos : positionsArray) {
			// 面を形成する頂点データを作成
			vector<VertexPositionColorTexture> vertices = {
				{ vpos[0], white,{ 0.0f, 0.0f } },
				{ vpos[1], white,{ 1.0f, 0.0f } },
				{ vpos[2], white,{ 0.0f, 1.0f } },
				{ vpos[3], white,{ 1.0f, 1.0f } }
			};

			// 面をオブジェクトとして生成する
			auto plane = ObjectFactory::Create<GameObject>(GetStage());

			// 面にはドローコンポーネントをつけて、ポリゴンとテクスチャを適用する
			auto drawComp = plane->AddComponent<PCTStaticDraw>();
			drawComp->CreateOriginalMesh<VertexPositionColorTexture>(vertices, indices);
			drawComp->SetOriginalMeshUse(true);
			drawComp->SetTextureResource(textureKeys[textureIndex++]); // フォルダから読み込んだテクスチャを順番に適用

			// スカイボックスは可能な限り大きくする
			auto transComp = plane->GetComponent<Transform>();
			const float SphereScale = 200.0f;
			transComp->SetScale(Vec3(SphereScale));

			m_planes.push_back(plane);

		}

		SetDrawLayer(-2);
	}

	void SkyBox::OnUpdate2()
	{
		// スカイボックスの中心はカメラの座標と同じにしておかないと歪んで見えてしまう

		// カメラの座標を取得する
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto eye = camera->GetEye();

		// 全ての面にカメラの座標を適用する
		for (auto plane : m_planes) {
			auto transComp = plane->GetComponent<Transform>();
			// スカイボックスが下すぎたので位置を調整
			eye.y = 90.0f;
			transComp->SetPosition(eye);
		}

	}

	void SkyBox::OnDraw()
	{
		// 全ての面を描画する
		for (auto plane : m_planes) {
			plane->OnDraw();
		}

	}
	///-----------------------------------------------------------------------------------
	// Mail
	///-----------------------------------------------------------------------------------

	MailObject::MailObject(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: GameObject(stagePtr)
	{
		auto stage = GetTypeStage<GameStage>();
		if (stage)
		{
			// Xmlからメールを取得
			stage->LoadXmlParam(pNode, m_position, m_quat, m_scale);
		}
	}
	MailObject::MailObject(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{

	}

	void MailObject::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);

		GetStage()->GetSharedObjectGroup(L"Mails")->IntoGroup(GetThis<GameObject>());
	}
	void MailObject::OnUpdate() {
		if (m_isArrive) {
			m_passageTime += App::GetApp()->GetElapsedTime();
			if (m_passageTime >= 10.0f) {
				m_isArrive = false;
				SetDrawActive(true);
				m_passageTime = 0;
			}
		}
		// メールの回転処理
		RotateMail();
	}
	void MailObject::OnDraw()
	{
		auto stage = GetTypeStage<GameStage>();
		// モデルの変換行列設定
		auto spanMat = stage->SetModelMatrix(Vec3(1.5f), Vec3(0.0f), Vec3(0.0f), Vec3(0.0f));

		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		SetDrawLayer(-1);
		drawComp->SetMeshResource(L"MAIL_MODEL");
		drawComp->SetTextureResource(L"MAIL_TX");
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetLightingEnabled(false);

		GameObject::OnDraw();

	}

	void MailObject::ArriveMail() {
		// SEの定数定義
		const float SE_Volume = 1.0f;
		auto& gm = GameManager::GetInstance();
		gm.AddMail();
		m_isArrive = true;
		SetDrawActive(false);
		App::GetApp()->GetScene<Scene>()->MusicOnceStart(L"Mail_SE", SE_Volume);
		// 獲得エフェクトを表示
		auto ptrEffect = GetStage()->GetSharedGameObject<GetEffect>(L"GetEffect", false);
		if (ptrEffect) {
			ptrEffect->InsertGetEffect(GetComponent<Transform>()->GetWorldPosition());
		}
	}

	void MailObject::RotateMail()
	{
		// 最大回転数を定義
		const float MaxRotation = 360.0f;
		m_rot += m_rotateSpeed;
		if (m_rot >= MaxRotation)
		{
			m_rot = 0.0f;
		}
		Quat Qt;
		// Vectorからクォータニオンに変換
		Qt.rotationRollPitchYawFromVector(Vec3(0, Deg2Rad(m_rot), 0));
		auto transComp = GetComponent<Transform>();
		transComp->SetQuaternion(Qt);

	}

}
