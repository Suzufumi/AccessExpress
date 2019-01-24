/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		int m_stageNum;
		 weak_ptr<XAudio2Manager> m_audioManager = App::GetApp()->GetXAudio2Manager();
		 weak_ptr<SoundItem> m_numMusic;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		
		void CreateResources();
		void CreateMusicResources();
		void MusicOnceStart(wstring key,float volume);
		shared_ptr<SoundItem> MusicRoopStart(wstring key, float volume);

		int GetStageNum() const { return m_stageNum; }
		void SetStageNum(int num) { m_stageNum = num; }
	};
}

//end basecross
