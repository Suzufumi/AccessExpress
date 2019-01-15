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
	class ViewChainNum : public GameObject {
	protected:
		int m_places;	// �\����
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		ViewChainNum(const shared_ptr<Stage>& stagePtr);
		virtual ~ViewChainNum() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};
	//--------------------------------------------------------------------------------------
	//�X���[�̎c�莞�Ԃ�������I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SlowTimeUI : public Sprite {
		weak_ptr<Sprite> m_text;
		weak_ptr<Sprite> m_gageBase;
	public:
		SlowTimeUI(const shared_ptr<Stage>& stagePtr);
		virtual ~SlowTimeUI() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
	//--------------------------------------------------------------------------------------
	//�X�R�A�ɔ��ł����`�F�C���̐���
	//--------------------------------------------------------------------------------------
	class FlyingChain : public GameObject {
	protected:
		int m_places;	// �\����
		int m_chain;	// �\������`�F�C����
		float m_leap;
		Vec3 p0 = Vec3(780, -480, 0);
		Vec3 p1 = Vec3(1000,-500,0);
		Vec3 p2 = Vec3(1152, 0, 0);

		bool m_isFly;	//���ł���Ƃ�true
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		FlyingChain(const shared_ptr<Stage>& stagePtr);
		virtual ~FlyingChain() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void FlySet(int chain);
	};
	//--------------------------------------------------------------------------------------
	///	���C��
	//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		Vec3 m_StartObj;
		Vec3 m_EndObj;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	StartObj	�X�^�[�g�̃I�u�W�F�N�g
		@param[in]	EndObj	�G���h�̃I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, Vec3 StartObj,Vec3 EndObj);
		virtual ~ActionLine() {}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//���̃X�^�[�g�ƃG���h�I�u�W�F�N�g�̐ݒ�
		void ResetObject(Vec3 StartObj, Vec3 EndObj) {
			m_StartObj = StartObj;
			m_EndObj = EndObj;
		}
		void SetOnDraw(bool f);
	};
}