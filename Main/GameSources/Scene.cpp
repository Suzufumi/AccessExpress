
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

	void Scene::CreateResources(){
		wstring dataDir;
		// media�f�B���N�g�����擾
		App::GetApp()->GetDataDirectory(dataDir);

		struct InitializedParam
		{
			wstring m_texName;
			wstring m_texKey;
		};
		const int TEXTURE_NUM = 9;
		InitializedParam textures[] = {
			{ L"Building1.png", L"Building1_TX" },
			{ L"Title.png", L"Title_TX"},
			{ L"StageSelect.png", L"StageSelect_TX"},
			{ L"ResultStage.png", L"ResultStage_TX"},
			{ L"cursor.png", L"cursor_TX"},
			{ L"Back.png", L"SKY_TX"},
			{ L"Number.png", L"Number_TX"},
			{ L"GameOver.png", L"GameOver_TX"},
			{ L"title2.png", L"TITLE_TX"},
			{ L"BackLight.png", L"BACK_L_TX"},
			{ L"BackDark.png", L"BACK_D_TX"},
			{L"TitleButoon.png", L"Title_BUTTON_TX"}
		};

		for (auto texture : textures)
		{
			wstring strTexture = dataDir + L"Textures\\" + texture.m_texName;
			App::GetApp()->RegisterTexture(texture.m_texKey, strTexture);

		}

		auto modelDir = dataDir + L"Model\\";

		//struct ModelParam
		//{
		//	wstring m_modelName;
		//	wstring m_modelKey;
		//	wstring m_texName;
		//	wstring m_texKey;
		//};

		//const int MODEL_SIZE = 3;
		//ModelParam models[3] = {
		//	{ L"PlayerChara.bmf", L"PLAYER_MODEL", L"UV_Player.png", L"PLAYER_TX"},
		//	{ }
		//}

		auto playerModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modelDir + L"Player\\", L"PlayerChara.bmf");
		//auto playerModelMesh = MeshResource::CreateStaticModelMesh(modelDir + L"Player\\", L"PlayerChara.bmf");
		App::GetApp()->RegisterResource(L"PLAYER_MODEL", playerModelMesh);
		auto modelTex = modelDir + L"Player\\" + L"UV_Player.png";
		App::GetApp()->RegisterTexture(L"PLAYER_TX", modelTex);
		modelTex = modelDir + L"Player\\" + L"UV_face.png";
		App::GetApp()->RegisterTexture(L"PLAYER_FACE_TX", modelTex);

		auto staticModel = MeshResource::CreateStaticModelMesh(modelDir, L"Building.bmf");
		App::GetApp()->RegisterResource(L"BUILDING_MODEL", staticModel);
		modelTex = modelDir + L"Building5.tga";
		App::GetApp()->RegisterTexture(L"Building2_TX", modelTex);
		staticModel = MeshResource::CreateStaticModelMesh(modelDir, L"RadioTower.bmf");
		App::GetApp()->RegisterResource(L"TOWER_MODEL", staticModel);
		modelTex = modelDir + L"RadioTower.png";
		App::GetApp()->RegisterTexture(L"TOWER_TX", modelTex);
		
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
		else if (event->m_MsgStr == L"ToGameOverStage") {
			ResetActiveStage<GameOverStage>();
		}
	}

}
//end basecross
