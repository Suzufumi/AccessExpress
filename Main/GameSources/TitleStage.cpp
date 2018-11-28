#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleStage::OnCreate() {
		CreateViewLight();
		auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"Title_TX");
	}
	void TitleStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
		}
	}
	void TitleStage::CreateViewLight(){
		auto ptrView = CreateView<SingleView>();
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();

	}
}