#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void StageSelect::OnCreate() {
		CreateViewLight();
		auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"StageSelect_TX");
	}
	void StageSelect::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
	}
	void StageSelect::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}
}