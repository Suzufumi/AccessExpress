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
	void File::OnCreate(){
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
	void File::SetOnPlayer(weak_ptr<GameObject> obj){
		auto trans = GetComponent<Transform>();
		//�I�u�W�F�N�g�ɃA�N�Z�X
		auto player = obj.lock();
		//�I�u�W�F�N�g��e�ɂ���
		trans->SetParent(player);
		//�e�̏�ɏ��
		trans->SetPosition(Vec3(0, player->GetComponent<Transform>()->GetScale().y/2 + m_scale.y/2, 0));
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
	:OBBObject(stage,pos,scale)
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
		ptrTrans->SetWorldPosition(Vec3(0,0,0));
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(1,1,1);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//drawComp->SetFogEnabled(true);
		//�e���f�肱�܂Ȃ�
		drawComp->SetOwnShadowActive(false);
		Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
		drawComp->SetDiffuse(Color);
	}
	void SightingDevice::OnUpdate() {
		if (m_captureLink) {
			Col4 Color(1.0f, 0.4f, 0.7f, 0.7f);
			GetComponent<PNTStaticDraw>()->SetDiffuse(Color);
		}
		else {
			Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
			GetComponent<PNTStaticDraw>()->SetDiffuse(Color);
		}
	}
}