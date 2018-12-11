#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//----------------------------------------------------------------------------
	//�Q�[�����Ǘ�����V���O���g��
	//----------------------------------------------------------------------------
	class GameManager final {
	private:
		GameManager() {} // �R���X�g���N�^�� private �ɒu���B
		GameManager(const GameManager&); // �R�s�[�R���X�g���N�^�� private �ɒu���A��`���Ȃ��B
		 ~GameManager() {} // �f�X�g���N�^�� private �ɒu���B

		 bool m_isSlow = false;	//�X���[��Ԃ�������true
	public:
		static GameManager& GetInstance() {
			static GameManager inst; // private �ȃR���X�g���N�^���Ăяo���B
			return inst;
		}
		//�X���[�t���O�̎擾
		const bool GetOnSlow() {
			return m_isSlow;
		}
		//�X���[�t���O�̐ݒ�
		void SetOnSlow(bool f) {
			m_isSlow = f;
		}
	};

}