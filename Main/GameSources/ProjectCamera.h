#pragma once
#include "stdafx.h"

namespace basecross
{
	class TpsCamera : public Camera
	{
	private:
		/*�v���C���[��Y�����]
		270�x����n�߂邱�Ƃ�X��+�������E�AZ��+���������ɂ���*/
		float m_angleY;
		float m_angleX;						//�v���C���[��X�����]
		float m_angeleXMax = 80.0f;				//�c�������Ǘ�����ϐ��̃}�b�N�X�l
		float m_maxAngleSpeed;		//�J��������]����X�s�[�h
		float m_cameraDistance;			//�J�����̃v���C���[����̋���
		float m_cameraLookUp;		//�J���������グ�鍂��
		bool m_isLink;				//�����N��Ԃ̏������s��
		Vec3 p0, p1, p2;			//�x�W�F�Ȑ��̐���_
		float m_leap;				//�␳�̌o��
		//��荞�ލۂɎg���p�x�̐��l
		float m_aroundStartAngleY;
		float m_aroundStartAngleX;
	public:
		TpsCamera();
		virtual ~TpsCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//
		void CameraControll();
		void CameraRoll(Vec3 sightPos);
		// �����œ����Ă����g�����X�t�H�[�����擾
		void SetTarget(const shared_ptr<Transform>& target);
		void SetBezier(weak_ptr<GameObject> playerPtr,Vec3 p2Player);
		void BezierMove(float leap,Vec3 playerPos);
		//��荞��
		void GoingAround(float AroundEndAngleX,float AroundEndAngleY);
		//���b�N�I�����Ă���Ώۂ�����ۂ̃J��������
		void RockonCameraMove(Vec3 sightPos,Vec3 linkPos);

		void SetOnLink(bool f) { m_isLink = f; };
		bool GetOnLink() { return m_isLink; };

		float GetCameraAngleX() const
		{ return m_angleX; }
		float GetCameraAngleY() const 
		{ return m_angleY; }
		void SetCameraAngleX(float anglex) {
			m_angleX = anglex;
		}
		void SetCameraAngleY(float angleY){
			m_angleY = angleY;
		}
		float GetMaxAngleSpeed() const
		{ return m_maxAngleSpeed; }
		float GetCameraDistance() const
		{ return m_cameraDistance; }
		float GetCameraLookUp() const
		{ return m_cameraLookUp; }
	};

	class TpsCameraJudgment : public GameObject {
	public:
		TpsCameraJudgment(const shared_ptr<Stage>& StagePtr);
		virtual ~TpsCameraJudgment() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//�Փ˂����Ƃ�
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//�Փ˂��������ꂽ�Ƃ�
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};
}