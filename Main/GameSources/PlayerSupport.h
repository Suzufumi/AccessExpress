#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------------------
	//playerにくっつけるRayの届く範囲を見せるオブジェクト
	//------------------------------------------------------------------------------------------------------------
	class RayRangeViewObj : public GameObject {
		float m_range;
	public:
		RayRangeViewObj(const shared_ptr<Stage>& StagePtr);
		virtual ~RayRangeViewObj() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void GetRange(float range) { m_range = range; };
	};

	//--------------------------------------------------------------------------------------
	//ファイル
	//--------------------------------------------------------------------------------------
	class File : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
	public:
		File(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~File() {};
		virtual void OnCreate() override;
		void SetOnPlayer(weak_ptr<GameObject> obj);
		void LookFile();
		void UnLookFile();
	};

	//--------------------------------------------------------------------------------------
	//宛先のアドレスを持っているオブジェクト
	//--------------------------------------------------------------------------------------
	class AddlessCertificate : public OBBObject {
		//このオブジェクトが取得されたときにゴールを開錠するためにゴールを持つ
		weak_ptr<Goal> m_goal;
		bool m_isUnlockGoal = false;
	public:
		AddlessCertificate(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~AddlessCertificate() {};
		virtual void OnCreate() override;
		//ゴールの情報を与える
		void SetGoal(weak_ptr<Goal> goal) { m_goal = goal; }
		bool GetUnlockGoal() { return m_isUnlockGoal; }
		void FindGoal();
	};
	//--------------------------------------------------------------------------------------
	//リンクオブジェクトを捉えるための照準
	//--------------------------------------------------------------------------------------
	class SightingDevice : public GameObject {
		bool m_captureLink = false;
	public:
		SightingDevice(const shared_ptr<Stage>& stageptr);
		virtual ~SightingDevice() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void SetCaptureLink(bool f) { m_captureLink = f; }
		void ResetCaptureLink() { m_captureLink = false; }
	};
	//--------------------------------------------------------------------------------------
	//チェインの文字
	//--------------------------------------------------------------------------------------
	class ViewChainLetter : public Sprite {
		public:
			ViewChainLetter(const shared_ptr<Stage>& stagePtr);
			virtual ~ViewChainLetter() {};
			virtual void OnCreate() override;
			virtual void OnUpdate() override;
	};
	//--------------------------------------------------------------------------------------
	//チェインの数字
	//--------------------------------------------------------------------------------------
	class ViewChainNum : public GameObject {
	protected:
		int m_places;	// 表示桁
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		ViewChainNum(const shared_ptr<Stage>& stagePtr);
		virtual ~ViewChainNum() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};
	//--------------------------------------------------------------------------------------
	//スローの残り時間を見せるオブジェクト
	//--------------------------------------------------------------------------------------
	class SlowTimeUI : public Sprite {
		weak_ptr<Sprite> m_text;
		weak_ptr<Sprite> m_gageBase;
	public:
		SlowTimeUI(const shared_ptr<Stage>& stagePtr);
		virtual ~SlowTimeUI() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
	//--------------------------------------------------------------------------------------
	//スコアに飛んでいくチェインの数字
	//--------------------------------------------------------------------------------------
	class FlyingChain : public GameObject {
	protected:
		int m_places;	// 表示桁
		int m_chain;	// 表示するチェイン数
		float m_leap;
		Vec3 p0 = Vec3(780, -480, 0);
		Vec3 p1 = Vec3(1000,-500,0);
		Vec3 p2 = Vec3(1152, 0, 0);

		bool m_isFly;	//飛んでいるときtrue
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		FlyingChain(const shared_ptr<Stage>& stagePtr);
		virtual ~FlyingChain() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void FlySet(int chain);
	};
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
		ActionLine(const shared_ptr<Stage>& StagePtr, Vec3 StartObj,Vec3 EndObj);
		virtual ~ActionLine() {}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//線のスタートとエンドオブジェクトの設定
		void ResetObject(Vec3 StartObj, Vec3 EndObj) {
			m_StartObj = StartObj;
			m_EndObj = EndObj;
		}
		void SetOnDraw(bool f);
	};
}