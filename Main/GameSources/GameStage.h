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
		//���̍쐬
		void CreateFloor();
		//�v���C���[�ƃv���C���[���
		void CreatePlayerRelationship();

		void CreateBill();
		void CreateLinkObject();
		void CreateDrone();
		void CreateGoal();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


}
//end basecross

