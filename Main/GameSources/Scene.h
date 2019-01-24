/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		int m_stageNum;
		 weak_ptr<XAudio2Manager> m_audioManager = App::GetApp()->GetXAudio2Manager();
		 weak_ptr<SoundItem> m_numMusic;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
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
