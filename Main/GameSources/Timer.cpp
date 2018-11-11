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
		// FPS‚ÌŽæ“¾
		wstring str(L"Žc‚èŽžŠÔ: ");
		str += Util::FloatToWStr(m_remainingTime);
		//•¶Žš—ñ‚ð‚Â‚¯‚é
		ptrString->SetText(str);
		ptrString->SetFont(ptrString->GetFontName(), 36.0f);
		ptrString->SetStartPosition(Point2D<float>(500, 0));
		ptrString->SetTextBlockWidth(200.0f);
	}
	void RemainingTimer::OnUpdate() {
		auto ptrString = GetComponent<StringSprite>();
		wstring str(L"Žc‚èŽžŠÔ: ");
		m_remainingTime += -App::GetApp()->GetElapsedTime();
		if (m_remainingTime >= 0.0f) {
			int t = (int)m_remainingTime;
			ptrString->SetText(str + Util::IntToWStr(t));
		}
		else {
			ptrString->SetText(L"”z‘—Ž¸”s");
		}
	}
}