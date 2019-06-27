#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Wall::Wall(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode)
		: GameObject(StagePtr)
	{
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		auto pos = MyUtil::unityVec3StrToBCVec3(posStr);
		auto scale = MyUtil::unityVec3StrToBCVec3(scaleStr);

		m_position = pos;
		m_scale = scale;

	}
	//--------------------------------------------------------------------------------------
	//初期化
	//-------------------------------------------------------------------------------------
	void Wall::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		//col->SetDrawActive(true);
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.4f, 0.0f)
		);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"BUILDING_MODEL");
		drawComp->SetTextureResource(L"Building_TX");
		drawComp->SetLightingEnabled(false);
		SetAlphaActive(true);
		drawComp->SetMeshToTransformMatrix(spanMat);
		SetDrawLayer(-2);
	}
	///-----------------------------------------------------------------------------------
	// 更新処理
	///-----------------------------------------------------------------------------------
	void Wall::OnUpdate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		if (80.0f >= (pos - player->GetComponent<Transform>()->GetWorldPosition()).length()) {
			SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
		}
	}
	///-----------------------------------------------------------------------------------
	// CheckPoint
	///-----------------------------------------------------------------------------------

	CheckPoint::CheckPoint(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: GameObject(stagePtr)
	{
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto quatStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		auto pos = MyUtil::unityVec3StrToBCVec3(posStr);
		auto quat = MyUtil::unityVec3StrToBCVec3(quatStr);
		auto scale = MyUtil::unityVec3StrToBCVec3(scaleStr);

		m_position = pos;
		m_qt = quat;
		m_scale = scale;

	}
	CheckPoint::CheckPoint(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat quat, Vec3 scale)
		:GameObject(stagePtr),m_position(pos),m_qt(quat),m_scale(scale)
	{
	}

	void CheckPoint::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		//auto col = AddComponent<CollisionObb>();
		//col->SetAfterCollision(AfterCollision::None);
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_qt);
		ptrTrans->SetScale(m_scale);

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.7f, 0.0f)
		);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		SetDrawLayer(-1);
		drawComp->SetMeshResource(L"CHECKPOINT_MODEL");
		drawComp->SetMeshToTransformMatrix(spanMat);
		//drawComp->SetLightingEnabled(false);
		//Col4 Color(1.0f, 1.0f, 1.0f, 0.5f);
		//drawComp->SetDiffuse(Color);
		SetAlphaActive(true);

		//GetStage()->GetSharedObjectGroup(L"CheckPoints")->IntoGroup(GetThis<GameObject>());
	}
	void CheckPoint::OnUpdate() {

	}
	void CheckPoint::ArriveCheckPoint() {
		auto& gm = GameManager::GetInstance();
		int count = 0;
		while (gm.GetMail() > 0)
		{
			gm.DecreaseMail();
			count++;
		}
		gm.AddMaxMail(count);
		Col4 col(0.4f, 1.0f, 0.4f, 1.0f);
		auto drawComp = GetComponent<BcPNTStaticDraw>();
		drawComp->SetDiffuse(col);
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
		auto& app = App::GetApp();

		// スカイボックス用のテクスチャが入っているフォルダを指定する
		auto texturePath = app->GetDataDirWString() + L"textures/SkyBox/";
		vector <wstring> textureKeys;
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_TX");
		textureKeys.push_back(L"SkyBox_Plane_TX");

		// 頂点カラーはすべて白
		const Col4 white(1.0f, 1.0f, 1.0f, 1.0f);

		// ボックスの「角」にあたる頂点の座標だけをまとめておく
		const vector<Vec3> positions{
			{ -0.5f, +0.5f, +0.5f },	// 0
			{ +0.5f, +0.5f, +0.5f },	// 1
			{ -0.5f, -0.5f, +0.5f },	// 2
			{ +0.5f, -0.5f, +0.5f },	// 3

			{ -0.5f, +0.5f, -0.5f },	// 4
			{ +0.5f, +0.5f, -0.5f },	// 5
			{ -0.5f, -0.5f, -0.5f },	// 6
			{ +0.5f, -0.5f, -0.5f }		// 7
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
			const float scale = 200.0f;
			transComp->SetScale(scale, scale, scale);

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
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		auto pos = MyUtil::unityVec3StrToBCVec3(posStr);
		auto scale = MyUtil::unityVec3StrToBCVec3(scaleStr);

		m_position = pos;
		m_scale = scale;

	}
	MailObject::MailObject(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{

	}

	void MailObject::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		//auto col = AddComponent<CollisionObb>();
		//col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.5f, 1.5f, 1.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		SetDrawLayer(-1);
		drawComp->SetMeshResource(L"MAIL_MODEL");
		drawComp->SetTextureResource(L"MAIL_TX");
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetLightingEnabled(false);
		//Col4 Color(1.0f, 1.0f, 0.0f, 1.0f);
		//drawComp->SetDiffuse(Color);

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
		RotateMail();
	}
	void MailObject::ArriveMail() {
		auto& gm = GameManager::GetInstance();
		gm.AddMail();
		m_isArrive = true;
		SetDrawActive(false);
		App::GetApp()->GetScene<Scene>()->MusicOnceStart(L"Mail_SE", 1.0f);
		// 獲得エフェクトを表示
		auto ptrEffect = GetStage()->GetSharedGameObject<GetEffect>(L"GetEffect", false);
		//MessageBox(NULL, L"", L"", MB_OK);
		if (ptrEffect) {
			ptrEffect->InsertGetEffect(GetComponent<Transform>()->GetWorldPosition());
		}
	}

	void MailObject::RotateMail()
	{
		m_rot += m_rotateSpeed;
		if (m_rot >= 360.0f)
		{
			m_rot = 0.0f;
		}
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, Deg2Rad(m_rot), 0));
		auto transComp = GetComponent<Transform>();
		transComp->SetQuaternion(Qt);

	}

}
