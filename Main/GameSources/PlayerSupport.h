#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------------------
	//playerにくっつける電波塔との衝突判定を行う当たり判定オブジェクト
	//------------------------------------------------------------------------------------------------------------
	class RadioTowerHitJudgment : public GameObject {
		float m_Acceleration;
		float m_length;
		weak_ptr<GameObject> m_player;
	public:
		RadioTowerHitJudgment(const shared_ptr<Stage>& StagePtr, weak_ptr<GameObject> player);
		virtual ~RadioTowerHitJudgment() {};
		virtual void OnCreate() override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		void Rset() {
			m_length = 5.0f;
			m_Acceleration = 6.0f;
		}
		float GetAcceleration() { return m_Acceleration; }
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
	public:
		AddlessCertificate(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~AddlessCertificate() {};
		virtual void OnCreate() override;
		//ゴールの情報を与える
		void SetGoal(weak_ptr<Goal> goal) { m_goal = goal; }
		void FindGoal();
	};
}