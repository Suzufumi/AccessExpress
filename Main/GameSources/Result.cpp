#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ResultStage::OnCreate() {
		CreateViewLight();
		CreateRemainingTime();
		CreateMaximumChain();
		CreateScore();
		auto clearSprite = AddGameObject<Sprite>(L"ResultStage_TX", Vec2(800.0f, 120.0f));
		clearSprite->SetPosition(Vec2(640.0f, 100.0f));
		auto push = AddGameObject<Sprite>(L"Title_BUTTON_TX", Vec2(1000, 100));
		push->SetPosition(Vec2(640.0f, 700.0f));
		//auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		//obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"ResultStage_TX");
	}
	void ResultStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		switch (m_progress){
		case progress::START:
			if (CntlVec[0].wPressedButtons) {
				m_progress = progress::ADD_SCORE;
			}
			break;
		case progress::ADD_SCORE:
			AddScore();
			break;
		case progress::END:
			if (CntlVec[0].wPressedButtons) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			}
		default:
			break;
		}
	}
	void ResultStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}
	///-----------------------------------------------------------------------------
	//�S�[�����̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateRemainingTime() {
		auto timeSprite = AddGameObject<Sprite>(L"RemainingTimeText_TX", Vec2(410, 205));
		timeSprite->SetPosition(Vec2(320.0f, 250.0f));
	};
	///-----------------------------------------------------------------------------
	//�}�b�N�X�`�F�C���̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateMaximumChain() {
		//����
		auto maxChainText = AddGameObject<Sprite>(L"MaxChainText_TX", Vec2(461, 230));
		maxChainText->SetPosition(Vec2(320.0f, 400.0f));
		//����
		m_maxChainNum = AddGameObject<NumberSprite>(2, 22);
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(880, -300, 0);
		m_maxChainP1 = m_maxChainNum.lock()->GetComponent<Transform>()->GetPosition();
	};
	///-----------------------------------------------------------------------------
	//�X�R�A�̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateScore() {
		//����
		auto scoreText = AddGameObject<Sprite>(L"ScoreText_TX", Vec2(410, 205));
		scoreText->SetPosition(Vec2(320.0f, 550.0f));
		auto scoreNum = AddGameObject<ScoreUI>();
		scoreNum->GetComponent<Transform>()->SetPosition(750, -450, 0);
		m_scoreP2 = scoreNum->GetComponent<Transform>()->GetPosition() + Vec3(50.0f,0,0);
	};
	///-----------------------------------------------------------------------------
	//�X�R�A�����o��ɑ��₷
	///-----------------------------------------------------------------------------
	void ResultStage::AddScore() {
		m_leap += App::GetApp()->GetElapsedTime();
		if (m_leap >= 1.0f) {
			m_leap = 1.0f;
			GameManager::GetInstance().AddScore((6 * 20)*10);
			//�`�F�b�N�|�C���g�p�[�t�F�N�g�{�[�i�X�������

			m_progress = progress::END;
			m_maxChainNum.lock()->SetDrawActive(false);
		}
		Vec3 chainPos = (1 - m_leap) * m_maxChainP1 + m_leap * m_scoreP2;
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(chainPos);
	}
}