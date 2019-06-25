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
	//������
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
		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.4f, 0.0f)
		);

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"BUILDING_MODEL");
		drawComp->SetTextureResource(L"Building_TX");
		drawComp->SetLightingEnabled(false);
		SetAlphaActive(true);
		drawComp->SetMeshToTransformMatrix(spanMat);
		SetDrawLayer(-2);
	}
	///-----------------------------------------------------------------------------------
	// �X�V����
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

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.7f, 0.0f)
		);

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
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
	// �w�i�p�̃X�v���C�g���쐬
	//--------------------------------------------------------------------------------------------------------------
	SkyBox::SkyBox(const shared_ptr<Stage>& ptrStage)
		: GameObject(ptrStage)
	{
	}

	void SkyBox::OnCreate()
	{
		auto& app = App::GetApp();

		// �X�J�C�{�b�N�X�p�̃e�N�X�`���������Ă���t�H���_���w�肷��
		auto texturePath = app->GetDataDirWString() + L"textures/SkyBox/";
		vector <wstring> textureKeys;
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_Back_TX");
		textureKeys.push_back(L"SkyBox_TX");
		textureKeys.push_back(L"SkyBox_Plane_TX");

		// ���_�J���[�͂��ׂĔ�
		const Col4 white(1.0f, 1.0f, 1.0f, 1.0f);

		// �{�b�N�X�́u�p�v�ɂ����钸�_�̍��W�������܂Ƃ߂Ă���
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

		// ���_�̍��W����ׂāu�ʁv�����
		vector<vector<Vec3>> positionsArray = {
			{ positions[0], positions[1], positions[2], positions[3] },	// ����
			{ positions[1], positions[5], positions[3], positions[7] },	// �E��
			{ positions[5], positions[4], positions[7], positions[6] },	// ��O
			{ positions[4], positions[0], positions[6], positions[2] },	// ����
			{ positions[4], positions[5], positions[0], positions[1] },	// ��
			{ positions[2], positions[3], positions[6], positions[7] }	// ��
		};

		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		// �ʂ̐�(6��)�������[�v
		int textureIndex = 0;
		for (auto vpos : positionsArray) {
			// �ʂ��`�����钸�_�f�[�^���쐬
			vector<VertexPositionColorTexture> vertices = {
				{ vpos[0], white,{ 0.0f, 0.0f } },
				{ vpos[1], white,{ 1.0f, 0.0f } },
				{ vpos[2], white,{ 0.0f, 1.0f } },
				{ vpos[3], white,{ 1.0f, 1.0f } }
			};

			// �ʂ��I�u�W�F�N�g�Ƃ��Đ�������
			auto plane = ObjectFactory::Create<GameObject>(GetStage());

			// �ʂɂ̓h���[�R���|�[�l���g�����āA�|���S���ƃe�N�X�`����K�p����
			auto drawComp = plane->AddComponent<PCTStaticDraw>();
			drawComp->CreateOriginalMesh<VertexPositionColorTexture>(vertices, indices);
			drawComp->SetOriginalMeshUse(true);
			drawComp->SetTextureResource(textureKeys[textureIndex++]); // �t�H���_����ǂݍ��񂾃e�N�X�`�������ԂɓK�p

			// �X�J�C�{�b�N�X�͉\�Ȍ���傫������
			auto transComp = plane->GetComponent<Transform>();
			const float scale = 200.0f;
			transComp->SetScale(scale, scale, scale);

			m_planes.push_back(plane);

		}

		SetDrawLayer(-2);
	}

	void SkyBox::OnUpdate2()
	{
		// �X�J�C�{�b�N�X�̒��S�̓J�����̍��W�Ɠ����ɂ��Ă����Ȃ��Ƙc��Ō����Ă��܂�

		// �J�����̍��W���擾����
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto eye = camera->GetEye();

		// �S�Ă̖ʂɃJ�����̍��W��K�p����
		for (auto plane : m_planes) {
			auto transComp = plane->GetComponent<Transform>();
			// �X�J�C�{�b�N�X�����������̂ňʒu�𒲐�
			eye.y = 90.0f;
			transComp->SetPosition(eye);
		}

	}

	void SkyBox::OnDraw()
	{
		// �S�Ă̖ʂ�`�悷��
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

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.5f, 1.5f, 1.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
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
		// �l���G�t�F�N�g��\��
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
