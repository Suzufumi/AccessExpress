#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TpsCamera::TpsCamera()
		: Camera(),
		m_AngleX(Deg2Rad(10.0f)),
		m_AngleY(Deg2Rad(270)),
		m_maxAngleSpeed(2.0f),
		m_cameraDistance(13.0f),
		m_cameraLookUp(3.5f)
	{}

	TpsCamera::~TpsCamera()
	{}

	void TpsCamera::OnCreate()
	{
		
	}

	void TpsCamera::OnUpdate()
	{

	}

	void TpsCamera::SetTarget(const shared_ptr<Transform>& target)
	{
	}
	//--------------------------------------------------------------------------------------------
	//�x�W�F�Ȑ��Ŕ�΂�(player�ƈꏏ�ɓ������߂�player�̌���Leap��������Ă���)
	//--------------------------------------------------------------------------------------------
	void TpsCamera::BezierMove(float leap,Vec3 playerPos) {
		Vec3 pos;
		pos = (1 - leap)*(1 - leap)*p0 + 2 * (1 - leap)*leap*p1 + leap * leap*p2;
		SetEye(pos);
		SetAt(playerPos + Vec3(0.0f,3.0f,0.0f));
	}
	//--------------------------------------------------------------------------------------------
	//����_�̐ݒ�
	//--------------------------------------------------------------------------------------------
	void TpsCamera::SetBezier(weak_ptr<GameObject> playerPtr,Vec3 p2Player) {
		auto player = playerPtr.lock();
		//��ԑO�̃v���C���[�ƃJ�����̈ʒu�֌W������
		Vec3 a = player->GetComponent<Transform>()->GetWorldPosition() - GetEye();
		p0 = GetEye();		//�n�_
		p2 = p2Player - a;	//�I�_
		//�n�_�ƏI�_�̒��_����a�̕����ɂ��炵�ė^����
		p1 = (p2 - ((p2 - p0) / 2)) - a.normalize() * Vec3(p2 - p0).length();
	}
	//--------------------------------------------------------------------------------------------
	//�J�����ƂƂ��ɓ�������̃R���X�g���N�^
	//--------------------------------------------------------------------------------------------
	TpsCameraJudgment::TpsCameraJudgment(const shared_ptr<Stage>& StagePtr)
		:GameObject(StagePtr)
	{
	}
	//--------------------------------------------------------------------------------------------
	//�쐬
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCreate() {
		GetComponent<Transform>()->SetScale(3.0f, 3.0f, 3.0f);

		AddComponent<CollisionObb>();
	}
	//--------------------------------------------------------------------------------------------
	//�X�V
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnUpdate() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		GetComponent<Transform>()->SetWorldPosition(tpsCamera->GetEye());
	}
	//--------------------------------------------------------------------------------------------
	//�Փˎ�����
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		//�ǂɃJ����������������A�ǂ������Ȃ�����
		if (wall) {
			wall->SetDrawActive(false);
		}
	}
	//--------------------------------------------------------------------------------------------
	//�Փˉ���������
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCollisionExit(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		//�Փ˂��������ꂽ�ǂ�������悤�ɂ���
		if (wall) {
			wall->SetDrawActive(true);
		}
	}
}