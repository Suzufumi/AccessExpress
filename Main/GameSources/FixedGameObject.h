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
		Wall(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~Wall() {};
		virtual void OnCreate();
	};
	//--------------------------------------------------------------------------------------------------------------
	//ゴール
	//--------------------------------------------------------------------------------------------------------------
	class Goal : public GameObject {
		Vec3 m_position;
		Quat m_quat;
		Vec3 m_scale;
	public:
		Goal(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~Goal() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		void ArriveGoal();
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
}
//end basecross