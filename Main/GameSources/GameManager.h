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
		GameManager& operator=(const GameManager&); // コピー代入演算子も private に置き、定義しない
		 ~GameManager() {} // デストラクタを private に置く。

		 bool m_isSlow = false;	//スロー状態だったらtrue
		 float m_slowSpeed = 20.0f;	//スロー状態で何分の１になるかの変数	
		 CONTROLER_STATE m_pad;//パッドの全情報
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
		float GetSlowSpeed() {
			return m_slowSpeed;
		}
		//パッドの情報を更新する
		void UpDatePadData() {
			// アプリケーション取得
			auto& app = App::GetApp();
			// ゲームコントローラー取得
			auto device = app->GetInputDevice();
			if (app->GetScene<Scene>()->GetGameStart()) {
				m_pad = device.GetControlerVec()[0];
			}
		};
		CONTROLER_STATE GetPad() {
			return m_pad;
		}
	};

}