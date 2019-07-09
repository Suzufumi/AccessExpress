/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		// �X�e�[�W�̃p�X��ۑ�����
		wstring m_stageXmlPath;
		//weak_ptr<FadeInOut> m_fade;
		//�r���[�̍쐬
		void CreateViewLight();
		//���̍쐬
		void CreateFloor();
		//�v���C���[�ƃv���C���[���
		void CreatePlayerRelationship();

		void CreateBill();
		void CreateLinkObject();
		void CreateDrone();
		void CreateMail();
		void CreateEffect();

		float m_timeOver;
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		// Xml�t�@�C������f�[�^��ǂݍ���
		void LoadXmlParam(IXMLDOMNodePtr pNode, Vec3& pos, Quat& quat, Vec3& scale);
		// ���f���̍s���ݒ肷��
		Mat4x4 SetModelMatrix(const Vec3& scale, const Vec3& rotOrigin, const Vec3& rot, const Vec3& pos);

	};


}
//end basecross

