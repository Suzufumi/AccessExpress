#include"stdafx.h"
#include"Project.h"
#include "Enemy.h"

namespace basecross {
	Drone::Drone(const shared_ptr<Stage>& stage ,Vec3 pos,RoopDirection dir)
		: OBBObject(stage,pos,Vec3(1.0f,1.0f,1.0f)),m_position(pos),m_roopDir(dir)
	{
	}
	void Drone::OnCreate() {
		//���a
		float radius = 10.0f;
		//�x�W�G�Ȑ��ŉ~�̌`�ɓ��������߂̐���_����邽�߂ɁA
		//�R���X�g���N�^�Őݒ肳�ꂽ�ʒu�����ɂ��炵���_�����
		Vec3 centerPos;
		float deg90;
		float deg45;
		if (m_roopDir == RoopDirection::ClockWise) {
			centerPos = m_position + Vec3(radius, 0.0f, 0.0f);
			deg90 = Deg2Rad(-90.0f);
			deg45 = Deg2Rad(-45.0f);
		}
		if (m_roopDir == RoopDirection::CounterClockwise) {
			centerPos = m_position + Vec3(-radius, 0.0f, 0.0f);
			deg90 = Deg2Rad(90.0f);
			deg45 = Deg2Rad(45.0f);
		}
		//�_����ɉ~��ɓ_��z�u����
		for (int i = 0; i <= 3; i++) {
			m_roop[i].p0 = centerPos + Vec3(radius * cosf(deg90*i), 0, radius * sinf(deg90*i));
			m_roop[i].p1 = 
				centerPos + Vec3((radius + 4) * cosf(deg90*i + deg45),0,
				(radius + 4) * sinf(deg90*i + deg45));
			m_roop[i].p2 = centerPos + Vec3(radius * cosf(deg90*(i + 1)), 0, radius * sinf(deg90*(i + 1)));
		}
		OBBObject::OnCreate();

		GetStage()->GetSharedObjectGroup(L"Drone")->IntoGroup(GetThis<Drone>());
	}
	void Drone::OnUpdate() {
		Jammer();

		Vec3 pos = GetComponent<Transform>()->GetWorldPosition();

		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_roopNum].p0.x + 
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_roopNum].p1.x + m_Lerp * m_Lerp * m_roop[m_roopNum].p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_roopNum].p0.y +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_roopNum].p1.y + m_Lerp * m_Lerp * m_roop[m_roopNum].p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_roopNum].p0.z +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_roopNum].p1.z + m_Lerp * m_Lerp * m_roop[m_roopNum].p2.z;

		m_Lerp += App::GetApp()->GetElapsedTime()/2.0f;
		if (m_Lerp >= 1.0f) {
			m_Lerp = 0.0f;
			m_roopNum += 1;
			if (m_roopNum > 3) {
				m_roopNum = 0;
			}
		}
		GetComponent<Transform>()->SetWorldPosition(pos);

		if (m_isDead) {
			GetStage()->GetSharedGameObject<Player>(L"Player")->SetJummerSpeed(1.0f);
			SetUpdateActive(false);
			//GetStage()->RemoveGameObject<Drone>(GetThis<Drone>());
		}
	}
	void Drone::Jammer() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetComponent<Transform>()->GetWorldPosition();
		auto dronePos = GetComponent<Transform>()->GetWorldPosition();

		auto length = (dronePos - playerPos).length();
		if (length < m_JammerRang) {
			player->SetJummerSpeed(0.5f);
		}
		else {
			player->SetJummerSpeed(1.0f);
		};		
	}
	//-------------------------------------------------------------------------------------------------
	//���S����`�F�C������������I�u�W�F�N�g
	//-------------------------------------------------------------------------------------------------
	ViewDeadChain::ViewDeadChain(const shared_ptr<Stage>& stage,int chain) 
		:GameObject(stage),m_deadChain(chain)
	{
	}
	void ViewDeadChain::OnCreate() {
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
		//�e���f�肱�܂Ȃ�
		drawComp->SetOwnShadowActive(false);
		//���C�g�𖳎����Č���
		drawComp->SetLightingEnabled(false);
		//�A���t�@�l�K��
		drawComp->SetBlendState(BlendState::AlphaBlend);
		//�e�N�X�`��
		drawComp->SetTextureResource(L"cursor_TX");
	}
}