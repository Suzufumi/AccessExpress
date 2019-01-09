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
		//Wall(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		Wall(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		virtual ~Wall() {};
		virtual void OnCreate();
	};
	//--------------------------------------------------------------------------------------------------------------
	//�S�[��
	//--------------------------------------------------------------------------------------------------------------
	class Goal : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
	public:
		Goal(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
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

	//--------------------------------------------------------------------------------------------------------------
	// �w�i�p�̋����쐬
	//--------------------------------------------------------------------------------------------------------------
	class SkySphere : public GameObject
	{
	public:
		SkySphere(const shared_ptr<Stage>& ptrStage) :
			GameObject(ptrStage) {}
		virtual ~SkySphere() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};

	//--------------------------------------------------------------------------------------------------------------
	// �w�i�p�̃X�v���C�g���쐬
	//--------------------------------------------------------------------------------------------------------------
	class SkySprite : public GameObject
	{
	private:
		Vec2 m_scale;
		Vec3 m_rotation;
		Vec3 m_position;
	public:
		SkySprite(const shared_ptr<Stage>& ptrStage, const Vec3 &rot, const Vec3 &pos);
		virtual ~SkySprite() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
//end basecross