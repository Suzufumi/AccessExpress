#include"stdafx.h"
#include"Project.h"

namespace basecross {
	Drone::Drone(const shared_ptr<Stage>& stage ,Vec3 pos)
		: OBBObject(stage,pos,Vec3(1.0f,1.0f,1.0f)),m_position(pos)
	{
	}
	void Drone::OnCreate() {
		//���a
		float radius = 3.0f;
		//�x�W�G�Ȑ��ŉ~�̌`�ɓ��������߂̐���_����邽�߂ɁA
		//�R���X�g���N�^�Őݒ肳�ꂽ�ʒu�����ɂ��炵���_�����
		Vec3 centerPos = m_position + Vec3(-radius, 0.0f, 0.0f);
		//�_����ɉ~��ɓ_��z�u����
		for (int i = 0; i < 3; i++) {
			m_roop[i].p1 = centerPos + Vec3(radius * cosf(90.0f*i), 0, radius * sinf(90.0f*i));
			m_roop[i].p2 = centerPos + Vec3((radius + 2) * cosf(90.0f*i), 0, (radius + 2) * sinf(90.0f*i));
			m_roop[i].p3 = centerPos + Vec3(radius * cosf(90.0f*(i + 1)), 0, radius * sinf(90.0f*(i + 1)));
		}
		OBBObject::OnCreate();
	}
}