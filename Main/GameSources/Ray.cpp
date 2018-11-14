#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Ray::Ray(const shared_ptr<Stage>& stage,weak_ptr<Player>& player)
		:GameObject(stage),m_player(player)
	{
	}

	void Ray::OnCreate() {
		m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();

	}
	void Ray::OnUpdate() {
		auto player = m_player.lock();
		auto playerPos = player->GetComponent<Transform>()->GetWorldPosition();
		auto dir = playerPos - m_cameraPos;
		dir = dir.normalize();

		//if()
		//HitTest::SEGMENT_OBB(playerPos,playerPos + dir * m_length,)
	}
}