#pragma once
#include "stdafx.h"

namespace basecross {
	//----------------------------------------------------------------------------------------------------------
	//建物、特に機能はない
	//----------------------------------------------------------------------------------------------------------
	class Wall : public GameObject {
		Vec3 m_position;
		Quat m_quat;
		Vec3 m_scale;
	public:
		//Wall(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		Wall(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		virtual ~Wall() {};
		virtual void OnCreate();
	};
	//--------------------------------------------------------------------------------------------------------------
	//ゴール
	//--------------------------------------------------------------------------------------------------------------
	class CheckPoint : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
		// CheckPointについたかどうか
		bool m_isArrive = false;
	public:
		CheckPoint(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		virtual ~CheckPoint() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		void ArriveCheckPoint();
		bool GetIsArrive() { return m_isArrive; }
		void SetIsArrive(bool isArrive) { m_isArrive = isArrive; }
	};
	//-------------------------------------------------------------------------------------------------------------
	//電波を発するオブジェクト、近づくとplayerのスピードが変わる
	//-------------------------------------------------------------------------------------------------------------
	class RadioTower : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
		float m_acceleration;
	public:
		RadioTower(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale, float acceleration);
		virtual ~RadioTower() { };
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		float GetAcceleration();
	};
	//--------------------------------------------------------------------------------------------------------------
	//ドライブ
	//--------------------------------------------------------------------------------------------------------------
	class Drive : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
	public:
		Drive(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~Drive() {};
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------------------------------
	// 背景用の球を作成
	//--------------------------------------------------------------------------------------------------------------
	class SkySphere : public GameObject
	{
	public:
		SkySphere(const shared_ptr<Stage>& ptrStage) :
			GameObject(ptrStage) {}
		virtual ~SkySphere() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};

	//--------------------------------------------------------------------------------------------------------------
	// 背景用のスプライトを作成
	//--------------------------------------------------------------------------------------------------------------
	class SkyBox : public GameObject
	{
	private:
		vector <shared_ptr<GameObject>> m_planes;
	public:
		SkyBox(const shared_ptr<Stage>& ptrStage);
		virtual ~SkyBox() {};

		virtual void OnCreate() override;
		virtual void OnUpdate2() override;
		virtual void OnDraw() override;
	};
}
//end basecross