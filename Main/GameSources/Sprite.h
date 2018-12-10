#pragma once
#include "stdafx.h"

namespace basecross {
	class Sprite : public GameObject {
	protected:
		std::wstring _key;	// テクスチャのキー
		Vec2 _size;			// スプライトの幅と高さ
		Rect2D<float> _rect; // テクスチャの範囲
	public:
		Sprite(const std::shared_ptr<Stage>& stage, const std::wstring& key, Vec2 size)
			: GameObject(stage), _key(key), _size(size), _rect(0, 0, size.x, size.y)
		{
		}

		Sprite(const std::shared_ptr<Stage>& stage, const std::wstring& key, Vec2 size, const Rect2D<float>& rect)
			:GameObject(stage), _key(key), _size(size), _rect(rect)
		{
		}

		virtual void OnCreate() override;

		void SetPosition(const Vec2& position) {
			auto& app = App::GetApp();
			Vec2 scrSize((float)app->GetGameWidth(), (float)app->GetGameHeight());
			Vec2 scrHalf = scrSize * 0.5f;

			auto transComp = GetComponent<Transform>();
			Vec2 pos = position - scrHalf;
			transComp->SetPosition(pos.x, -pos.y, 0);
		};

		void SetMesh(const shared_ptr<MeshResource> mesh);
	};

	//------------------------------------------------------------------------------------
	/// fadeクラス
	//------------------------------------------------------------------------------------
	class FadeInOut : public Sprite
	{
	private:
		Vec2 m_startPos;
		Vec2 m_startScale;
		bool m_isAction = false;
	public:
		FadeInOut(const shared_ptr<Stage>& stagePtr, const Vec2& pos);
		virtual ~FadeInOut();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		bool GetIsAction() const
		{ return m_isAction; }
		void SetIsAction(bool isAction)
		{ m_isAction = isAction;}
	};
}
