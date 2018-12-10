#include"stdafx.h"
#include"Project.h"

namespace basecross {
	//---------------------------------------------------------------------------------------------
	//�d�g���Ƃ̏Փ˔�����s�������蔻��̃R���X�g���N�^
	//---------------------------------------------------------------------------------------------
	RadioTowerHitJudgment::RadioTowerHitJudgment(const shared_ptr<Stage>& StagePtr, weak_ptr<GameObject> player)
		:GameObject(StagePtr), m_player(player)
	{
	}
	void RadioTowerHitJudgment::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(10.0f, 10.0f, 10.0f);

		//���`�̓����蔻����Z�b�g
		auto col = AddComponent<CollisionSphere>();
		//�I�u�W�F�N�g�Ɠ������Ă��������Ȃ�
		col->SetAfterCollision(AfterCollision::None);
		//�����蔻�肪������悤�ɂ���
		//col->SetDrawActive(true);
		//Player�ƂԂ���Ȃ��悤�ɂ��邽�߂̃^�O
		AddTag(L"PlayerUse");
		//�R���X�g���N�^�Ŏ擾�����v���C���[�ɃA�N�Z�X
		auto obj = m_player.lock();
		//player�Ɛe�q�֌W�ɂȂ�
		ptrTrans->SetParent(obj);
		//player�Ɠ��ʒu�ɂȂ�
		ptrTrans->SetWorldPosition(obj->GetComponent<Transform>()->GetWorldPosition());
	}
	//-------------------------------------------------------------------------------------------------------------
	//�q�b�g���Ă���Œ�
	//-------------------------------------------------------------------------------------------------------------
	void RadioTowerHitJudgment::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		//�Փ˂������̂��d�g�������ׂ�
		auto radioTower = dynamic_pointer_cast<RadioTower>(Other);
		//�d�g��������
		if (radioTower) {
			//Player�ʒu����d�g���̈ʒu������
			Vec3 d = this->GetComponent<Transform>()->GetWorldPosition() - radioTower->GetComponent<Transform>()->GetWorldPosition();
			//�ʒu�̍����狗�����o��
			float leng = d.length();
			//�����o�������������ݎ����Ă��鋗�������߂�������
			if (m_length > leng) {
				//��ԋ߂��������X�V����
				m_length = leng;
				//���x���擾����
				m_Acceleration = radioTower->GetAcceleration();
			}
		}
	}

	//-------------------------------------------------------------------------------------
	//�t�@�C��
	//-------------------------------------------------------------------------------------
	File::File(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}
	//-------------------------------------------------------------------------------------
	//������
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
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//drawComp->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		drawComp->SetOwnShadowActive(true);
		Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
		drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		//�^�O�t��
		AddTag(L"File");
	}
	void File::SetOnPlayer(weak_ptr<GameObject> obj) {
		auto trans = GetComponent<Transform>();
		//�I�u�W�F�N�g�ɃA�N�Z�X
		auto player = obj.lock();
		//�I�u�W�F�N�g��e�ɂ���
		trans->SetParent(player);
		//�e�̏�ɏ��
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
		//�e�I�u�W�F�N�g�̃R���X�g���N�^���ĂԁA�����Ō`�̌`������
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
	//�Ə�
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
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//�e���f�肱�܂Ȃ�
		drawComp->SetOwnShadowActive(false);
		//���C�g�𖳎����Č���
		drawComp->SetLightingEnabled(false);
		//�A���t�@�l�K��
		drawComp->SetBlendState(BlendState::AlphaBlend);
		//�e�N�X�`��
		drawComp->SetTextureResource(L"cursor_TX");
		//�Ə���ʂ��Đ��E������Ƃق��̃I�u�W�F�N�g�������ɂȂ��Ă��܂��̂�
		//���C���[�ʒu��ς��邱�ƂőΉ�
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
	//�`�F�C���̕�����������I�u�W�F�N�g
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
	//�`�F�C���̕�����������I�u�W�F�N�g
	//-------------------------------------------------------------------------------------------------
	ViewChainNum::ViewChainNum(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"Number_TX", Vec2(32, 64))
	{

	}

	void ViewChainNum::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(720, 590));

		//���_�z��
		vector<VertexPositionNormalTexture> vertices;
		//�C���f�b�N�X���쐬���邽�߂̔z��
		vector<uint16_t> indices;
		//Square�̍쐬(�w���p�[�֐��𗘗p)
		MeshUtill::CreateSquare(1.0f, vertices, indices);
		//UV�l�̕ύX
		float from = ((float)5) / 10.0f;
		float to = from + (1.0f / 10.0f);
		//���㒸�_
		vertices[0].textureCoordinate = Vec2(from, 0);
		//�E�㒸�_
		vertices[1].textureCoordinate = Vec2(to, 0);
		//�������_
		vertices[2].textureCoordinate = Vec2(from, 1.0f);
		//�E�����_
		vertices[3].textureCoordinate = Vec2(to, 1.0f);
		//���_�̌^��ς����V�������_���쐬
		vector<VertexPositionColorTexture> new_vertices;
		for (auto& v : vertices) {
			VertexPositionColorTexture nv;
			nv.position = v.position;
			nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			nv.textureCoordinate = v.textureCoordinate;
			new_vertices.push_back(nv);
		}
		//�V�������_���g���ă��b�V�����\�[�X�̍쐬
		m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);
		
		//auto drawComp = AddComponent<PCTStaticDraw>();
		//drawComp->SetMeshResource(m_SquareMeshResource);
		//drawComp->SetTextureResource(L"Number_TX");
		SetMesh(m_SquareMeshResource);
		SetAlphaActive(true);
		
		SetDrawLayer(1);
		
	}
	void ViewChainNum::OnUpdate() {
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
		}
	}

}