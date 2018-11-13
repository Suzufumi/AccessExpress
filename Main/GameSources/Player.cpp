/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross{
	//------------------------------------------------------------------------------------------------
	//playerのコンストラクタ
	//------------------------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr, Vec3 pos, Quat quat, Vec3 sca)
		:GameObject(StagePtr), m_position(pos), m_quaternion(quat), m_scale(sca)
	{
	}
	//-------------------------------------------------------------------------------------------------
	//初期化
	//-------------------------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

		//物理を使うために渡す
		PsBoxParam Ps(ptrTrans->GetWorldMatrix(), 1.0f, false, PsMotionType::MotionTypeActive);
		//四角形の当たり判定をセット
		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(true);
		//コリジョンの判定をしない
		col->SetAfterCollision(AfterCollision::None);
		//playerに持たせて使うものには衝突しない
		col->AddExcludeCollisionTag(L"PlayerUse");

		//文字列をつける
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 0.5f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticModelDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"PLAYER_MODEL");
		// 変換した行列を代入
		drawComp->SetMeshToTransformMatrix(spanMat);
		//Col4 Color(1.0f, 0.2f, 1.0f, 0.7f);
		//drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		// レイヤーの調整
		SetDrawLayer(1);

		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//最初のステートをWalkStateに設定
		m_StateMachine->ChangeState(WalkState::Instance());
	}
	//-------------------------------------------------------------------------------------------------------------
	//Update
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate() {
		// アプリケーション取得
		auto& app = App::GetApp();
		// 1フレームの実行にかかった時間を取得
		float delta = app->GetElapsedTime();


		// ゲームコントローラー取得
		auto device = app->GetInputDevice();
		m_pad = device.GetControlerVec()[0];
		//左スティックの入力方向を求める
		m_padDir = Vec3(m_pad.fThumbLX, 0.0f, m_pad.fThumbLY);
		m_padDir = m_padDir.normalize();

		//右スティックの値でカメラの回転処理を行う
		CameraRoll();

		//ステートマシンのアップデート
		m_StateMachine->Update();
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		////当たり判定を取り出す
		//auto hitJud = m_RadioTowerHitJudgment.lock();
		////当たり判定から速度を持ってくる
		//m_nowWalkSpeed = hitJud->GetAcceleration();
		////当たり判定のスピードを初期化
		//hitJud->Rset();

		// トランスフォームコンポーネントから座標を取得する
		auto pos = GetComponent<Transform>()->GetWorldPosition();

		// カメラの注視点をプレイヤーの座標に合わせる
		auto camera = GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		auto eye = pos + Vec3(cos(m_AngleY) * m_cameraDistance, m_cameraHeight, sin(m_AngleY) * m_cameraDistance);
		camera->SetEye(eye);


		// デバッグ文字の表示
		DrawStrings();
	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突したとき
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//ファイルに変換する
		auto file = dynamic_pointer_cast<File>(Other);
		if (file) {
			file->SetOnPlayer(GetThis<GameObject>());
			m_File = file;
			m_isHaveFile = true;
		}
		auto addless = dynamic_pointer_cast<AddlessCertificate>(Other);
		if (addless) {
			if (!addless->GetUnlockGoal())
			{
				addless->FindGoal();
				addless->SetDrawActive(false);
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突し続けているとき
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//playerの下面と衝突した物体の上面が当たっていたら
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//自動でY方向に力を加える処理を行わないようにする
			m_isFall = false;
		}

		Extrusion(Other);

		auto access = dynamic_pointer_cast<LinkObject>(Other);
		if (access) {
			if (state != L"Link") {
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B) {
					SetBezierPoint(access->GetGoPosition());
					m_Lerp = 0;
					state = L"Link";
					m_StateMachine->ChangeState(LinkState::Instance());
				}
			}
		}
		//ドライブに変換できた
		//auto drive = dynamic_pointer_cast<Drive>(Other);
		//if (drive) {
		//	if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B) {
		//		//playerの持っているファイルを取り出す
		//		auto file = m_File.lock();
		//		//取り出せたら
		//		if (file != nullptr) {
		//			//ファイルを持っている(見えている)
		//			if (m_isHaveFile) {
		//				//見えなくする
		//				file->UnLookFile();
		//				//持ってなくする
		//				m_isHaveFile = false;
		//			}
		//			//ファイルを持ってない(見えない)
		//			else {
		//				//見えるようにする
		//				file->LookFile();
		//				//持っている
		//				m_isHaveFile = true;
		//			}
		//		}
		//	}
		//}
	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突が解除されたとき
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		m_isFall = true;
	}


	//--------------------------------------------------------------------------------------------
	//XZ平面の移動処理
	//--------------------------------------------------------------------------------------------
	void Player::Walk() {
		auto playerTrans = GetComponent<Transform>();
		auto playerPos = playerTrans->GetWorldPosition();
		//左スティックに値が入力されていたら
		if (m_padDir.x > 0.4f || m_padDir.x < -0.4f ||
			m_padDir.z > 0.4f || m_padDir.z < -0.4f) {
			//方向と移動スピードを掛け算してVelocityに与える
			playerPos.x += m_nowWalkSpeed * m_forward.x * App::GetApp()->GetElapsedTime();
			playerPos.z += m_nowWalkSpeed * m_forward.z * App::GetApp()->GetElapsedTime();
		}
		//左スティックが入力されてなかったら
		//else {
		//	//Velocity値をだんだん小さくする
		//	m_velocity.x = m_velocity.x * 0.9f;
		//	m_velocity.z = m_velocity.x * 0.9f;
		//}
		if (m_isFall) {
			playerPos.y += -m_nowFallSpeed * App::GetApp()->GetElapsedTime();
		}
		playerTrans->SetWorldPosition(playerPos);
	}
	//--------------------------------------------------------------------------------------------
	//カメラの回転処理
	//--------------------------------------------------------------------------------------------
	void Player::CameraRoll() {
		float delta = App::GetApp()->GetElapsedTime();
		// スティックの傾きを角度に変換する
		float padRad = atan2f(m_padDir.z, m_padDir.x);

		m_AngleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // カメラを回転させる
		m_cameraHeight += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // カメラを昇降させる

		//360度を越えたら0にする
		if (m_AngleX > 360) {
			m_AngleX = 0;
		}
		//0度よりも小さくなったら
		if (m_AngleX < 0) {
			m_AngleX = 360;
		}

		if (m_padDir.length() != 0.0f) {
			// スティックの角度にカメラの角度を加える
			padRad += m_AngleY + XM_PIDIV2;  // カメラの角度に９０度加える
			// 角度をXZ成分に分解してベクトルを作り直す
			m_padDir.x = cosf(padRad); // 新しい角度を X 成分に分解する
			m_padDir.z = sinf(padRad); // 新しい角度を Z 成分に分解する

			m_forward = m_padDir;
		}
	}
	//リンクからリンクへ飛ぶ処理
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		//計算のための時間加算
		m_Lerp += App::GetApp()->GetElapsedTime();
		if (m_Lerp >= 1.0f) {
			m_Lerp = 1.0f;
			state = L"walk";
			//飛び終わったらステートを歩きにする
			m_StateMachine->ChangeState(WalkState::Instance());
		}
		//ベジエ曲線の計算
		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * p0.x + 2 * (1 - m_Lerp) * m_Lerp * p1.x + m_Lerp * m_Lerp * p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * p0.y + 2 * (1 - m_Lerp) * m_Lerp * p1.y + m_Lerp * m_Lerp * p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * p0.z + 2 * (1 - m_Lerp) * m_Lerp * p1.z + m_Lerp * m_Lerp * p2.z;
		GetComponent<Transform>()->SetWorldPosition(pos);
	}
	void Player::SetBezierPoint(Vec3 point) {
		p0 = GetComponent<Transform>()->GetWorldPosition();
		p1 = point + Vec3(0, 10, 0);
		p2 = point + Vec3(0,1,0);
	}
	void Player::Extrusion(const weak_ptr<GameObject>& Other) {
		//playerの情報
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetWorldPosition();
		auto scale = trans->GetScale();
		auto half = scale * 0.5f;
		//衝突したオブジェクトの情報
		auto other = Other.lock();
		auto otherTrans = other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetWorldPosition();
		auto otherScale = otherTrans->GetScale();
		auto otherHalf = otherScale * 0.5f;
		// 衝突しているので応答を行う
		float diff[6] = {
			(otherPos.x + otherHalf.x) - (pos.x - half.x), // 右
			(pos.x + half.x) - (otherPos.x - otherHalf.x), // 左
			(otherPos.y + otherHalf.y) - (pos.y - half.y), // 上
			(pos.y + half.y) - (otherPos.y - otherHalf.y), // 下
			(otherPos.z + otherHalf.z) - (pos.z - half.z), // 奥
			(pos.z + half.z) - (otherPos.z - otherHalf.z), // 手前
		};
		// 側面の距離が最小になっている要素を見つける
		int min = 0;
		for (int i = 0; i < 6; i++) {
			if (diff[i] < diff[min]) {
				min = i;
			}
		}
		// 最小になっている方向に対して押し出しを行う
		switch (min) {
		case 0:
			pos.x += diff[min];
			break;
		case 1:
			pos.x -= diff[min];
			break;
		case 2:
			pos.y += diff[min];
			break;
		case 3:
			pos.y -= diff[min];
			break;
		case 4:
			pos.z += diff[min];
			break;
		case 5:
			pos.z -= diff[min];
			break;
		default:
			break;
		}
		trans->SetWorldPosition(pos);
	}

	void Player::DrawStrings()
	{
		// FPSの取得
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring strFps(L"FPS: ");
		strFps += Util::UintToWStr(fps);
		wstring str = strFps+state;
		//文字列をつける
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);

	}

	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	用途: 歩き状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<WalkState> WalkState::Instance() {
		static shared_ptr<WalkState> instance(new WalkState);
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void WalkState::Enter(const shared_ptr<Player>& Obj) {
	}
	//ステート実行中に毎ターン呼ばれる関数
	void WalkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
	}
	//ステートにから抜けるときに呼ばれる関数
	void WalkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	用途: リンク上を飛んでいる状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
	}
	//ステート実行中に毎ターン呼ばれる関数
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->LinkGo();
	}
	//ステートにから抜けるときに呼ばれる関数
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
	}
}
//end basecross

