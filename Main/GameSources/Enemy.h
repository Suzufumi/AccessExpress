#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Drone : public OBBObject {
		struct BezierPoint {
			Vec3 p0; //�n�_
			Vec3 p1;
			Vec3 p2; //�I�_
		};
		BezierPoint m_roop[8];
		float m_Lerp = 0.0f;
		int m_joinNum = 0;
		int m_joinNumMax;
		Vec3 m_position;
		int m_deadChain;
		bool m_isDead = false;
		//�W�Q�d�g�͈̔�
		float m_JammerRang = 5.0f;
	public:
		enum DroneMotion {
			ClockWise = 0,			//���v���
			CounterClockwise = 1,	//�t���v���
			VerticalMotion = 2,		//�㉺�^��
			Wave = 3				//�g�̓����A���i�񂾂�Ԃ��Ă���
		};
		DroneMotion m_roopDir;

		Drone(const shared_ptr<Stage>& stage, Vec3 pos, DroneMotion dir,int chain);
		virtual ~Drone() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		int GetDeadChain() { return m_deadChain; };
		void Die() { m_isDead = true; };
		//�W�Q�d�g�̏���
		void Jammer();
		//���v���̓����̃x�W�G�Ȑ��̐���_�����
		void SetBezierClock();
		//�㉺�^���̃x�W�G�Ȑ��̐���_�����
		void SetVerticalMotion();
		//�g�̓����̃x�W�G�Ȑ��̐���_�����
		void SetWaveMotion();
	};

	class ViewDeadChain : public GameObject {
		weak_ptr<Drone> m_DroneObject;
		//���̃I�u�W�F�N�g�݂̂Ŏg�p����X�N�G�A���b�V��
		shared_ptr<MeshResource> m_SquareMeshResource;
		//�w�ԍ�
		size_t m_Number;

		Quat Billboard(const Vec3& Line) {
			Vec3 Temp = Line;
			Mat4x4 RotMatrix;
			Vec3 DefUp(0, 1.0f, 0);
			Vec2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.length() < 0.1f) {
				DefUp = Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix.inverse();
			Quat Qt;
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
			return Qt;
		}

	public:
		//�\�z�Ɣj��
		ViewDeadChain(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<Drone>& DroneObjectPtr, size_t Number);
		virtual ~ViewDeadChain() {};
		//������
		virtual void OnCreate() override;
		//�ω�
		virtual void OnUpdate() override;
	};
}