#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void StageSelect::OnCreate() {
		CreateViewLight();
	}
	void StageSelect::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
	}
	void StageSelect::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();

	}
}