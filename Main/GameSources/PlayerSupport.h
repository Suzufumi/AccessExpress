#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------------------
	//player�ɂ�������d�g���Ƃ̏Փ˔�����s�������蔻��I�u�W�F�N�g
	//------------------------------------------------------------------------------------------------------------
	class RadioTowerHitJudgment : public GameObject {
		float m_Acceleration;
		float m_length;
		weak_ptr<GameObject> m_player;
	public:
		RadioTowerHitJudgment(const shared_ptr<Stage>& StagePtr, weak_ptr<GameObject> player);
		virtual ~RadioTowerHitJudgment() {};
		virtual void OnCreate() override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		void Rset() {
			m_length = 5.0f;
			m_Acceleration = 6.0f;
		}
		float GetAcceleration() { return m_Acceleration; }
	};

	//--------------------------------------------------------------------------------------
	//�t�@�C��
	//--------------------------------------------------------------------------------------
	class File : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
	public:
		File(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~File() {};
		virtual void OnCreate() override;
		void SetOnPlayer(weak_ptr<GameObject> obj);
		void LookFile();
		void UnLookFile();
	};

	//--------------------------------------------------------------------------------------
	//����̃A�h���X�������Ă���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class AddlessCertificate : public OBBObject {
		//���̃I�u�W�F�N�g���擾���ꂽ�Ƃ��ɃS�[�����J�����邽�߂ɃS�[��������
		weak_ptr<Goal> m_goal;
		bool m_isUnlockGoal = false;
	public:
		AddlessCertificate(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~AddlessCertificate() {};
		virtual void OnCreate() override;
		//�S�[���̏���^����
		void SetGoal(weak_ptr<Goal> goal) { m_goal = goal; }
		bool GetUnlockGoal() { return m_isUnlockGoal; }
		void FindGoal();
	};
	//--------------------------------------------------------------------------------------
	//�����N�I�u�W�F�N�g�𑨂��邽�߂̏Ə�
	//--------------------------------------------------------------------------------------
	class SightingDevice : public GameObject {
		bool m_captureLink = false;
	public:
		SightingDevice(const shared_ptr<Stage>& stageptr);
		virtual ~SightingDevice() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void SetCaptureLink(bool f) { m_captureLink = f; }
		void ResetCaptureLink() { m_captureLink = false; }
	};
	//--------------------------------------------------------------------------------------
	//�`�F�C���̕���
	//--------------------------------------------------------------------------------------
	class ViewChainLetter : public Sprite {
		public:
			ViewChainLetter(const shared_ptr<Stage>& stagePtr);
			virtual ~ViewChainLetter() {};
			virtual void OnCreate() override;
			virtual void OnUpdate() override;
	};
	//--------------------------------------------------------------------------------------
	//�`�F�C���̐���
	//--------------------------------------------------------------------------------------
	class ViewChainNum : public Sprite {
		shared_ptr<MeshResource> m_SquareMeshResource;

	public:
		ViewChainNum(const shared_ptr<Stage>& stagePtr);
		virtual ~ViewChainNum() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}