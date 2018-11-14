#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
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
		ActionLine(const shared_ptr<Stage>& StagePtr, Vec3 StartObj,
			Vec3 EndObj);
		virtual ~ActionLine() {}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//���̃X�^�[�g�I�u�W�F�N�g�̎擾
		Vec3 GetStartObj() const {
			return m_StartObj;
		}
		//���̃X�^�[�g�I�u�W�F�N�g�̐ݒ�
		void SetStartObj(Vec3 Obj) {
			m_StartObj = Obj;
		}
		//���̃G���h�I�u�W�F�N�g�̎擾
		Vec3 GetEndObj() const {
			return m_EndObj;
		}
		//���̃G���h�I�u�W�F�N�g�̐ݒ�
		void SetEndObj(Vec3 Obj) {
			m_EndObj = Obj;
		}
		//���̃X�^�[�g�ƃG���h�I�u�W�F�N�g�̐ݒ�
		void ResetObject(Vec3 StartObj, Vec3 EndObj) {
			m_StartObj = StartObj;
			m_EndObj = EndObj;
		}
	};

	class Ray : public GameObject {
		Vec3 m_positon;
		Vec3 m_direction;
		float m_length = 30.0f;

		weak_ptr<GameObject> m_player;
		Vec3 m_cameraPos;
		weak_ptr<ActionLine> m_ActionLine;
	public:
		Ray(const shared_ptr<Stage>& stage,weak_ptr<GameObject> player);
		virtual ~Ray() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}