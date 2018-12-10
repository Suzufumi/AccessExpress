#pragma once
#include "stdafx.h"

namespace basecross
{
	class TpsCamera : public Camera
	{
	private:
		/*�v���C���[��Y�����]
		270�x����n�߂邱�Ƃ�X��+�������E�AZ��+���������ɂ���*/
		float m_AngleY;
		float m_AngleX;						//�v���C���[��X�����]
		float m_maxAngleSpeed;		//�J��������]����X�s�[�h
		float m_cameraHeight;			//�J�����̏�������
		float m_cameraDistance;			//�J�����̃v���C���[����̋���
		bool m_isLink;				//�����N��Ԃ̏������s��
		Vec3 p0, p1, p2;
	public:
		TpsCamera();
		virtual ~TpsCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		// �����œ����Ă����g�����X�t�H�[�����擾
		void SetTarget(const shared_ptr<Transform>& target);
		void SetBezier(weak_ptr<GameObject> playerPtr,Vec3 p2Player);
		void BezierMove(float leap,Vec3 playerPos);

		void SetOnLink(bool f) { m_isLink = f; };
		bool GetOnLink() { return m_isLink; };

		float GetCameraAngleX() const
		{ return m_AngleX; }
		float GetCameraAngleY() const 
		{ return m_AngleY; }
		float GetMaxAngleSpeed() const
		{ return m_maxAngleSpeed; }
		float GetCameraHeight() const
		{ return m_cameraHeight; }
		float GetCameraDistance() const
		{ return m_cameraDistance; }
	};
}