#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Drone : public GameObject {
		Vec3 m_pos;
		Quat m_quat;
		Vec3 m_scale;
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
	public:
		enum DroneMotion {
			ClockWise = 0,			//���v���
			CounterClockwise = 1,	//�t���v���
			VerticalMotion = 2,		//�㉺�^��
			Wave = 3				//�g�̓����A���i�񂾂�Ԃ��Ă���
		};
		DroneMotion m_roopDir;

		Drone(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		virtual ~Drone() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		//�o�߂��X�V����
		void LeapUpdate();
		//����
		void BezierMove();
		//���v���̓����̃x�W�G�Ȑ��̐���_�����
		void SetBezierClock();
		//�㉺�^���̃x�W�G�Ȑ��̐���_�����
		void SetVerticalMotion();
		//�g�̓����̃x�W�G�Ȑ��̐���_�����
		void SetWaveMotion();
	};

}