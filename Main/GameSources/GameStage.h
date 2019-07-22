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
		// ゲームマネージャーの値の設定
		void ResetGameManagerParam();
		//プレイヤーとプレイヤー回り
		void CreatePlayerRelationship();
		// 建物の作成
		void CreateBill();
		// 電波塔の作成
		void CreateLinkObject();
		// ドローンの作成
		void CreateDrone();
		// Mailの作成
		void CreateMail();
		// エフェクトの作成
		void CreateEffect();
		template<typename T>
		void ImportXmlStatus(const wstring& nodeName);	// Xmlファイルからノードを取りだす

		float m_timeOver;
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		// Xmlファイルからデータを読み込む
		void LoadXmlParam(IXMLDOMNodePtr pNode, Vec3& pos, Quat& quat, Vec3& scale);
		// モデルの行列を設定する
		Mat4x4 SetModelMatrix(const Vec3& scale, const Vec3& rotOrigin, const Vec3& rot, const Vec3& pos);

	};


}
//end basecross

