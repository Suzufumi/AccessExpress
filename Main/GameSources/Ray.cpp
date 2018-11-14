#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	���C��
	//--------------------------------------------------------------------------------------
	ActionLine::ActionLine(const shared_ptr<Stage>& StagePtr, Vec3 StartObj,
		Vec3 EndObj) :
		GameObject(StagePtr),
		m_StartObj(StartObj),
		m_EndObj(EndObj)
	{}

	//������
	void ActionLine::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f));
		ptrTrans->SetQuaternion(Quat());
		ptrTrans->SetPosition(Vec3(0.0f));

		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<PCStaticDraw>();

		auto startPos = m_StartObj;
		auto endPos = m_EndObj;

		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(startPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(endPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};

		auto meshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		meshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		ptrDraw->SetOriginalMeshResource(meshRes);
		ptrDraw->SetOriginalMeshUse(true);

	}

	void ActionLine::OnUpdate() {
		auto startPos = m_StartObj;
		auto endPos = m_EndObj;

		auto ptrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& backupVec = ptrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = backupVec[0];
		new_v.position = startPos;
		new_vec.push_back(new_v);

		new_v = backupVec[1];
		new_v.position = endPos;
		new_vec.push_back(new_v);

		ptrDraw->UpdateVertices(new_vec);
	}


	Ray::Ray(const shared_ptr<Stage>& stage,weak_ptr<GameObject> player)
		:GameObject(stage),m_player(player)
	{
	}

	void Ray::OnCreate() {

	}
	void Ray::OnUpdate() {
		auto obj = m_player.lock();
		auto player = dynamic_pointer_cast<Player>(obj);
		auto playerPos = player->GetComponent<Transform>()->GetWorldPosition();
		m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//player�ƃJ�����̈ʒu�����΂����������߂�
		auto dir = playerPos - m_cameraPos;
		dir = dir.normalize();
		//�����N�I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//������o��
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			auto linkTrans = linkObj->GetComponent<Transform>();
			//�����N�I�u�W�F�N�g��OBB�����
			OBB obb(linkTrans->GetScale(), linkTrans->GetWorldMatrix());
			//�v���C���[����ł�Ray��OBB�Ŕ���
			bool hit = HitTest::SEGMENT_OBB(playerPos, playerPos + dir * m_length, obb);
			
			if (hit){
				player->SetBezierPoint(linkTrans->GetWorldPosition());
			}
		}

		auto ptrActionLine = m_ActionLine.lock();
		if (ptrActionLine) {
			ptrActionLine->SetStartObj(playerPos);
			ptrActionLine->SetEndObj(playerPos + dir * m_length);
		}
		else {
			//���C���̍쐬
			auto ptrLine = GetStage()->AddGameObject<ActionLine>(playerPos, playerPos + dir * m_length);
			m_ActionLine = ptrLine;
		}
	}
}