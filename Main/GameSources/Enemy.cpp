#include"stdafx.h"
#include"Project.h"
#include "Enemy.h"

namespace basecross {
	Drone::Drone(const shared_ptr<Stage>& stage, IXMLDOMNodePtr pNode)
		: GameObject(stage)
	{
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto rotStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		auto pos = MyUtil::unityVec3StrToBCVec3(posStr);
		auto quat = MyUtil::unityQuatStrToBCQuat(rotStr);
		auto scale = MyUtil::unityVec3StrToBCVec3(scaleStr);

		m_position = pos;
		m_quat = quat;
		m_scale = scale;

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
		m_deadChain = MyUtil::wstrToInt(chainStr);

	}
	void Drone::OnCreate() {

		if (m_roopDir >= 0 && m_roopDir <= 1) {
			//���v���,�t���v���
			SetBezierClock();
		}
		else if (m_roopDir == 2) {
			//�㉺�^��
			SetVerticalMotion();
		}
		else if (m_roopDir == 3) {
			//�g�̉^��
			SetWaveMotion();
		}

		auto transComp = GetComponent<Transform>();
		transComp->SetWorldPosition(m_pos);
		transComp->SetQuaternion(m_quat);
		transComp->SetScale(m_scale);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		GetStage()->AddGameObject<ViewDeadChain>(GetThis<Drone>(), m_deadChain);

		GetStage()->GetSharedObjectGroup(L"Drone")->IntoGroup(GetThis<Drone>());
	}
	void Drone::OnUpdate() {
		Jammer();

		Vec3 pos = GetComponent<Transform>()->GetWorldPosition();

		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_joinNum].p0.x +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_joinNum].p1.x + m_Lerp * m_Lerp * m_roop[m_joinNum].p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_joinNum].p0.y +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_joinNum].p1.y + m_Lerp * m_Lerp * m_roop[m_joinNum].p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_joinNum].p0.z +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_joinNum].p1.z + m_Lerp * m_Lerp * m_roop[m_joinNum].p2.z;

		auto& gameManager = GameManager::GetInstance();
		auto addLerp = App::GetApp()->GetElapsedTime() / 2.0f;
		if (gameManager.GetOnSlow()) {
			m_Lerp += addLerp / gameManager.GetSlowSpeed();
		}
		else {
			m_Lerp += addLerp;
		}
		if (m_Lerp >= 1.0f) {
			m_Lerp = 0.0f;
			m_joinNum += 1;
			if (m_joinNum >= m_joinNumMax) {
				m_joinNum = 0;
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
		length = fabsf(length);
		if (length < m_JammerRang) {
			player->SetJummerSpeed(0.5f);
			player->SetJummer(true);
		}
		else {
			//���̃I�u�W�F�N�g�ɂ���ĖW�Q����ĂȂ�������
			if (!player->GetJummer()) {
				player->SetJummerSpeed(1.0f);
			}
		};
	}
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
	void Drone::SetVerticalMotion() {
		m_joinNumMax = 1;
		m_roop[0].p0 = m_position;
		m_roop[0].p1 = m_position + Vec3(0.0f, 15.0f, 0.0f);
		m_roop[0].p2 = m_position;
	}
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

	//-------------------------------------------------------------------------------------------------
	//���S����`�F�C������������I�u�W�F�N�g
	//-------------------------------------------------------------------------------------------------
	ViewDeadChain::ViewDeadChain(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<Drone>& DroneObjectPtr, size_t Number) :
		GameObject(StagePtr),
		m_DroneObject(DroneObjectPtr),
		m_Number(Number)
	{}

	//������
	void ViewDeadChain::OnCreate() {

		auto ptrTransform = GetComponent<Transform>();
		if (!m_DroneObject.expired()) {
			auto dronePtr = m_DroneObject.lock();
			auto droneTrans = dronePtr->GetComponent<Transform>();
			auto pos = droneTrans->GetPosition();
			pos.y += 2.0f;
			ptrTransform->SetPosition(pos);
			ptrTransform->SetScale(1.0f, 1.0f, 1.0f);
			ptrTransform->SetQuaternion(droneTrans->GetQuaternion());
			//�ύX�ł���X�N�G�A���\�[�X���쐬

			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Square�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSquare(1.0f, vertices, indices);
			//UV�l�̕ύX
			float from = ((float)m_Number) / 10.0f;
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

			auto drawComp = AddComponent<PCTStaticDraw>();
			drawComp->SetMeshResource(m_SquareMeshResource);
			drawComp->SetTextureResource(L"Number_TX");
			SetAlphaActive(true);
			SetDrawLayer(1);
		}
	}
	//�ω�
	void ViewDeadChain::OnUpdate() {

		if (!m_DroneObject.expired()) {
			auto dronePtr = m_DroneObject.lock();
			auto droneTrans = dronePtr->GetComponent<Transform>();

			auto ptrTransform = GetComponent<Transform>();
			auto pos = droneTrans->GetPosition();
			pos.y += 2.0f;
			ptrTransform->SetPosition(pos);
			ptrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat qt;
			//�������r���{�[�h�ɂ���
			qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			ptrTransform->SetQuaternion(qt);

		}

	}
}