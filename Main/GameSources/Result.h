#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
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
		weak_ptr<GameObject> m_player;
		weak_ptr<Sprite> m_clearSprite;
		weak_ptr<Sprite> m_mailText;
		weak_ptr<NumberSprite> m_mailNum;
		weak_ptr<Sprite> m_maxChainText;
		weak_ptr<NumberSprite> m_maxChainNum;
		weak_ptr<Sprite> m_scoreText;
		weak_ptr<ScoreUI> m_scoreNum;
		weak_ptr<Sprite> m_push;
		weak_ptr<Sprite> m_rankText;
		vector<weak_ptr<Sprite>> m_rankResult;
		vector<weak_ptr<Sprite>> m_rankFace;
		weak_ptr<SoundItem> m_audioPtr;
		weak_ptr<GameObject> m_antenna;
		weak_ptr<GameObject> m_mail;
		bool m_isCountUpEnd = false;
		Vec3 m_mailP0,m_maxChainP0,m_scoreP1;
		float m_time = 1.2f;
		bool m_isMailFly = false;
		enum progress {
			ANTENNA,
			MAIL_UP,
			START,
			ADD_SCORE,
			SCORE_COUNTUP,
			END
			//FADE
		};
		progress m_progress = progress::ANTENNA;
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void FadeProcess();
	};
}