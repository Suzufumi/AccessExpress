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
		m_cameraLookUp(3.0f)
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
}