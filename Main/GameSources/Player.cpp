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
		//物理コンポーネントをセット
		auto psPtr = AddComponent<RigidbodyBox>(Ps);
		//自動の重力を使わない
		psPtr->SetAutoGravity(false);
		//四角形の当たり判定をセット
		auto col = AddComponent<CollisionObb>();
		//コリジョンの判定をしない
		col->SetAfterCollision(AfterCollision::None);
		//playerに持たせて使うものには衝突しない
		col->AddExcludeCollisionTag(L"PlayerUse");

		//文字列をつける
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
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

		// トランスフォームコンポーネントから座標を取得する
		auto pos = GetComponent<Transform>()->GetWorldPosition();

		// カメラの注視点をプレイヤーの座標に合わせる
		auto camera = GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		auto eye = pos + Vec3(cos(m_AngleY) * m_cameraDistance, m_cameraHeight, sin(m_AngleY) * m_cameraDistance);
		camera->SetEye(eye);
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		//現在のVelocity値を取得
		m_velocity = GetComponent<RigidbodyBox>()->GetLinearVelocity();

		//当たり判定を取り出す
		auto hitJud = m_RadioTowerHitJudgment.lock();
		//当たり判定から速度を持ってくる
		m_nowWalkSpeed = hitJud->GetAcceleration();
		//当たり判定のスピードを初期化
		hitJud->Rset();

		//XZ平面の移動処理を行う
		Walk();

		//上昇下降の処理
		Floating();

		//Velocityの更新
		GetComponent<RigidbodyBox>()->SetLinearVelocity(m_velocity);

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
			addless->FindGoal();
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
			//Y軸方向のvelocityを0にする
			auto vel = GetComponent<RigidbodyBox>()->GetLinearVelocity();
			vel.y = 0;
			GetComponent<RigidbodyBox>()->SetLinearVelocity(vel);
			//オブジェクトの上にポジションをずらす
			auto pos = trans->GetWorldPosition();
			pos.y = otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2 + trans->GetScale().y / 2;
			trans->SetWorldPosition(pos);
			//自動でY方向に力を加える処理を行わないようにする
			m_isFall = false;
		}
		//ドライブに変換できた
		auto drive = dynamic_pointer_cast<Drive>(Other);
		if (drive) {
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B) {
				//playerの持っているファイルを取り出す
				auto file = m_File.lock();
				//取り出せたら
				if (file != nullptr) {
					//ファイルを持っている(見えている)
					if (m_isHaveFile) {
						//見えなくする
						file->UnLookFile();
						//持ってなくする
						m_isHaveFile = false;
					}
					//ファイルを持ってない(見えない)
					else {
						//見えるようにする
						file->LookFile();
						//持っている
						m_isHaveFile = true;
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突が解除されたとき
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		m_isFall = true;
	}

	//-------------------------------------------------------------------------------------------------
	//上昇下降の処理
	//------------------------------------------------------------------------------------------------
	void Player::Floating() {
		//Aボタン押された
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A) {
			//上昇実行フラグtrue
			m_isFloa = true;
		}
		//Aボタン離れた
		if (m_pad.wReleasedButtons & XINPUT_GAMEPAD_A) {
			//上昇実行フラグfalse
			m_isFloa = false;
		}
		//上昇中なら
		if (m_isFloa == true) {
			//Y軸方向の力を固定値に
			m_velocity.y = m_nowWalkSpeed;
		}
		//上昇中じゃなくて空中にいる
		else if (m_isFall == true) {
			//だんだん下方向へ力が加わっていく
			m_velocity.y += -m_nowWalkSpeed * App::GetApp()->GetElapsedTime();
		}
	}

	//--------------------------------------------------------------------------------------------
	//XZ平面の移動処理
	//--------------------------------------------------------------------------------------------
	void Player::Walk() {
		//左スティックに値が入力されていたら
		if (m_padDir.x > 0.4f || m_padDir.x < -0.4f ||
			m_padDir.z > 0.4f || m_padDir.z < -0.4f) {
			//方向と移動スピードを掛け算してVelocityに与える
			m_velocity.x = m_nowWalkSpeed * m_forward.x;
			m_velocity.z = m_nowWalkSpeed * m_forward.z;
		}
		//左スティックが入力されてなかったら
		else {
			//Velocity値をだんだん小さくする
			m_velocity.x = m_velocity.x * 0.9f;
			m_velocity.z = m_velocity.x * 0.9f;
		}
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

	void Player::DrawStrings()
	{
		// FPSの取得
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring strFps(L"FPS: ");
		strFps += Util::UintToWStr(fps);
		wstring str = strFps;
		//文字列をつける
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);

	}

}
//end basecross

