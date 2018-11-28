/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Player : public GameObject {
		unique_ptr< StateMachine<Player> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		wstring state = L"walk";

		float m_nowFallSpeed = 8.0f;
		float m_nowWalkSpeed = 10.0f;			//���݂̈ړ��̃X�s�[�h
		float m_standardWalkSpeed = 5.0f;		//��{�̈ړ��X�s�[�h
		float m_angleX;
		float m_angleY;
		float m_maxAngleSpeed;		//�J��������]����X�s�[�h
		float m_cameraHeight;			//�J�����̏�������
		float m_cameraDistance;			//�J�����̃v���C���[����̋���
		bool m_isFall = false;					//Y�������̗͂������邩�ǂ���
		bool m_isHaveFile = false;
		bool m_isHit = false;					//�I�u�W�F�N�g�ɓ������Ă��邩�ǂ���
		float m_Lerp = 0.0f;					//�x�W�G�Ȑ��̂��߂̌o�ߎ��ԕϐ�
		Vec3 p0, p1,p2;							//�x�W�G�Ȑ��̂��߂̈ʒu���
		float m_energy = 100.0f;
		float m_changeEnergy = 1.0f;
		float m_maxEnergy = 100.0f;

		CONTROLER_STATE m_pad;					//�p�b�h�̑S���

		//�R���X�g���N�^�Ŏ����Ă�������������
		Vec3 m_position;
		Quat m_quaternion;
		Vec3 m_scale;

		weak_ptr<RadioTowerHitJudgment> m_RadioTowerHitJudgment;
		weak_ptr<File> m_File;
		weak_ptr<SightingDevice> m_SightingDivice;

		Vec3 m_padDir;							//���X�e�B�b�N�̌���������
		Vec3 m_forward;							//�J�����̕����𓥂܂����v���C���[�̌����Ă������

		weak_ptr<TpsCamera> m_tpsCamera; // �J�����̃C���X�^���X���󂯎��

	public:
		Player(const shared_ptr<Stage>& StagePtr, Vec3 pos, Quat quat, Vec3 sca);
		virtual ~Player() {};
		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V
		virtual void OnUpdate2() override; // ��X�V
		//�A�N�Z�T
		const unique_ptr<StateMachine<Player>>& GetStateMachine() {
			return m_StateMachine;
		}

		//�Փ˂����Ƃ�
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//�Փ˂��Ă���
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		//�Փ˂��������ꂽ�Ƃ�
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		//�����o������
		void Extrusion(const weak_ptr<GameObject>& Other);

		//XZ���ʂ̈ړ�����
		void Walk();
		//�����鏈��
		void Fall();

		void CameraRoll();
		void CameraControll();

		//�q�I�u�W�F�N�g���Ă����Ă���d�g���Ƃ̓����蔻����v���C���[�̂ق��ł��F�m����
		void SetRadioTowerHitJudgment(weak_ptr<RadioTowerHitJudgment> hit) {
			m_RadioTowerHitJudgment = hit;
		}
		//�Ə��̃I�u�W�F�N�g���Ǘ�����
		void SetSightingDivice(weak_ptr<SightingDevice> dev) {
			m_SightingDivice = dev;
		}
		void SightingDiviceChangePosition();
		//�x�W�G�Ȑ��Ŕ�ԏ���
		void LinkGo();
		//�x�W�G�Ȑ��̏����|�W�V�����ݒ�
		void SetBezierPoint(Vec3 point);
		//Ray�ƃ����N�I�u�W�F�N�g���������Ă��邩�����鏈��
		void RayHitLink();
		//A�{�^���������ꂽ
		bool CheckAButton();

		//�G�l���M�[������悤�ɂ���
		void ChengeEnergyMai() { m_changeEnergy = -1.0f; }
		//�G�l���M�[��������悤�ɂ���
		void ChengeEnergyPur() { m_changeEnergy = 1.0f; }
		//�G�l���M�[�̗ʂ�Ԃ�
		float GetEnergy() { return m_energy; }

		void DrawStrings();
	};

	//--------------------------------------------------------------------------------------
	//class WalkState : public ObjState<Player>;
	//�p�r: �����Ă�����
	//--------------------------------------------------------------------------------------
	class WalkState : public ObjState<Player> {
		WalkState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<WalkState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//class LinkState : public ObjState<Player>;
	//�p�r: �����N�����ł�����
	//--------------------------------------------------------------------------------------
	class LinkState : public ObjState<Player> {
		LinkState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<LinkState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//class DateState : public ObjState<Player>;
	//�p�r: �f�[�^�̏��
	//--------------------------------------------------------------------------------------
	class DateState : public ObjState<Player> {
		DateState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<DateState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

}
//end basecross

