#pragma once
#include "stdafx.h"

namespace basecross {
	class Sprite : public GameObject {
	protected:
		std::wstring _key;	// �e�N�X�`���̃L�[
		Vec2 _size;			// �X�v���C�g�̕��ƍ���
		Rect2D<float> _rect; // �e�N�X�`���͈̔�
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

		void SetScale(const Vec2& scale);

		void SetMesh(const shared_ptr<MeshResource> mesh);
	};

	//------------------------------------------------------------------------------------
	/// fade�N���X
	//------------------------------------------------------------------------------------
	class FadeInOut : public Sprite
	{
	private:
		Vec2 m_startPos;
		Vec2 m_startScale;
		const float m_fadeSpeed = 0.01f;
		float m_alpha;
		bool m_isFadeIn = false;
		bool m_isFadeOut = false;
	public:
		FadeInOut(const shared_ptr<Stage>& stagePtr, const Vec2& pos, const Vec2& scale);
		virtual ~FadeInOut();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void StartFadeIn();
		void StartFadeOut();
		bool GetIsFadeIn() const
		{ return m_isFadeIn; }
		void SetIsFadeIn(bool isFadeIn)
		{ m_isFadeIn = isFadeIn;}
		bool GetIsFadeOut() const
		{
			return m_isFadeOut;
		}
		void SetIsFadeOut(bool isFadeIn)
		{
			m_isFadeOut = isFadeIn;
		}
		void SetAlpha(float alpha)
		{ m_alpha = alpha;}
	};

	class AnimSprite : public GameObject
	{
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		//�g�[�^������
		float m_TotalTime;
		//�I������Ă��邩�ǂ���
		bool m_Selected;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	TextureKey	�e�N�X�`���L�[
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		AnimSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		//�j��
		virtual ~AnimSprite();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate()override;
		//�A�N�Z�T
		bool IsSelect() const {
			return m_Selected;
		}
		void SetSelect(bool b) {
			m_Selected = b;
			m_TotalTime = 0.0f;
		}
	};
	///---------------------------------------------------------------------------------
	//������\������X�v���C�g
	///---------------------------------------------------------------------------------
	class NumberSprite : public GameObject {
	protected:
		int m_places;	// �\����
		int m_num;
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		NumberSprite(const shared_ptr<Stage>& stagePtr,int places,int num);
		virtual ~NumberSprite() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void ChangeNum(int num) { m_num = num; };
	};

}
