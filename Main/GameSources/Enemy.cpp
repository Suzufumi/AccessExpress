#include"stdafx.h"
#include"Project.h"
#include "Enemy.h"

namespace basecross {
	Drone::Drone(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		:GameObject(stagePtr)
	{
		auto stage = GetTypeStage<GameStage>();
		if (stage)
		{
			// Xml����Enemy�̃p�����[�^���擾
			stage->LoadXmlParam(pNode,m_pos, m_quat, m_scale);
		}

		auto type = XmlDocReader::GetAttribute(pNode, L"DroneType");
		if (type == L"ClockWise")
		{
			m_roopDir = ClockWise;
		}
		else if (type == L"CounterClockwise")
		{
			m_roopDir = CounterClockwise;
		}
		else if (type == L"VerticalMotion")
		{
			m_roopDir = VerticalMotion;
		}
		else if (type == L"Wave")
		{
			m_roopDir = Wave;
		}

		auto chainStr = XmlDocReader::GetAttribute(pNode, L"Chain");

	}
	void Drone::OnCreate() {
		if (m_roopDir >= ClockWise && m_roopDir <= CounterClockwise) {
			//���v���,�t���v���
			SetBezierClock();
		}
		else if (m_roopDir == VerticalMotion) {
			//�㉺�^��
			SetVerticalMotion();
		}
		else if (m_roopDir == Wave) {
			//�g�̉^��
			SetWaveMotion();
		}

		auto transComp = GetComponent<Transform>();
		transComp->SetWorldPosition(m_pos);
		transComp->SetQuaternion(m_quat);
		transComp->SetScale(m_scale);

		GetStage()->GetSharedObjectGroup(L"Drone")->IntoGroup(GetThis<Drone>());
		SetDrawLayer(-1);
	}
	//-------------------------------------------------------------------------------
	// �A�b�v�f�[�g
	//-------------------------------------------------------------------------------
	void Drone::OnUpdate() {
		LeapUpdate();
		BezierMove();

		auto sparkPtr = GetStage()->GetSharedGameObject<SparkEffect>(L"SparkEffect", false);
		if (sparkPtr){
			sparkPtr->InsertSpark(GetComponent<Transform>()->GetWorldPosition());
		}
	}
	//-------------------------------------------------------------------------------
	// �`��
	//-------------------------------------------------------------------------------
	void Drone::OnDraw()
	{
		auto stage = GetTypeStage<GameStage>();
		// ���f���̕ϊ��s���ݒ�
		auto spanMat = stage->SetModelMatrix(Vec3(1.0f), Vec3(0.0f), Vec3(0.0f), Vec3(0.0f));

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"ENEMY_MODEL");
		drawComp->SetTextureResource(L"ENEMY_TX");
		drawComp->SetMeshToTransformMatrix(spanMat);

		GameObject::OnDraw();
	}
	//-------------------------------------------------------------------------------
	//�x�W�G�Ȑ��̌o�߂��X�V����
	//-------------------------------------------------------------------------------
	void Drone::LeapUpdate() {
		auto& gameManager = GameManager::GetInstance();
		auto addLerp = App::GetApp()->GetElapsedTime() / 2.0f;
		//�X���[��Ԃ��擾
		if (gameManager.GetOnSlow()) {
			//�X���[�ɂ���
			m_Lerp += addLerp / gameManager.GetSlowSpeed();
		}
		else {
			m_Lerp += addLerp;
		}
		//���̓�����100%�܂œ������玟�̓����Ɉڍs����
		if (m_Lerp >= 1.0f) {
			m_Lerp = 0.0f;
			m_joinNum += 1;
			//�Ō�̓������I������ŏ��̓����ɖ߂�
			if (m_joinNum >= m_joinNumMax) {
				m_joinNum = 0;
			}
		}
	}
	//-------------------------------------------------------------------------------
	//����
	//-------------------------------------------------------------------------------
	void Drone::BezierMove() {
		Vec3 pos = GetComponent<Transform>()->GetWorldPosition();
		//�ʒu�̍X�V
		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_joinNum].p0.x +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_joinNum].p1.x + m_Lerp * m_Lerp * m_roop[m_joinNum].p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_joinNum].p0.y +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_joinNum].p1.y + m_Lerp * m_Lerp * m_roop[m_joinNum].p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_joinNum].p0.z +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_joinNum].p1.z + m_Lerp * m_Lerp * m_roop[m_joinNum].p2.z;
		GetComponent<Transform>()->SetWorldPosition(pos);
	}
	//-------------------------------------------------------------------------------
	//���v���̓����̃x�W�G�Ȑ��̐���_�����
	//-------------------------------------------------------------------------------
	void Drone::SetBezierClock() {
		m_joinNumMax = 4;
		//���a
		float radius = 10.0f;
		//�x�W�G�Ȑ��ŉ~�̌`�ɓ��������߂̐���_����邽�߂ɁA
		//�R���X�g���N�^�Őݒ肳�ꂽ�ʒu�����ɂ��炵���_�����
		Vec3 centerPos;
		float deg90;
		float deg45;
		if (m_roopDir == DroneMotion::ClockWise) {
			centerPos = m_position + Vec3(radius, 0.0f, 0.0f);
			deg90 = Deg2Rad(-90.0f);
			deg45 = Deg2Rad(-45.0f);
		}
		if (m_roopDir == DroneMotion::CounterClockwise) {
			centerPos = m_position + Vec3(-radius, 0.0f, 0.0f);
			deg90 = Deg2Rad(90.0f);
			deg45 = Deg2Rad(45.0f);
		}
		//�_����ɉ~��ɓ_��z�u����
		for (int i = 0; i < m_joinNumMax; i++) {
			m_roop[i].p0 = centerPos + Vec3(radius * cosf(deg90*i), 0, radius * sinf(deg90*i));
			m_roop[i].p1 =
				centerPos + Vec3((radius + 4) * cosf(deg90*i + deg45), 0,
				(radius + 4) * sinf(deg90*i + deg45));
			m_roop[i].p2 = centerPos + Vec3(radius * cosf(deg90*(i + 1)), 0, radius * sinf(deg90*(i + 1)));
		}
	}
	//-------------------------------------------------------------------------------
	//�㉺�^���̃x�W�G�Ȑ��̐���_�����
	//-------------------------------------------------------------------------------
	void Drone::SetVerticalMotion() {
		m_joinNumMax = 1;
		m_roop[0].p0 = m_position;
		m_roop[0].p1 = m_position + Vec3(0.0f, 15.0f, 0.0f);
		m_roop[0].p2 = m_position;
	}
	//-------------------------------------------------------------------------------
	//�g�̓����̃x�W�G�Ȑ��̐���_�����
	//-------------------------------------------------------------------------------
	void Drone::SetWaveMotion() {
		m_joinNumMax = 8;
		auto halfJoinNum = m_joinNumMax / 2;
		m_roop[0].p0 = m_position;
		m_roop[0].p1 = m_position + Vec3(2.0f, 15.0f, 0.0f);
		m_roop[0].p2 = m_position + Vec3(4.0f, 0.0f, 0.0f);
		for (int i = 1; i < halfJoinNum; i++) {
			if (i % 2 == 0) {
				m_roop[i].p0 = m_roop[i - 1].p2;
				m_roop[i].p1 = m_roop[i - 1].p2 + Vec3(2.0f, 15.0f, 0.0f);
				m_roop[i].p2 = m_roop[i - 1].p2 + Vec3(4.0f, 0.0f, 0.0f);
			}
			else {
				m_roop[i].p0 = m_roop[i - 1].p2;
				m_roop[i].p1 = m_roop[i - 1].p2 + Vec3(2.0f, -15.0f, 0.0f);
				m_roop[i].p2 = m_roop[i - 1].p2 + Vec3(4.0f, 0.0f, 0.0f);
			}
		}
		for (int i = 0; i < halfJoinNum; i++) {
			if (i % 2 == 0) {
				m_roop[halfJoinNum + i].p0 = m_roop[halfJoinNum - i - 1].p2;
				m_roop[halfJoinNum + i].p1 = m_roop[halfJoinNum - i - 1].p1 + Vec3(0.0f, 30.0f, 0.0f);
				m_roop[halfJoinNum + i].p2 = m_roop[halfJoinNum - i - 1].p0;
			}
			else {
				m_roop[halfJoinNum + i].p0 = m_roop[halfJoinNum - i - 1].p2;
				m_roop[halfJoinNum + i].p1 = m_roop[halfJoinNum - i - 1].p1 + Vec3(0.0f, -30.0f, 0.0f);
				m_roop[halfJoinNum + i].p2 = m_roop[halfJoinNum - i - 1].p0;
			}
		}
	}

}