#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleStage::OnCreate() {
		CreateViewLight();
	}
	void TitleStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
	}
	void TitleStage::CreateViewLight(){
		auto ptrView = CreateView<SingleView>();

	}
}