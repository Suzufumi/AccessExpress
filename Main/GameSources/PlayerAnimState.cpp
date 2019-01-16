#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	///---------------------------------------------------------------------------------
	// Default
	///---------------------------------------------------------------------------------
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

	///---------------------------------------------------------------------------------
	// Move
	///---------------------------------------------------------------------------------
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

	///---------------------------------------------------------------------------------
	// Fly
	///---------------------------------------------------------------------------------

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

	///---------------------------------------------------------------------------------
	// Clear
	///---------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerClearAnim)

	void PlayerClearAnim::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Clear");
	}

	void PlayerClearAnim::Execute(const shared_ptr<Player>& Obj)
	{
	}

	void PlayerClearAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}

	///---------------------------------------------------------------------------------
	// Over
	///---------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerOverAnim)

	void PlayerOverAnim::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetDynamicComponent<SmBaseDraw>()->ChangeCurrentAnimation(L"Over");
	}

	void PlayerOverAnim::Execute(const shared_ptr<Player>& Obj)
	{
	}

	void PlayerOverAnim::Exit(const shared_ptr<Player>& Obj)
	{

	}

}