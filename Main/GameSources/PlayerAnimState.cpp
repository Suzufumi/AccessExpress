#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefaultAnim)
	
	void PlayerDefaultAnim::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Default");
	}

	void PlayerDefaultAnim::Execute(const shared_ptr<Player>& Obj)
	{

	}

	void PlayerDefaultAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}


	IMPLEMENT_SINGLETON_INSTANCE(PlayerMoveAnim)

	void PlayerMoveAnim::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Move");
	}

	void PlayerMoveAnim::Execute(const shared_ptr<Player>& Obj)
	{

	}

	void PlayerMoveAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}


	IMPLEMENT_SINGLETON_INSTANCE(PlayerFlyAnim)

	void PlayerFlyAnim::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Fly");
	}

	void PlayerFlyAnim::Execute(const shared_ptr<Player>& Obj)
	{
		auto drawComp = Obj->GetDynamicComponent<SmBaseDraw>();
		auto animTime = drawComp->GetCurrentAnimationTime();
		if (Obj->GetStateMachine()->GetCurrentState() == DataState::Instance())
		{
			Obj->GetAnimStateMachine()->ChangeState(PlayerDefaultAnim::Instance());
		}
	}

	void PlayerFlyAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}


}