#include "stdafx.h"
#include "Project.h"

namespace basecross {
	RemainingTimer::RemainingTimer(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{
	}
	void RemainingTimer::OnCreate() {
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		// FPSの取得
		wstring str(L"残り時間: ");
		str += Util::FloatToWStr(m_remainingTime);
		//文字列をつける
		ptrString->SetText(str);
		ptrString->SetFont(ptrString->GetFontName(), 36.0f);
		ptrString->SetStartPosition(Point2D<float>(500, 0));
		ptrString->SetTextBlockWidth(200.0f);
	}
	void RemainingTimer::OnUpdate() {
		auto ptrString = GetComponent<StringSprite>();
		wstring str(L"残り時間: ");
		m_remainingTime += -App::GetApp()->GetElapsedTime();
		if (m_remainingTime >= 0.0f) {
			int t = (int)m_remainingTime;
			ptrString->SetText(str + Util::IntToWStr(t));
		}
		else {
			ptrString->SetText(L"配送失敗");
		}
	}

	//---------------------------------------------------------------------------------
	//時間制限のスプライト
	//---------------------------------------------------------------------------------
	RemainingTimerSprite::RemainingTimerSprite(const shared_ptr<Stage>& stage, int timer)
		:GameObject(stage), m_timer((float)timer)
	{
		m_places = 3;//3桁表示
	}

	void RemainingTimerSprite::OnCreate() {
		// 数字ごとの範囲を設定する
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
				GetStage(), L"Number2_TX", Vec2(640 , 128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThisでThisオブジェクトのshared_ptrを取ってこれる
			transComp->SetParent(GetThis<RemainingTimerSprite>());	// 基準点が画面の左上からTimerの場所になった
			transComp->SetScale(1.2f, 0.9f, 0.9f);
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 155));
			m_numbers.push_back(number);
		}
	}

	void RemainingTimerSprite::OnUpdate() {
		auto& gameManager = GameManager::GetInstance();
		if (App::GetApp()->GetScene<Scene>()->GetGameStart()) {
			if (gameManager.GetOnSlow()) {
				m_timer += -App::GetApp()->GetElapsedTime() / gameManager.GetSlowSpeed();
			}
			else {
				m_timer += -App::GetApp()->GetElapsedTime();
			}
		}
		if (m_timer < 0.0f) {
			m_timer = 0.0f;
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
		}
		int timer = (int)m_timer;

		for (int i = 0; i < m_places; i++) {
			int num = timer % 10;	// 一の位を抜き出す
			timer /= 10;			// 一の位を切り捨てる

			float start_x = m_numRects[num].left / 640.0f;
			float end_x = m_numRects[num].right / 640.0f;
			float start_y = m_numRects[num].top / 128.0f;
			float end_y = m_numRects[num].bottom / 128.0f;

			m_vertices[i][0].textureCoordinate = Vec2(start_x, start_y);
			m_vertices[i][1].textureCoordinate = Vec2(end_x, start_y);
			m_vertices[i][2].textureCoordinate = Vec2(start_x, end_y);
			m_vertices[i][3].textureCoordinate = Vec2(end_x, end_y);

			auto drawComp = m_numbers[i]->GetComponent<PTSpriteDraw>();
			drawComp->UpdateVertices(m_vertices[i]);	// 位置は変えずにポリゴンの中身だけ変える
		}
	}

	void RemainingTimerSprite::OnDraw() {
		for (auto number : m_numbers) {
			number->OnDraw();
		}
	}
}