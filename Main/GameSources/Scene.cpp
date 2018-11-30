
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			CreateResources();
			//�N���A����F��ݒ�
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::CreateResources()
	{
		wstring dataDir;
		// media�f�B���N�g�����擾
		App::GetApp()->GetDataDirectory(dataDir);

		wstring strTexture = dataDir + L"Building1.png";
		App::GetApp()->RegisterTexture(L"Building1_TX", strTexture);
		strTexture = dataDir + L"Title.png";
		App::GetApp()->RegisterTexture(L"Title_TX", strTexture);
		strTexture = dataDir + L"StageSelect.png";
		App::GetApp()->RegisterTexture(L"StageSelect_TX", strTexture);
		strTexture = dataDir + L"ResultStage.png";
		App::GetApp()->RegisterTexture(L"ResultStage_TX", strTexture);

		auto modelDir = dataDir + L"Model\\";
		auto playerModelMesh = MultiMeshResource::CreateStaticModelMultiMesh(modelDir + L"Player\\", L"PlayerChara.bmf");
		//auto playerModelMesh = MeshResource::CreateStaticModelMesh(modelDir + L"Player\\", L"PlayerChara.bmf");
		App::GetApp()->RegisterResource(L"PLAYER_MODEL", playerModelMesh);
		auto modelTex = modelDir + L"Player\\" + L"UV_Player.png";
		App::GetApp()->RegisterTexture(L"PLAYER_TX", modelTex);
		modelTex = modelDir + L"Player\\" + L"UV_face.png";
		App::GetApp()->RegisterTexture(L"PLAYER_FACE_TX", modelTex);

		auto buildingModel = MeshResource::CreateStaticModelMesh(modelDir, L"Building.bmf");
		App::GetApp()->RegisterResource(L"BUILDING_MODEL", buildingModel);
		modelTex = modelDir + L"Building5.tga";
		App::GetApp()->RegisterTexture(L"Building2_TX", modelTex);
		
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToTitleStage") {
			ResetActiveStage<TitleStage>();
		}
		else if (event->m_MsgStr == L"ToStageSelect") {
			ResetActiveStage<StageSelect>();
		}
		else if (event->m_MsgStr == L"ToResultStage") {
			ResetActiveStage<ResultStage>();
		}
	}

}
//end basecross
