#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class EnergyGaugeUI : public Sprite{
		weak_ptr<Sprite> m_energyGauge[12];
	public:
		EnergyGaugeUI(const shared_ptr<Stage>& stagePtr);
		virtual ~EnergyGaugeUI();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
	//------------------------------------------------------------------------
	//�X�R�A��\������UI
	//------------------------------------------------------------------------
	class ScoreUI : public GameObject {
	protected:
		int m_displayScore;
		int m_places;	// �\����
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		ScoreUI(const shared_ptr<Stage>& stagePtr);
		virtual ~ScoreUI() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		//�J�E���g�A�b�v���I����Ă��邩�Ԃ�
		bool GetFinishCountUp() {
			if (GameManager::GetInstance().GetScore() == m_displayScore) {
				return true;
			}
			else {
				return false;
			}
		}
		//�����X�R�A�ƕ\���X�R�A�𓯂��ɂ���
		void CountSkip() {
			m_displayScore = GameManager::GetInstance().GetScore();
		}
	};
}