#include"stdafx.h"
#include"Project.h"
#include "Enemy.h"

namespace basecross {
	Drone::Drone(const shared_ptr<Stage>& stage ,Vec3 pos)
		: OBBObject(stage,pos,Vec3(1.0f,1.0f,1.0f)),m_position(pos)
	{
	}
	void Drone::OnCreate() {
		//半径
		float radius = 15.0f;
		//ベジエ曲線で円の形に動かすための制御点を作るために、
		//コンストラクタで設定された位置を横にずらした点を作る
		Vec3 centerPos = m_position + Vec3(-radius, 0.0f, 0.0f);
		float deg90 = Deg2Rad(90.0f);
		//点を基準に円状に点を配置する
		for (int i = 0; i <= 3; i++) {
			m_roop[i].p0 = centerPos + Vec3(radius * cosf(deg90*i), 0, radius * sinf(deg90*i));
			m_roop[i].p1 = 
				centerPos + Vec3((radius + 4) * cosf(deg90*i + Deg2Rad(45.0f)),0,
				(radius + 4) * sinf(deg90*i + Deg2Rad(45.0f)));
			m_roop[i].p2 = centerPos + Vec3(radius * cosf(deg90*(i + 1)), 0, radius * sinf(deg90*(i + 1)));
		}
		OBBObject::OnCreate();
		GetStage()->GetSharedObjectGroup(L"Drone")->IntoGroup(GetThis<Drone>());
	}
	void Drone::OnUpdate() {
		Jammer();

		Vec3 pos = GetComponent<Transform>()->GetWorldPosition();

		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_roopNum].p0.x + 
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_roopNum].p1.x + m_Lerp * m_Lerp * m_roop[m_roopNum].p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_roopNum].p0.y +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_roopNum].p1.y + m_Lerp * m_Lerp * m_roop[m_roopNum].p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * m_roop[m_roopNum].p0.z +
			2 * (1 - m_Lerp) * m_Lerp * m_roop[m_roopNum].p1.z + m_Lerp * m_Lerp * m_roop[m_roopNum].p2.z;

		m_Lerp += App::GetApp()->GetElapsedTime()/2.0f;
		if (m_Lerp >= 1.0f) {
			m_Lerp = 0.0f;
			m_roopNum += 1;
			if (m_roopNum > 3) {
				m_roopNum = 0;
			}
		}
		GetComponent<Transform>()->SetWorldPosition(pos);

		if (m_isDead) {
			GetStage()->GetSharedGameObject<Player>(L"Player")->SetJummerSpeed(1.0f);
			SetUpdateActive(false);
			//GetStage()->RemoveGameObject<Drone>(GetThis<Drone>());
		}
	}
	void Drone::Jammer() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetComponent<Transform>()->GetWorldPosition();
		auto dronePos = GetComponent<Transform>()->GetWorldPosition();

		auto length = (dronePos - playerPos).length();
		if (length < m_JammerRang) {
			player->SetJummerSpeed(0.5f);
		}
		else {
			player->SetJummerSpeed(1.0f);
		};
		
	}
}