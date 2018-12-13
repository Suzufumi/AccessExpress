/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		// ステージのパスを保存する
		wstring m_stageXmlPath;
		//ビューの作成
		void CreateViewLight();
		//床の作成
		void CreateFloor();
		//プレイヤーとプレイヤー回り
		void CreatePlayerRelationship();

		void CreateBill();
		void CreateLinkObject();
		void CreateDrone();
		void CreateGoal();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


}
//end basecross

