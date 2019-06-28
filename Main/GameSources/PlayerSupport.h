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
	//チェインの文字とバー
	//--------------------------------------------------------------------------------------
	class ViewChainLetter : public Sprite {
		weak_ptr<Sprite> m_gage;
		weak_ptr<Sprite> m_gageFram;
		weak_ptr<GameObject> m_gageParent;
		float m_gageDefaltLength;
		float m_gageBounsLength = 0.1f;
		public:
			ViewChainLetter(const shared_ptr<Stage>& stagePtr);
			virtual ~ViewChainLetter() {};
			virtual void OnCreate() override;
			virtual void OnUpdate() override;
			//ゲージの位置、大きさが変動する
			void GageAct();
			//ゲージの表示非表示を制御する
			void ViewGage();
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
		void ViewNum();
	};
	//--------------------------------------------------------------------------------------
	//スコアに飛んでいくチェインの数字
	//--------------------------------------------------------------------------------------
	class FlyingChain : public GameObject {
	protected:
		int m_places;	// 表示桁
		int m_score;	// 表示するスコア
		float m_wait;
		float m_leap;
		Vec3 p0 = Vec3(880, -380, 0);
		Vec3 p1 = Vec3(1000,-400,0);
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
		void ViewNum();
		void Fly();
		void FlySet(int chain);
	};
}