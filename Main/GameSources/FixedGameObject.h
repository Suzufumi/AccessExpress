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
		virtual void OnUpdate();
	};
	//--------------------------------------------------------------------------------------------------------------
	//�S�[��
	//--------------------------------------------------------------------------------------------------------------
	class CheckPoint : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
		Quat m_qt;
	public:
		CheckPoint(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		CheckPoint(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat quat, Vec3 scale);
		virtual ~CheckPoint() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		void ArriveCheckPoint();
	};
	//--------------------------------------------------------------------------------------------------------------
	// �w�i�p�̃X�v���C�g���쐬
	//--------------------------------------------------------------------------------------------------------------
	class SkyBox : public GameObject
	{
	private:
		vector <shared_ptr<GameObject>> m_planes;
	public:
		SkyBox(const shared_ptr<Stage>& ptrStage);
		virtual ~SkyBox() {};

		virtual void OnCreate() override;
		virtual void OnUpdate2() override;
		virtual void OnDraw() override;
	};
	//--------------------------------------------------------------------------------------------------------------
	// ���[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------------------------------
	class MailObject : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
		// �擾���ꂽ��true
		bool m_isArrive = false;
		float m_passageTime = 0.0f;
		float m_rot = 0.0f;
		float m_rotateSpeed = 2.0f;	// ���[���̉�]���x
	public:
		MailObject(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		MailObject(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~MailObject() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		void ArriveMail();
		void RotateMail();
		bool GetIsArrive() { return m_isArrive; }
		void SetIsArrive(bool isArrive) { m_isArrive = isArrive; }
	};
}
//end basecross