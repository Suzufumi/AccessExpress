#pragma once
#include "stdafx.h"

namespace basecross {
	//----------------------------------------------------------------------------------------------------------
	//建物、特に機能はない
	//----------------------------------------------------------------------------------------------------------
	class Wall : public GameObject {
		Vec3 m_position;
		Quat m_quat;
		Vec3 m_scale;
	public:
		//Wall(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		Wall(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		virtual ~Wall() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnDraw();
	};
	//--------------------------------------------------------------------------------------------------------------
	// アンテナ
	//--------------------------------------------------------------------------------------------------------------
	class Antenna : public GameObject {
		Vec3 m_position;
		Vec3 m_scale;
		Quat m_qt;
	public:
		Antenna(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat quat, Vec3 scale);
		virtual ~Antenna() {};
		virtual void OnCreate() override;
		virtual void OnDraw() override;
	};
	//--------------------------------------------------------------------------------------------------------------
	// 背景用のスプライトを作成
	//--------------------------------------------------------------------------------------------------------------
	class SkyBox : public GameObject
	{
	private:
		vector <shared_ptr<GameObject>> m_planes;
	public:
		SkyBox(const shared_ptr<Stage>& ptrStage);
		virtual ~SkyBox() {};

		virtual void OnCreate() override;
		virtual void OnUpdate2() override;
		virtual void OnDraw() override;
	};
	//--------------------------------------------------------------------------------------------------------------
	// メールオブジェクト
	//--------------------------------------------------------------------------------------------------------------
	class MailObject : public GameObject {
		Vec3 m_position;
		Quat m_quat;
		Vec3 m_scale;
		// 取得されたらtrue
		bool m_isArrive = false;
		float m_passageTime = 0.0f;
		float m_rot = 0.0f;
		float m_rotateSpeed = 2.0f;	// メールの回転速度
	public:
		MailObject(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		MailObject(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		virtual ~MailObject() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnDraw();
		void ArriveMail();
		void RotateMail();
		bool GetIsArrive() { return m_isArrive; }
		void SetIsArrive(bool isArrive) { m_isArrive = isArrive; }
	};
}
//end basecross