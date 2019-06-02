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

		 bool m_isGameStart;		//プレイヤーが操作できる状態ならtrue
		 bool m_isTimeUp = false;			//時間が切れたらtrue
		 bool m_isSlow = false;		//スロー状態だったらtrue
		 float m_slowSpeed = 30.0f;	//スロー状態で何分の１になるかの変数	
		 CONTROLER_STATE m_pad;		//パッドの全情報
		 int m_nowScore = 0;		//現在のスコアの値
		 float m_slowPassage;		//スローになってからの経過(1.0fが最大として扱う)
		 int m_maxChain = 0;
		 bool m_isFade = true;		// フェード中かどうか
		 int m_maxMailNum;
		 int m_mailNum;
		 //float m_controlGageSpeed = 1.0f;	// チェインゲージの進行スピードの管理
		 int m_bouns = 0;			//何回ボーナスをもらったか
	public:
		static GameManager& GetInstance() {
			static GameManager inst; // private なコンストラクタを呼び出す。
			return inst;
		}
		//--------------------------------------------------------------------------------------
		//操作ができる状態か設定
		//--------------------------------------------------------------------------------------
		void SetGameStart(bool f) { m_isGameStart = f; };
		//--------------------------------------------------------------------------------------
		//操作ができる状態か取得
		//--------------------------------------------------------------------------------------
		bool GetGameStart() { return m_isGameStart; };
		//--------------------------------------------------------------------------------------
		//タイムアップしたか設定
		//--------------------------------------------------------------------------------------
		void SetTimeUp(bool f) { m_isTimeUp = f; };
		//--------------------------------------------------------------------------------------
		//タイムアップしたか取得
		//--------------------------------------------------------------------------------------
		bool GetTimeUp() { return m_isTimeUp; };
		//--------------------------------------------------------------------------------------
		//タイムアップしたかのリセット
		//--------------------------------------------------------------------------------------
		void ResetTimeUp() { m_isTimeUp = false; };
		//--------------------------------------------------------------------------------------
		//スローフラグの取得
		//--------------------------------------------------------------------------------------
		const bool GetOnSlow() {
			return m_isSlow;
		}
		//--------------------------------------------------------------------------------------
		//スローフラグの設定
		//--------------------------------------------------------------------------------------
		void SetOnSlow(bool f) {
			m_isSlow = f;
		}
		//--------------------------------------------------------------------------------------
		//スロースピードを返す
		//--------------------------------------------------------------------------------------
		float GetSlowSpeed() {
			return m_slowSpeed;
		}
		//--------------------------------------------------------------------------------------
		//パッドの情報を更新する
		//--------------------------------------------------------------------------------------
		void UpDatePadData() {
			// アプリケーション取得
			auto& app = App::GetApp();
			// ゲームコントローラー取得
			auto device = app->GetInputDevice();
			if (m_isGameStart) {
				m_pad = device.GetControlerVec()[0];
			}
		};
		//--------------------------------------------------------------------------------------
		//パッドの入力情報を返す
		//--------------------------------------------------------------------------------------
		CONTROLER_STATE GetPad() {
			return m_pad;
		}
		//--------------------------------------------------------------------------------------
		//スコアを増やす
		//--------------------------------------------------------------------------------------
		void AddScore(int score){
			m_nowScore += score; 
		}
		//--------------------------------------------------------------------------------------
		//スコアの値を返す
		//--------------------------------------------------------------------------------------
		int GetScore() {
			return m_nowScore; 
		}
		//--------------------------------------------------------------------------------------
		//スコアを初期化する
		//--------------------------------------------------------------------------------------
		void ResetNowScore() {
			m_nowScore = 0;
		}
		//--------------------------------------------------------------------------------------
		//スローからの経過を設定する
		//--------------------------------------------------------------------------------------
		void AddSlowPassage(float passage) {
			//ボーナスによって加算の値が減る
			m_slowPassage += passage * (1.0f - (0.025f * m_bouns));
		}
		//--------------------------------------------------------------------------------------
		//スローからの経過の値を返す
		//--------------------------------------------------------------------------------------
		float GetSlowPassage() {
			return m_slowPassage;
		}
		//--------------------------------------------------------------------------------------
		//スローからの経過を初期化する
		//--------------------------------------------------------------------------------------
		void ResetSloawPassage() {
			m_slowPassage = 0;
		}
		//----------------------------------------------------------------------------------
		//マックスチェイン数取得
		//----------------------------------------------------------------------------------
		int GetMaxChain() { return m_maxChain; }
		//----------------------------------------------------------------------------------
		//マックスチェイン数更新
		//----------------------------------------------------------------------------------
		void SetMaxChain(int chain) { m_maxChain = chain; }
		//----------------------------------------------------------------------------------
		//マックスチェイン数初期化
		//----------------------------------------------------------------------------------
		void ResetMaxChain() { m_maxChain = 0; }

		bool GetIsFade()
		{ return m_isFade; }
		void SetIsFade(bool isFade)
		{ m_isFade = isFade; }

		//----------------------------------------------------------------------------------
		//届けたメールの総数を更新
		//----------------------------------------------------------------------------------
		void AddMaxMail(int mailNum)
		{ m_maxMailNum += mailNum; }
		int GetMaxMail()
		{ return m_maxMailNum; }
		void ResetMaxMail()
		{ m_maxMailNum = 0; }

		//----------------------------------------------------------------------------------
		//メール数を増やす
		//----------------------------------------------------------------------------------
		void AddMail() { m_mailNum++; }
		//----------------------------------------------------------------------------------
		//メール数を返す
		//----------------------------------------------------------------------------------
		int GetMail() { return m_mailNum; }
		//----------------------------------------------------------------------------------
		//メール数をリセットする
		//----------------------------------------------------------------------------------
		void ResetMail() { m_mailNum = 0; }

		void DecreaseMail() { m_mailNum--; }

		int GetBouns() { return m_bouns; }
		void ResetBouns() { m_bouns = 0; }
		void AddBouns() { m_bouns++; }
	};
}