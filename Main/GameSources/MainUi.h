#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//------------------------------------------------------------------------
	//メール数を表示するUI
	//------------------------------------------------------------------------
	class MailNumUI : public Sprite {
		weak_ptr<NumberSprite> m_mailNum;
	public:
		MailNumUI(const shared_ptr<Stage>& stagePtr);
		virtual ~MailNumUI() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
	//------------------------------------------------------------------------
	//スコアを表示するUI
	//------------------------------------------------------------------------
	class ScoreUI : public GameObject {
	protected:
		int m_displayScore;
		int m_places;	// 表示桁
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		ScoreUI(const shared_ptr<Stage>& stagePtr);
		virtual ~ScoreUI() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		//カウントアップの処理
		void CountUp();
		//画面に出すスコアの値を設定する
		void ViewScore();
		//カウントアップが終わっているか返す
		bool GetFinishCountUp() {
			if (GameManager::GetInstance().GetScore() == m_displayScore) {
				return true;
			}
			else {
				return false;
			}
		}
		//内部スコアと表示スコアを同じにする
		void CountSkip() {
			m_displayScore = GameManager::GetInstance().GetScore();
		}
	};
}