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
		//this->SetPosition(Vec2(0, 0));

	}

	//------------------------------------------------------------------------------------
	/// fadeクラス
	//------------------------------------------------------------------------------------
	FadeInOut::FadeInOut(const shared_ptr<Stage>& stagePtr, const Vec2& pos)
		: Sprite(stagePtr, L"FADE_TX", Vec2(1280, 800), Rect2D<float>(1.0f, 1.0f, 1.0f, 1.0f)), m_startPos(pos)
	{
	}

	FadeInOut::~FadeInOut()
	{}

	void FadeInOut::OnCreate()
	{
		Sprite::OnCreate();
		this->SetPosition(m_startPos);
		//GetComponent<PTSpriteDraw>()->SetDrawActive(false);
		auto ptrAction = AddComponent<Action>();
		auto width = App::GetApp()->GetGameWidth();
		auto height = App::GetApp()->GetGameHeight();
		ptrAction->AddMoveTo(1.0f, Vec3(width, 0.0f, 0.0f));
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

	//--------------------------------------------------------------------------------------
///	アニメスプライト
//--------------------------------------------------------------------------------------
	AnimSprite::AnimSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0.0f),
		m_Selected(false)
	{}

	AnimSprite::~AnimSprite() {}

	void AnimSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertex = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertex, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void AnimSprite::OnUpdate() {
		if (m_Selected) {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime * 5.0f;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			col.w = sin(m_TotalTime) * 0.5f + 0.5f;
			PtrDraw->SetDiffuse(col);
		}
		else {
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			PtrDraw->SetDiffuse(col);
		}
	}

}
