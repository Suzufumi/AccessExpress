/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Player : public GameObject {
		unique_ptr<StateMachine<Player>>  m_StateMachine;	//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Player>> m_animStateMachine; // �A�j���[�V�����̃X�e�[�g�}�V��

		//�R���X�g���N�^�Ŏ����Ă�������������
		Vec3 m_position;
		Quat m_quaternion;
		Vec3 m_scale;

		const int BONUS_CHAIN = 5;	// �{�[�i�X��^����{��
		float m_nowFallSpeed = 8.0f;		//�����̃X�s�[�h
		float m_nowWalkSpeed = 8.0f;		//���݂̈ړ��̃X�s�[�h
		float m_cameraLookUp;			//�J���������グ�鍂��
		bool m_isFall = true;				//Y�������̗͂������邩�ǂ���
		bool m_isHit = false;				//�I�u�W�F�N�g�ɓ������Ă��邩�ǂ���
		float m_Lerp = 0.0f;				//�x�W�G�Ȑ��̂��߂̌o�ߎ��ԕϐ�
		Vec3 p0, p1,p2;						//�x�W�G�Ȑ��̂��߂̈ʒu���
		int m_chain = 0;		// �R���{�̂��߂̕ϐ�
		int m_chainTime = 0;	// �R���{�ƃR���{�̎��Ԃ�}�鎞��
		int m_comboChainLimit = 0; // �R���{���i��ł����ɂ�čX�V���鐧������
		float m_BezierSpeedLeap;		//�����ɉ�������ԏ����ւ̃X�s�[�h�␳
		float m_BezierSpeed = 30.0f;	//��ԍۂ̊�X�s�[�h
		bool m_islockon = false;		//���b�N�I�����Ă���ۂ�true
		float m_rayRange;				//���C�̓͂�����
		float m_rayRangeDefolt = 30.0f;	//���C�̓͂�������l
		bool m_isGoLink;				//��Ԃ��Ƃ��m�肵�Ă�����true
		float m_nesting = NULL;	//�����o���Ɏg���߂荞�݂̍ŏ��l
		int m_nestingMin;	//�����o���Ɏg���߂荞��ł������

		CONTROLER_STATE m_pad;					//�p�b�h�̑S���
		weak_ptr<RayRangeViewObj> m_RayRangeViewObj;
		weak_ptr<SightingDevice> m_SightingDevice;	//�Ə�
		weak_ptr<Drone> m_Drone;
		int m_DroneNo = NULL;
		int m_MailNum = NULL;
		weak_ptr<GameObject> m_LockOnObj;		//���b�N�I�����Ă���I�u�W�F�N�g
		weak_ptr<TpsCamera> m_tpsCamera; // �J�����̃C���X�^���X���󂯎��

		Vec3 m_padDir;							//���X�e�B�b�N�̌���������
		Vec3 m_forward;							//�J�����̕����𓥂܂����v���C���[�̌����Ă������

	public:
		enum Target {
			LINK,
			DRONE,
			MAIL
		};
		enum FaceState
		{
			Default = 3,
			Smile = 4,
			EyeClose = 6,
			Regret = 7
		};
		FaceState m_faceNum = Default;
		Target m_target;

		Player(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~Player() {};
		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V
		virtual void OnUpdate2() override; // ��X�V
		//�A�N�Z�T
		const unique_ptr<StateMachine<Player>>& GetStateMachine() {
			return m_StateMachine;
		}

		Target GetTarget() { return m_target; };

		const unique_ptr<StateMachine<Player>>& GetAnimStateMachine()
		{
			return m_animStateMachine;
		}

		//�Փ˂����Ƃ�
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//�Փ˂��Ă���
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		//�����鏈��
		void Fall();
		//���X�e�B�b�N�̒l�Ńv���C���[����]������
		void PlayerRoll();
		void Forword();

		//�Ə��̃I�u�W�F�N�g���Ǘ�����
		void SetSightingDevice(weak_ptr<SightingDevice> dev) {
			m_SightingDevice = dev;
			SightingDeviceDrawActive(true);
		}
		//�x�W�G�Ȑ��̏����|�W�V�����ݒ�
		void SetBezierPoint(Vec3 point);
		//�Ə��̈ʒu��ς���
		void SightingDeviceChangePosition();
		//Ray���΂�
		void RayShot();
		bool RayOBBHit(Vec3 origin, Vec3 originDir, shared_ptr<GameObject> obj);
		//Ray�ƃ����N�I�u�W�F�N�g���������Ă��邩�����鏈��
		void LinkRayCheck(Vec3 origin, Vec3 originDir);
		//Ray�ƃh���[�����������Ă��邩�����鏈��
		void DroneRayCheck(Vec3 origin, Vec3 originDir);
		//Ray�ƃ��[�����������Ă��邩
		void MailRayCheck(Vec3 origin, Vec3 originDir);
		//�x�W�G�Ȑ��Ń����N�֔�ԏ���
		void LinkGo();
		//�x�W�G�Ȑ��Ńh���[���֔�ԏ���
		void DroneGo();
		// �x�W�F�Ȑ��Ń��[���ɔ��
		void MailGo();
		//���b�N�I���Ώۂ�������A�����������
		void RockOn();
		// ���b�N�I������I�u�W�F�N�g��ݒ�
		void RockonObject(Vec3 origin, Vec3 originDir, wstring groupName, float correction);
		//�����o������
		void ExtrusionJudgment(const weak_ptr<GameObject>& Other);
		//�����o������
		void Extrusion();

		// �\��̐؂�ւ�
		void FaceChanger(FaceState beforeFace, FaceState afterFace);
		// �^�C���A�b�v���ɃA�j���[�V������������
		void ShowTimeUpAnime();
		//�^�C���A�b�v�A�j���[�V�������I������烊�U���g�ɂ���
		void TimeUpAnimeFinishToResult();
		//�Ə��̕\����؂�ւ���
		void SightingDeviceDrawActive(bool f) { 
			auto devi = m_SightingDevice.lock(); 
			devi->SetDrawActive(f);
		}
		Vec3 GetSightPos() { 
			return m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition(); 
		}

		// �`�F�C�������Z����
		void AddChain();
		// �`�F�C�����s���ɐF�X���Z�b�g����
		void ChainFailure();
		// ���݂̃R���{����Ԃ�
		int GetChain() const { return m_chain; }
		// �R���{�ƃR���{�̐������Ԃ����Z����
		void AddChainTime() { m_chainTime++; }
		// ���݂̃R���{�Ԃ̎��Ԃ�Ԃ�
		int GetChainTimeLim() const { return m_chainTime; }
		void ResetTimeLim() { m_chainTime = 0; }
		int GetComboChainLimit() const{ return m_comboChainLimit; }
		void SetComboChainLimit(int chainLim) { m_comboChainLimit = chainLim; }
		float GetRayRange() { return m_rayRange; }
		void ResetGoLink() { m_isGoLink = false; }
		Vec2 forwardAngle();
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
	class DataState : public ObjState<Player> {
		DataState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<DataState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	//class ClearState : public ObjState<Player>;
	//�p�r: �N���A�A�j���[�V�������
	//--------------------------------------------------------------------------------------
	class ClearState : public ObjState<Player> {
		ClearState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<ClearState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

}
//end basecross

