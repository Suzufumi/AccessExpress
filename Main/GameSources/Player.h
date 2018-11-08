/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Player : public GameObject {
		float m_nowWalkSpeed = 5.0f;			//���݂̈ړ��̃X�s�[�h
		float m_standardWalkSpeed = 5.0f;		//��{�̈ړ��X�s�[�h
		/*�v���C���[��Y�����]
		270�x����n�߂邱�Ƃ�X��+�������E�AZ��+���������ɂ���*/
		float m_AngleY = Deg2Rad(270);
		float m_AngleX = 0;						//�v���C���[��X�����]
		float m_maxAngleSpeed = 3.141592f;		//�J��������]����X�s�[�h
		float m_cameraHeight = 3.0f;			//�J�����̏�������
		float m_cameraDistance = 10.0f;			//�J�����̃v���C���[����̋���
		bool m_isFall = false;					//Y�������̗͂������邩�ǂ���
		bool m_isFloa = false;					//���V���Ă�
		bool m_isHaveFile = false;

		CONTROLER_STATE m_pad;					//�p�b�h�̑S���

		//�R���X�g���N�^�Ŏ����Ă�������������
		Vec3 m_position;
		Quat m_quaternion;
		Vec3 m_scale;

		weak_ptr<RadioTowerHitJudgment> m_RadioTowerHitJudgment;
		weak_ptr<File> m_File;

		Vec3 m_padDir;									//���X�e�B�b�N�̌���������
		Vec3 m_forward;									//�J�����̕����𓥂܂����v���C���[�̌����Ă������
		Vec3 m_velocity;								//���݂�velocity�̒l������

	public:
		Player(const shared_ptr<Stage>& StagePtr, Vec3 pos, Quat quat, Vec3 sca);
		virtual ~Player() {};
		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V
		virtual void OnUpdate2() override; // ��X�V
		//�Փ˂����Ƃ�
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//�Փ˂��Ă���
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		//�Փ˂��������ꂽ�Ƃ�
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		void Floating();
		void Walk();
		void CameraRoll();
		//�q�I�u�W�F�N�g���Ă����Ă���d�g���Ƃ̓����蔻����v���C���[�̂ق��ł��F�m����
		void SetRadioTowerHitJudgment(weak_ptr<RadioTowerHitJudgment> hit) {
			m_RadioTowerHitJudgment = hit;
		}

		void DrawStrings();

	};
}
//end basecross

