#include"stdafx.h"
#include"Project.h"
#include "Enemy.h"

namespace basecross {
	Drone::Drone(const shared_ptr<Stage>& stage, Vec3 pos, RoopDirection dir,int chain)
		: OBBObject(stage, pos, Vec3(1.0f, 1.0f, 1.0f)), m_position(pos), m_roopDir(dir),m_deadChain(chain)
	{
	}
	void Drone::OnCreate() {
		if (m_roopDir >= 0 && m_roopDir <= 1) {
			SetBezierClock();
		}
		else if (m_roopDir >= 2) {
			SetVerticalMotion();
		}

		OBBObject::OnCreate();
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

		m_Lerp += App::GetApp()->GetElapsedTime() / 2.0f;
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
		}
		else {
			//player->SetJummerSpeed(1.0f);
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

		auto PtrTransform = GetComponent<Transform>();
		if (!m_DroneObject.expired()) {
			auto dronePtr = m_DroneObject.lock();
			auto droneTrans = dronePtr->GetComponent<Transform>();
			auto Pos = droneTrans->GetPosition();
			Pos.y += 2.0f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
			PtrTransform->SetQuaternion(droneTrans->GetQuaternion());
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

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(L"Number_TX");
			SetAlphaActive(true);
			SetDrawLayer(1);
		}
	}
	//�ω�
	void ViewDeadChain::OnUpdate() {

		if (!m_DroneObject.expired()) {
			auto dronePtr = m_DroneObject.lock();
			auto droneTrans = dronePtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = droneTrans->GetPosition();
			Pos.y += 2.0f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//�������r���{�[�h�ɂ���
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}

	}
}