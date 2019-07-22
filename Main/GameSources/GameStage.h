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
		//�r���[�̍쐬
		void CreateViewLight();
		// �Q�[���}�l�[�W���[�̒l�̐ݒ�
		void ResetGameManagerParam();
		//�v���C���[�ƃv���C���[���
		void CreatePlayerRelationship();
		// �����̍쐬
		void CreateBill();
		// �d�g���̍쐬
		void CreateLinkObject();
		// �h���[���̍쐬
		void CreateDrone();
		// Mail�̍쐬
		void CreateMail();
		// �G�t�F�N�g�̍쐬
		void CreateEffect();
		template<typename T>
		void ImportXmlStatus(const wstring& nodeName);	// Xml�t�@�C������m�[�h����肾��

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

