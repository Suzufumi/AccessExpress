#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		const int RANK_SSS = 100000;
		const int RANK_SS = 60000;
		const int RANK_S = 30000;
		const int RANK_A = 15000;
		const int RANK_B = 5000;
		//�r���[�̍쐬
		void CreateViewLight();
		//���U���g�֘A���쐬����
		void CreateResult();
		void CreateCollectedMail();
		void CreateMaximumChain();
		void CreateScore();
		void CreateRank();
		void CreateMail(Vec3 position,Vec3 scale);
		// �X�R�A�����Z����(���[���ƃ`�F�C�������X�R�A�֔��)
		void AddScore();
		// �ŏI�I�ȃX�R�A�̃J�E���g
		void CountupScore();
		// �ŏI�X�R�A�\���ƃ����N�\��
		void ResultProcess(int resultScore);
		// �X�R�A�ɉ����������N�̕\��
		void ShowRank(int resultScore);
		void CreatePlayer();
		float m_leap = 0.0f;
		Vec3 playerP0, playerP1, playerP2;
		Vec3 mailP0, mailP1;
		//�v���C���[�̌�������
		Vec3 m_playerForward;
		// �v���C���[�̃|�C���^
		weak_ptr<GameObject> m_player;
		// �Q�[���N���AUI�̃|�C���^
		weak_ptr<Sprite> m_clearSprite;
		// ���[��UI�̃|�C���^
		weak_ptr<Sprite> m_mailText;
		// �W�߂����[�����̃|�C���^
		weak_ptr<NumberSprite> m_mailNum;
		// �ő�`�F�C��UI�̃|�C���^
		weak_ptr<Sprite> m_maxChainText;
		// �ő�`�F�C�����̃|�C���^
		weak_ptr<NumberSprite> m_maxChainNum;
		// �X�R�AUI�̃|�C���^
		weak_ptr<Sprite> m_scoreText;
		// �X�R�A�̃|�C���^
		weak_ptr<ScoreUI> m_scoreNum;
		// PushAnyButton�̃|�C���^
		weak_ptr<Sprite> m_push;
		// �����NUI�̃|�C���^
		weak_ptr<Sprite> m_rankText;
		// �����N��vector�z��
		vector<weak_ptr<Sprite>> m_rankResult;
		// �����N�ɉ������\���vector�z��
		vector<weak_ptr<Sprite>> m_rankFace;
		weak_ptr<SoundItem> m_audioPtr;
		weak_ptr<GameObject> m_antenna;
		weak_ptr<GameObject> m_mail;
		bool m_isCountUpEnd = false;
		Vec3 m_mailP0,m_maxChainP0,m_scoreP1;
		float m_time = 1.2f;
		bool m_isMailFly = false;
		weak_ptr<FadeInOut> m_fade;
		enum progress {
			ANTENNA,
			MAIL_UP,
			START,
			ADD_SCORE,
			SCORE_COUNTUP,
			END,
			FADE
		};
		progress m_progress = progress::ANTENNA;
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void FadeProcess();
	};
}