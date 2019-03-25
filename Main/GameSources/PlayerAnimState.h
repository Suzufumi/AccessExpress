#pragma once
#include "stdafx.h"

namespace basecross
{
	class Player;

	///------------------------------------------------------------------------------------------
	// 通常アニメーション
	///------------------------------------------------------------------------------------------
	class PlayerDefaultAnim : public ObjState<Player>
	{
		PlayerDefaultAnim() {};
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultAnim)
		virtual void Enter(const shared_ptr<Player>& Obj) override;
		virtual void Execute(const shared_ptr<Player>& Obj) override;
		virtual void Exit(const shared_ptr<Player>& Obj) override;
	};

	///------------------------------------------------------------------------------------------
	// 移動アニメーション
	///------------------------------------------------------------------------------------------
	class PlayerMoveAnim : public ObjState<Player>
	{
		PlayerMoveAnim() {};
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerMoveAnim)
		virtual void Enter(const shared_ptr<Player>& Obj) override;
		virtual void Execute(const shared_ptr<Player>& Obj) override;
		virtual void Exit(const shared_ptr<Player>& Obj) override;
	};

	///------------------------------------------------------------------------------------------
	// 電波移動アニメーション
	///------------------------------------------------------------------------------------------
	class PlayerFlyAnim : public ObjState<Player>
	{
		PlayerFlyAnim() {};
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerFlyAnim)
		virtual void Enter(const shared_ptr<Player>& Obj) override;
		virtual void Execute(const shared_ptr<Player>& Obj) override;
		virtual void Exit(const shared_ptr<Player>& Obj) override;
	};

	///------------------------------------------------------------------------------------------
	// クリアアニメーション
	///------------------------------------------------------------------------------------------
	class PlayerClearAnim : public ObjState<Player>
	{
		PlayerClearAnim() {};
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerClearAnim)
		virtual void Enter(const shared_ptr<Player>& Obj) override;
		virtual void Execute(const shared_ptr<Player>& Obj) override;
		virtual void Exit(const shared_ptr<Player>& Obj) override;
	};

	///------------------------------------------------------------------------------------------
	// オーバーアニメーション
	///------------------------------------------------------------------------------------------
	class PlayerOverAnim : public ObjState<Player>
	{
		PlayerOverAnim() {};
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerOverAnim)
		virtual void Enter(const shared_ptr<Player>& Obj) override;
		virtual void Execute(const shared_ptr<Player>& Obj) override;
		virtual void Exit(const shared_ptr<Player>& Obj) override;
	};
}