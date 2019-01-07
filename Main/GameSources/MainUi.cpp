#include "stdafx.h"
#include "Project.h"

namespace basecross{
	EnergyGaugeUI::EnergyGaugeUI(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"UI_BASE_TX", Vec2(256, 256))
	{

	}

	EnergyGaugeUI::~EnergyGaugeUI()
	{}

	void EnergyGaugeUI::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(133,123));
		struct GaugeParam {
			const wstring key;
			Vec2 size;
			Vec2 pos;
		};
		Vec2 size = Vec2(28,28);
		GaugeParam gauges[] = {
			{L"RED_GAUGE_TX", size,Vec2(117,191)},
			{L"RED_GAUGE_TX", size,Vec2(117,160)},
			{L"RED_GAUGE_TX", size,Vec2(117,129)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,192)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,161)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,130)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,99)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,192)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,161)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,130)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,99)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,68)}
		};
		int count = 0;
		for (auto gauge : gauges) {
			m_energyGauge[count] = GetStage()->AddGameObject<Sprite>(gauge.key, gauge.size);
			m_energyGauge[count].lock()->SetPosition(gauge.pos);
			m_energyGauge[count].lock()->SetDrawLayer(1);
			count++;
		}
	}
	void EnergyGaugeUI::OnUpdate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		//�ԗ̈�
		for (int i = 0; i < 3; i++) {
			if (player->GetEnergy() < (3 + i * 5)) {
				m_energyGauge[i].lock()->SetDrawActive(false);
			}
			else {
				m_energyGauge[i].lock()->SetDrawActive(true);
			}
		}
		//���F�̈�
		for (int i = 0; i < 4; i++) {
			if (player->GetEnergy() < (18 + i * 8)) {
				m_energyGauge[3 + i].lock()->SetDrawActive(false);
			}
			else {
				m_energyGauge[3 + i].lock()->SetDrawActive(true);
			}
		}
		//�Η̈�
		for (int i = 0; i < 5; i++) {
			if (player->GetEnergy() < (50 + i * 10)) {
				m_energyGauge[7 + i].lock()->SetDrawActive(false);
			}
			else {
				m_energyGauge[7 + i].lock()->SetDrawActive(true);
			}
		}
	}
	//------------------------------------------------------------------------------------
	//�R���X�g���N�^
	//------------------------------------------------------------------------------------
	ScoreUI::ScoreUI(const shared_ptr<Stage>& stagePtr)
		: GameObject(stagePtr)
	{
		m_nowScore = 0;
		m_displayScore = 0;
		m_places = 3;
	}
	//------------------------------------------------------------------------------------
	//�\�z
	//------------------------------------------------------------------------------------
	void ScoreUI::OnCreate() {
		// �������Ƃ͈̔͂�ݒ肷��
		for (int i = 0; i < 10; i++) {
			m_numRects.push_back({
				static_cast<float>(64 * i),			// left
				static_cast<float>(0),				// top
				static_cast<float>(64 * (i + 1)),	// right
				static_cast<float>(128)				// bottom
				});
		}

		float start_x = m_numRects[5].left / 640.0f;
		float end_x = m_numRects[5].right / 640.0f;
		float start_y = m_numRects[5].top / 128.0f;
		float end_y = m_numRects[5].bottom / 128.0f;

		for (int i = 0; i < m_places; i++) {
			m_vertices.push_back({
				{ Vec3(0.0f,128.0f,0.0f),Vec2(start_x,start_y) },
				{ Vec3(64.0f,128.0f,0.0f),Vec2(end_x,start_y) },
				{ Vec3(0.0f,0.0f,0.0f),Vec2(start_x,end_y) },
				{ Vec3(64.0f,0.0f,0.0f),Vec2(end_x,end_y) }
				});
		}

		for (int i = 0; i < m_places; i++) {
			auto number = ObjectFactory::Create<Sprite>(
				GetStage(), L"Number_TX", Vec2(640, 128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThis��This�I�u�W�F�N�g��shared_ptr������Ă����
			transComp->SetParent(GetThis<ScoreUI>());	// ��_����ʂ̍��ォ��ScoreUI�̏ꏊ�ɂȂ���
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			m_numbers.push_back(number);
		}
	}

	void ScoreUI::OnUpdate() {
		auto& gameManager = GameManager::GetInstance();
		
		int score = m_displayScore;
		for (int i = 0; i < m_places; i++) {
			int num = score % 10;	// ��̈ʂ𔲂��o��
			score /= 10;			// ��̈ʂ�؂�̂Ă�

			float start_x = m_numRects[num].left / 640.0f;
			float end_x = m_numRects[num].right / 640.0f;
			float start_y = m_numRects[num].top / 128.0f;
			float end_y = m_numRects[num].bottom / 128.0f;

			m_vertices[i][0].textureCoordinate = Vec2(start_x, start_y);
			m_vertices[i][1].textureCoordinate = Vec2(end_x, start_y);
			m_vertices[i][2].textureCoordinate = Vec2(start_x, end_y);
			m_vertices[i][3].textureCoordinate = Vec2(end_x, end_y);

			auto drawComp = m_numbers[i]->GetComponent<PTSpriteDraw>();
			drawComp->UpdateVertices(m_vertices[i]);	// �ʒu�͕ς����Ƀ|���S���̒��g�����ς���
		}
	}

	void ScoreUI::OnDraw() {
		for (auto number : m_numbers) {
			number->OnDraw();
		}
	}
}