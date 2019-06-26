/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Player : public GameObject {
		unique_ptr<StateMachine<Player>>  m_StateMachine;	//ステートマシーン
		unique_ptr<StateMachine<Player>> m_animStateMachine; // アニメーションのステートマシン

		//コンストラクタで持ってきた引数を入れる
		Vec3 m_position;
		Quat m_quaternion;
		Vec3 m_scale;

		const int BONUS_CHAIN = 5;	// ボーナスを与える倍数
		float m_nowFallSpeed = 8.0f;		//落下のスピード
		float m_nowWalkSpeed = 8.0f;		//現在の移動のスピード
		float m_cameraLookUp;			//カメラが見上げる高さ
		bool m_isFall = true;				//Y軸方向の力を加えるかどうか
		bool m_isHit = false;				//オブジェクトに当たっているかどうか
		float m_Lerp = 0.0f;				//ベジエ曲線のための経過時間変数
		Vec3 p0, p1,p2;						//ベジエ曲線のための位置情報
		int m_chain = 0;		// コンボのための変数
		int m_chainTime = 0;	// コンボとコンボの時間を図る時間
		int m_comboChainLimit = 0; // コンボが進んでいくにつれて更新する制限時間
		float m_BezierSpeedLeap;		//距離に応じた飛ぶ処理へのスピード補正
		float m_BezierSpeed = 30.0f;	//飛ぶ際の基準スピード
		bool m_islockon = false;		//ロックオンしている際にtrue
		float m_rayRange;				//レイの届く距離
		float m_rayRangeDefolt = 30.0f;	//レイの届く距離基準値
		bool m_isGoLink;				//飛ぶことが確定していたらtrue
		float m_nesting = NULL;	//押し出しに使うめり込みの最小値
		int m_nestingMin;	//押し出しに使うめり込んでいる方向

		CONTROLER_STATE m_pad;					//パッドの全情報
		weak_ptr<RayRangeViewObj> m_RayRangeViewObj;
		weak_ptr<SightingDevice> m_SightingDevice;	//照準
		weak_ptr<Drone> m_Drone;
		int m_DroneNo = NULL;
		int m_MailNum = NULL;
		weak_ptr<GameObject> m_LockOnObj;		//ロックオンしているオブジェクト
		weak_ptr<TpsCamera> m_tpsCamera; // カメラのインスタンスを受け取る

		Vec3 m_padDir;							//左スティックの向きを入れる
		Vec3 m_forward;							//カメラの方向を踏まえたプレイヤーの向いている方向

	public:
		enum Target {
			LINK,
			DRONE,
			MAIL
		};
		enum FaceState
		{
			Default = 3,
			Smile = 4,
			EyeClose = 6,
			Regret = 7
		};
		FaceState m_faceNum = Default;
		Target m_target;

		Player(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~Player() {};
		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		virtual void OnUpdate2() override; // 後更新
		//アクセサ
		const unique_ptr<StateMachine<Player>>& GetStateMachine() {
			return m_StateMachine;
		}

		Target GetTarget() { return m_target; };

		const unique_ptr<StateMachine<Player>>& GetAnimStateMachine()
		{
			return m_animStateMachine;
		}

		//衝突したとき
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//衝突している
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		//角度の向きを考慮したスティックの角度を算出する
		void Forword();
		//落ちる処理
		void Fall();
		//左スティックの値でプレイヤーを回転させる
		void PlayerRoll();

		//照準のオブジェクトを管理する
		void SetSightingDevice(weak_ptr<SightingDevice> dev) {
			m_SightingDevice = dev;
			SightingDeviceDrawActive(true);
		}
		//ベジエ曲線の初期ポジション設定
		void SetBezierPoint(Vec3 point);
		//照準の位置を変える
		void SightingDeviceChangePosition();
		//Rayを飛ばす
		void RayShot();
		bool RayOBBHit(Vec3 origin, Vec3 originDir, shared_ptr<GameObject> obj);
		//Rayとリンクオブジェクトが当たっているかを見る処理
		void LinkRayCheck(Vec3 origin, Vec3 originDir);
		//Rayとドローンが当たっているかを見る処理
		void DroneRayCheck(Vec3 origin, Vec3 originDir);
		//Rayとメールが当たっているか
		void MailRayCheck(Vec3 origin, Vec3 originDir);
		//ベジエ曲線でリンクへ飛ぶ処理
		void LinkGo();
		//ベジエ曲線でドローンへ飛ぶ処理
		void DroneGo();
		// ベジェ曲線でメールに飛ぶ
		void MailGo();
		//ロックオン対象がいたら、そちらを向く
		void RockOn();
		// ロックオンするオブジェクトを設定
		void RockOnObject(Vec3 origin, Vec3 originDir, wstring groupName, float correction);
		//押し出し判定
		void ExtrusionJudgment(const weak_ptr<GameObject>& Other);
		//押し出し処理
		void Extrusion();

		// 表情の切り替え
		void FaceChanger(FaceState beforeFace, FaceState afterFace);
		// タイムアップ時にアニメーションを見せる
		void ShowTimeUpAnime();
		//タイムアップアニメーションが終わったらリザルトにいく
		void TimeUpAnimeFinishToResult();
		//照準の表示を切り替える
		void SightingDeviceDrawActive(bool f) { 
			auto devi = m_SightingDevice.lock(); 
			devi->SetDrawActive(f);
		}
		Vec3 GetSightPos() { 
			return m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition(); 
		}

		// チェインを加算する
		void AddChain();
		// チェイン失敗時に色々リセットする
		void ChainFailure();
		// 現在のコンボ数を返す
		int GetChain() const { return m_chain; }
		// コンボとコンボの制限時間を加算する
		void AddChainTime() { m_chainTime++; }
		// 現在のコンボ間の時間を返す
		int GetChainTimeLim() const { return m_chainTime; }
		void ResetTimeLim() { m_chainTime = 0; }
		int GetComboChainLimit() const{ return m_comboChainLimit; }
		void SetComboChainLimit(int chainLim) { m_comboChainLimit = chainLim; }
		float GetRayRange() { return m_rayRange; }
		void ResetGoLink() { m_isGoLink = false; }
		Vec2 forwardAngle();
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
	class DataState : public ObjState<Player> {
		DataState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<DataState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	//class ClearState : public ObjState<Player>;
	//用途: クリアアニメーション状態
	//--------------------------------------------------------------------------------------
	class ClearState : public ObjState<Player> {
		ClearState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<ClearState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

}
//end basecross

