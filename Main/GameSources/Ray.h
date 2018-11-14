#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
///	ライン
//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		Vec3 m_StartObj;
		Vec3 m_EndObj;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	StartObj	スタートのオブジェクト
		@param[in]	EndObj	エンドのオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, Vec3 StartObj,
			Vec3 EndObj);
		virtual ~ActionLine() {}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//線のスタートオブジェクトの取得
		Vec3 GetStartObj() const {
			return m_StartObj;
		}
		//線のスタートオブジェクトの設定
		void SetStartObj(Vec3 Obj) {
			m_StartObj = Obj;
		}
		//線のエンドオブジェクトの取得
		Vec3 GetEndObj() const {
			return m_EndObj;
		}
		//線のエンドオブジェクトの設定
		void SetEndObj(Vec3 Obj) {
			m_EndObj = Obj;
		}
		//線のスタートとエンドオブジェクトの設定
		void ResetObject(Vec3 StartObj, Vec3 EndObj) {
			m_StartObj = StartObj;
			m_EndObj = EndObj;
		}
	};

	class Ray : public GameObject {
		Vec3 m_positon;
		Vec3 m_direction;
		float m_length = 30.0f;

		weak_ptr<GameObject> m_player;
		Vec3 m_cameraPos;
		weak_ptr<ActionLine> m_ActionLine;
	public:
		Ray(const shared_ptr<Stage>& stage,weak_ptr<GameObject> player);
		virtual ~Ray() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}