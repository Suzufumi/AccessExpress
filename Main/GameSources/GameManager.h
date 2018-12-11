#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//----------------------------------------------------------------------------
	//ゲームを管理するシングルトン
	//----------------------------------------------------------------------------
	class GameManager final {
	private:
		GameManager() {} // コンストラクタを private に置く。
		GameManager(const GameManager&); // コピーコンストラクタも private に置き、定義しない。
		 ~GameManager() {} // デストラクタを private に置く。

		 bool m_isSlow = false;	//スロー状態だったらtrue
	public:
		static GameManager& GetInstance() {
			static GameManager inst; // private なコンストラクタを呼び出す。
			return inst;
		}
		//スローフラグの取得
		const bool GetOnSlow() {
			return m_isSlow;
		}
		//スローフラグの設定
		void SetOnSlow(bool f) {
			m_isSlow = f;
		}
	};

}