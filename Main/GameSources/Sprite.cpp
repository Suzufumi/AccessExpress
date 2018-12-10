#include "stdafx.h"
#include "Project.h"

namespace basecross{
	void Sprite::OnCreate() {
		float sprite_w = _rect.right - _rect.left;
		float sprite_h = _rect.bottom - _rect.top;

		float half_w = sprite_w * 0.5f;
		float half_h = sprite_h * 0.5f;

		//float uv_w = sprite_w / _size.x; // 0.1f
		float start_x = _rect.left / _size.x;
		float end_x = _rect.right / _size.x;
		float start_y = _rect.top / _size.y;
		float end_y = _rect.bottom / _size.y;

		// 頂点(Vertex)の配列を作る
		vector<VertexPositionTexture> vertices = {
			{Vec3(-half_w,+half_h,0.0f),Vec2(start_x,start_y)},	// 0番目の頂点
			{Vec3(+half_w,+half_h,0.0f),Vec2(end_x,start_y)},	// 1番目の頂点
			{Vec3(-half_w,-half_h,0.0f),Vec2(start_x,end_y)},	// 2番目の頂点
			{Vec3(+half_w,-half_h,0.0f),Vec2(end_x,end_y)}	// 3番目の頂点
		};

		// 頂点インデックス(頂点の並び順)
		vector<uint16_t> indices = {
			0, 1, 2,	// 一つ目のポリゴン
			2, 1, 3		// 二つ目のポリゴン
		};
		// PT = PositionとTexture
		auto drawComp = AddComponent<PTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(_key);
		SetAlphaActive(true);	// 透明度を反映するかどうかのフラグ

		this->SetPosition(Vec2(0, 0));
	}
	void Sprite::SetMesh(const shared_ptr<MeshResource> mesh) {
		auto drawComp = GetComponent<PTSpriteDraw>();
		drawComp->SetMeshResource(mesh);
	}

	//------------------------------------------------------------------------------------
	/// fadeクラス
	//------------------------------------------------------------------------------------
	FadeInOut::FadeInOut(const shared_ptr<Stage>& stagePtr, const Vec2& pos)
		: Sprite(stagePtr, L"FADE_TX", Vec2(0, 0), Rect2D<float>(1.0f, 1.0f, 1.0f, 1.0f)), m_startPos(pos)
	{
	}

	FadeInOut::~FadeInOut()
	{}

	void FadeInOut::OnCreate()
	{
		Sprite::OnCreate();
		this->SetPosition(Vec2(0.0f, 0.0f));
		//GetComponent<PTSpriteDraw>()->SetDrawActive(false);
		auto ptrAction = AddComponent<Action>();
		auto width = App::GetApp()->GetGameWidth();
		auto height = App::GetApp()->GetGameHeight();
		ptrAction->AddMoveTo(1.0f, Vec3(width, height / 2, 0.0f));
		ptrAction->Run();
		ptrAction->SetLooped(true);
		SetDrawLayer(100);
	}

	void FadeInOut::OnUpdate()
	{
		auto ptrAction = AddComponent<Action>();
		if (m_isAction)
		{
			ptrAction->Run();
		}
		else
		{
			ptrAction->Stop();
		}
	}
}
