
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			CreateResources();
			CreateMusicResources();
			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::CreateResources(){
		wstring dataDir;
		// mediaディレクトリを取得
		App::GetApp()->GetDataDirectory(dataDir);

		struct InitializedParam
		{
			wstring m_texName;
			wstring m_texKey;
		};
		InitializedParam textures[] = {
			//{ L"Building1.png", L"Building1_TX" },
			{ L"Logo.png", L"TITLE_TX"},
			{ L"StageSelect.png", L"StageSelect_TX"},
			{ L"kannryou.png", L"ResultStage_TX"},
			{ L"cursor.png", L"cursor_TX"},
			{ L"Back.png", L"SKY_TX"},
			{ L"GameOver.png", L"GameOver_TX"},
			{ L"BackLight.png", L"BACK_L_TX"},
			{ L"PressButton.png", L"Title_BUTTON_TX"}, 
			{ L"fade.png", L"FADE_TX"},	
			{ L"num.png", L"Number_TX"},
			{ L"Plane.png", L"SkyBox_Plane_TX"}, 
			{ L"Sky.png", L"SkyBox_TX"},
			{ L"BackGr.png", L"SkyBox_Back_TX"},
			{ L"RayRange.png",L"RayRange_TX"},
			{ L"MaxChainText.png",L"MaxChainText_TX"},
			{ L"scoreResult.png",L"ScoreText_TX"},
			{ L"Time.png", L"TIME_TX"},
			{ L"score.png", L"SCORE_TX"},
			{ L"CollectedMail.png",L"CollectedMail_TX"},
			{ L"mail.png",L"mail_TX"},
			{ L"kakeru.png",L"kakeru_TX"},
			{ L"ClearBack.png",L"ClearBack_TX"},
			{ L"GetEffect.png", L"GetEffect_TX"},
			{ L"spark2.png", L"SPARK_TX"},
			{L"rankText.png", L"RANK_TEXT_TX"},
			{L"rankS.png", L"RANK_S_TX"},
			{L"rankA.png", L"RANK_A_TX"},
			{L"rankB.png", L"RANK_B_TX"},
			{L"rankC.png", L"RANK_C_TX"},
			{L"result_S.png", L"RESULT_S_TX"},
			{L"result_A.png", L"RESULT_A_TX"},
			{L"result_B.png", L"RESULT_B_TX"},
			{L"result_C.png", L"RESULT_C_TX"},
			{L"TIMEUP.png", L"TIMEUP_TX"},
			{L"Balloon.png", L"BALLOON_TX"},
			{ L"chain_gauge.png",L"chain_gauge_TX"},
			{ L"chainbar.png",L"chainbar_TX"},
			{ L"ChainGauge.png",L"ChainGauge_TX"},
			{ L"ChainLetter.png", L"ChainLetter_TX"},
			{ L"sippai.png", L"sippai_TX"},
			{ L"normal.png", L"NORMAL_TX"},
			{ L"hard.png", L"HARD_TX"},
			{ L"tutorialback.png", L"TUTORIAL_TX"},
			{ L"tutorial1.png", L"TUTO1_TX"},
			{ L"tutorial2.png", L"TUTO2_TX"},
			{ L"tutorial3.png", L"TUTO3_TX"},
			{ L"tutorial4.png", L"TUTO4_TX"},
			{ L"tutorial5.png", L"TUTO5_TX"}
		};

		for (auto texture : textures)
		{
			wstring strTexture = dataDir + L"Textures\\" + texture.m_texName;
			App::GetApp()->RegisterTexture(texture.m_texKey, strTexture);

		}

		auto modelDir = dataDir + L"Model\\";

		auto playerModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modelDir + L"Player\\", L"PlayerChara.bmf");
		//auto playerModelMesh = MeshResource::CreateStaticModelMesh(modelDir + L"Player\\", L"PlayerChara.bmf");
		App::GetApp()->RegisterResource(L"PLAYER_MODEL", playerModelMesh);
		auto modelTex = modelDir + L"Player\\" + L"UV_Player.png";
		App::GetApp()->RegisterTexture(L"PLAYER_TX", modelTex);
		modelTex = modelDir + L"Player\\" + L"UV_face.png";
		App::GetApp()->RegisterTexture(L"PLAYER_FACE_TX", modelTex);

		auto staticModel = MeshResource::CreateStaticModelMesh(modelDir, L"Building.bmf");
		App::GetApp()->RegisterResource(L"BUILDING_MODEL", staticModel);
		modelTex = modelDir + L"Tx_Building1.tga";
		App::GetApp()->RegisterTexture(L"Building_TX", modelTex);
		staticModel = MeshResource::CreateStaticModelMesh(modelDir, L"RadioTower.bmf");
		App::GetApp()->RegisterResource(L"TOWER_MODEL", staticModel);
		modelTex = modelDir + L"RadioTower.tga";
		App::GetApp()->RegisterTexture(L"TOWER_TX", modelTex);

		staticModel = MeshResource::CreateStaticModelMesh(modelDir + L"Enemy\\", L"Drone.bmf");
		App::GetApp()->RegisterResource(L"ENEMY_MODEL", staticModel);
		modelTex = modelDir + L"Enemy\\" + L"ufo.png";
		App::GetApp()->RegisterTexture(L"ENEMY_TX", modelTex);

		staticModel = MeshResource::CreateStaticModelMesh(modelDir, L"CheckPoint.bmf");
		App::GetApp()->RegisterResource(L"CHECKPOINT_MODEL", staticModel);
		staticModel = MeshResource::CreateStaticModelMesh(modelDir, L"mail.bmf");
		App::GetApp()->RegisterResource(L"MAIL_MODEL", staticModel);
		modelTex = modelDir + L"Mail.png";
		App::GetApp()->RegisterTexture(L"MAIL_TX", modelTex);
	}
	//----------------------------------------------------------------------------------------
	//音を登録する
	//----------------------------------------------------------------------------------------
	void Scene::CreateMusicResources() {
		wstring dataDir;
		// mediaディレクトリを取得
		App::GetApp()->GetDataDirectory(dataDir);

		//構造体作成
		struct InitializedParam{
			wstring m_musicName;
			wstring m_musicKey;
		};
		//ファイル名とキーの設定
		InitializedParam musics[] = {
			//BGM
			{L"tw061.wav",L"tw061_mus"},
			{L"tw015.wav",L"tw015_mus"},
			{L"tw014b.wav",L"tw014b_mus"},
			//SE
			{L"bomb.wav",L"bomb_se"},
			{L"jumpAnime.wav", L"Jump_SE"},
			{L"decision02.wav", L"Decision_SE"},
			{L"countUpSE.wav", L"Countup_SE"},
			{L"GetMail_SE.wav", L"Mail_SE"},
			{L"TimeUp.wav", L"TIMEUP_SE"}
		};
		//プロジェクトに登録
		for (auto music : musics){
			wstring strTexture = dataDir + L"Musics\\" + music.m_musicName;
			App::GetApp()->RegisterWav(music.m_musicKey, strTexture);
		}
	}
	//----------------------------------------------------------------------------------------
	//音を一回鳴らす
	//----------------------------------------------------------------------------------------
	shared_ptr<SoundItem> Scene::MusicOnceStart(wstring key,float volume) {
		auto audioMana = m_audioManager.lock();
		return audioMana->Start(key, XAUDIO2_NO_LOOP_REGION, volume);
	}

	//----------------------------------------------------------------------------------------
	//音をループで流す
	//----------------------------------------------------------------------------------------
	shared_ptr<SoundItem> Scene::MusicRoopStart(wstring key, float volume) {
		auto audioMana = m_audioManager.lock();
		auto si = audioMana->Start(key, XAUDIO2_LOOP_INFINITE, volume);
		return si;
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		auto audioMana = m_audioManager.lock();
		audioMana->Stop(m_numMusic.lock());
		if (event->m_MsgStr == L"ToGameStage") {
			m_numMusic = MusicRoopStart(L"tw015_mus", 0.7f);

			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToTitleStage") {
			m_numMusic = MusicRoopStart(L"tw014b_mus", 0.6f);
			
			ResetActiveStage<TitleStage>();
		}
		else if (event->m_MsgStr == L"ToStageSelect") {
			m_numMusic = MusicRoopStart(L"tw061_mus", 0.6f);

			ResetActiveStage<StageSelect>();
		}
		else if (event->m_MsgStr == L"ToResultStage") {
			m_numMusic = MusicRoopStart(L"tw061_mus", 0.6f);

			ResetActiveStage<ResultStage>();
		}
		else if (event->m_MsgStr == L"ToGameOverStage") {
			ResetActiveStage<GameOverStage>();
		}
	}

}
//end basecross
