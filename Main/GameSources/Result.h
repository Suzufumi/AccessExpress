#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		const int RANK_S = 40000;
		const int RANK_A = 20000;
		const int RANK_B = 10000;
		//ビューの作成
		void CreateViewLight();
		void CreateCollectedMail();
		void CreateMaximumChain();
		void CreateScore();
		void CreateRank();
		// スコアを加算する(メールとチェイン数がスコアへ飛ぶ)
		void AddScore();
		// 最終的なスコアのカウント
		void CountupScore();
		// 最終スコア表示とランク表示
		void ResultProcess(int resultScore);
		// スコアに応じたランクの表示
		void ShowRank(int resultScore);
		void CreatePlayer();
		float m_leap = 0.0f;
		weak_ptr<GameObject> m_player;
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
		Vec3 m_mailP1,m_maxChainP1,m_scoreP2;
		enum progress {
			START,
			ADD_SCORE,
			SCORE_COUNTUP,
			END
			//FADE
		};
		progress m_progress = progress::START;
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void FadeProcess();
	};
}