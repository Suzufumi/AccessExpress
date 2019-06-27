#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {
		weak_ptr<GameObject> m_player;
		//ビューの作成
		void CreateViewLight();
		// プレイヤーの作成
		void CreateCharacter();
		// タイトルロゴ等の作成
		void CreateTitleResources();
		// 
		void FadePlay();
	public:
		virtual ~TitleStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}