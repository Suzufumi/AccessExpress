/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Player : public GameObject {
		unique_ptr< StateMachine<Player> >  m_StateMachine;	//ステートマシーン

		float m_nowFallSpeed = 8.0f;		//落下のスピード
		float m_nowWalkSpeed = 10.0f;		//現在の移動のスピード
		float m_humanWalkSpeed = 15.0f;		//人間状態の移動スピード
		float m_dataWalkSpeed = 10.0f;		//データ状態の移動スピード
		float m_angleX;
		float m_angleY;
		float m_maxAngleSpeed;			//カメラが回転するスピード
		float m_cameraHeight;			//カメラの初期高さ
		float m_cameraDistance;			//カメラのプレイヤーからの距離
		bool m_isFall = true;					//Y軸方向の力を加えるかどうか
		bool m_isHaveFile = false;
		bool m_isHit = false;					//オブジェクトに当たっているかどうか
		float m_Lerp = 0.0f;					//ベジエ曲線のための経過時間変数
		Vec3 p0, p1,p2;							//ベジエ曲線のための位置情報
		float m_energy = 100.0f;
		float m_changeEnergy = 1.0f;
		float m_maxEnergy = 100.0f;
		int m_chain = 0;		// コンボのための変数
		int m_chainTimeLimit = 0;	// コンボとコンボの時間を図る時間
		bool m_isAdvanceTime = false; // 時間をすすめるかどうかのフラグ
		Vec3 m_response;				//落ちた時に復帰する場所
		float m_responseHeght = 0.0f;	//リスポーンが実行される高さ
		float m_JummerSpeed = 1.0f;		//スピードにかけられる妨害

		CONTROLER_STATE m_pad;					//パッドの全情報

		//コンストラクタで持ってきた引数を入れる
		Vec3 m_position;
		Quat m_quaternion;
		Vec3 m_scale;

		weak_ptr<RadioTowerHitJudgment> m_RadioTowerHitJudgment;
		weak_ptr<File> m_File;
		weak_ptr<SightingDevice> m_SightingDevice;
		weak_ptr<Drone> m_Drone;
		int m_DroneNo = NULL;

		Vec3 m_padDir;							//左スティックの向きを入れる
		Vec3 m_forward;							//カメラの方向を踏まえたプレイヤーの向いている方向

		weak_ptr<TpsCamera> m_tpsCamera; // カメラのインスタンスを受け取る

		//押し出しに使うめり込みの最小値
		float m_nesting = NULL;
		//押し出しに使うめり込んでいる方向
		int m_nestingMin;

	public:
		enum State {
			HUMAN = 0,
			DATA = 1
		};

		Player(const shared_ptr<Stage>& StagePtr, Vec3 pos, Quat quat, Vec3 sca);
		virtual ~Player() {};
		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		virtual void OnUpdate2() override; // 後更新
		//アクセサ
		const unique_ptr<StateMachine<Player>>& GetStateMachine() {
			return m_StateMachine;
		}

		//衝突したとき
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//衝突している
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		//衝突が解除されたとき
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		//押し出し処理
		void Extrusion(const weak_ptr<GameObject>& Other);

		//XZ平面の移動処理
		void Walk();
		//落ちる処理
		void Fall();

		void CameraRoll();
		void CameraControll();

		//ステートに応じて平行移動のスピードを変える
		void ChangeWalkSpeed(State state);
		//妨害電波による速度低下値をセットする
		void SetJummerSpeed(float speed) { m_JummerSpeed = speed; };
		//子オブジェクトしてもっている電波塔との当たり判定をプレイヤーのほうでも認知する
		void SetRadioTowerHitJudgment(weak_ptr<RadioTowerHitJudgment> hit) {
			m_RadioTowerHitJudgment = hit;
		}
		//照準のオブジェクトを管理する
		void SetSightingDevice(weak_ptr<SightingDevice> dev) {
			m_SightingDevice = dev;
			SightingDeviceDrawActive(false);
		}
		//照準の位置を変える
		void SightingDeviceChangePosition();
		//ベジエ曲線で飛ぶ処理
		void LinkGo();
		//ベジエ曲線の初期ポジション設定
		void SetBezierPoint(Vec3 point);
		//Rayを飛ばす
		void RayShot();
		//Rayとリンクオブジェクトが当たっているかを見る処理
		void LinkRayCheck(Vec3 origin,Vec3 originDir);
		//Rayとリンクオブジェクトが当たっているかを見る処理
		void DroneRayCheck(Vec3 origin, Vec3 originDir);
		//Aボタンが押された
		bool CheckAButton();

		//エネルギーが減るようにする
		void ChengeEnergyMai() { m_changeEnergy = -1.0f; }
		//エネルギーが増えるようにする
		void ChengeEnergyPur() { m_changeEnergy = 1.0f; }
		//エネルギーの量を返す
		float GetEnergy() { return m_energy; }
		//照準の表示を切り替える
		void SightingDeviceDrawActive(bool f) { 
			auto devi = m_SightingDevice.lock(); 
			devi->SetDrawActive(f);
		}

		void ComboBonus(int nowChains);
		// コンボを加算する
		void AddCombo() { m_chain++; }
		// コンボをリセットする
		void ResetCombo() { m_chain = 0; }
		// 現在のコンボ数を返す
		int GetChain() const { return m_chain; }
		// コンボとコンボの制限時間を加算する
		void AddChainTimeLimit() { m_chainTimeLimit++; }
		// 現在のコンボ間の時間を返す
		int GetChainTimeLim() const { return m_chainTimeLimit; }
		void ResetTimeLim() { m_chainTimeLimit = 0; }
		bool GetAdvanceTimeActive() const { return m_isAdvanceTime; }
		void SetAdvanceTimeActive(bool advanceTime) { m_isAdvanceTime = advanceTime; }
		//落下した際にリスポーン位置へワープする
		void Response();

		void DrawStrings();
	};

	//--------------------------------------------------------------------------------------
	//class WalkState : public ObjState<Player>;
	//用途: 歩いている状態
	//--------------------------------------------------------------------------------------
	class WalkState : public ObjState<Player> {
		WalkState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<WalkState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//class LinkState : public ObjState<Player>;
	//用途: リンク上を飛んでいる状態
	//--------------------------------------------------------------------------------------
	class LinkState : public ObjState<Player> {
		LinkState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<LinkState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//class DateState : public ObjState<Player>;
	//用途: データ体状態
	//--------------------------------------------------------------------------------------
	class DateState : public ObjState<Player> {
		DateState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<DateState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

}
//end basecross

