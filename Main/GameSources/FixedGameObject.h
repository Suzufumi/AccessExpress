#pragma once
#include "stdafx.h"

namespace basecross {
	//----------------------------------------------------------------------------------------------------------
	//�����A���ɋ@�\�͂Ȃ�
	//----------------------------------------------------------------------------------------------------------
	class Wall : public GameObject {
		Vec3 m_position;
		Quat m_quat;
		Vec3 m_scale;
	public:
		Wall(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~Wall() {};
		virtual void OnCreate();
	};
	//--------------------------------------------------------------------------------------------------------------
	//�S�[��
	//--------------------------------------------------------------------------------------------------------------
	class Goal : public GameObject {
		Vec3 m_position;
		Quat m_quat;
		Vec3 m_scale;
	public:
		Goal(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~Goal() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		void ArriveGoal();
	};
	//-------------------------------------------------------------------------------------------------------------
	//�d�g�𔭂���I�u�W�F�N�g�A�߂Â���player�̃X�s�[�h���ς��
	//-------------------------------------------------------------------------------------------------------------
	class RadioTower : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
		float m_acceleration;
	public:
		RadioTower(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale, float acceleration);
		virtual ~RadioTower() { };
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		float GetAcceleration();
	};
	//--------------------------------------------------------------------------------------------------------------
	//�h���C�u
	//--------------------------------------------------------------------------------------------------------------
	class Drive : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
	public:
		Drive(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~Drive() {};
		virtual void OnCreate() override;
	};
}
//end basecross