#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------------------
	//player�ɂ�������Ray�̓͂��͈͂�������I�u�W�F�N�g
	//------------------------------------------------------------------------------------------------------------
	class RayRangeViewObj : public GameObject {
		float m_range;
	public:
		RayRangeViewObj(const shared_ptr<Stage>& StagePtr);
		virtual ~RayRangeViewObj() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void GetRange(float range) { m_range = range; };
	};

	//--------------------------------------------------------------------------------------
	//�����N�I�u�W�F�N�g�𑨂��邽�߂̏Ə�
	//--------------------------------------------------------------------------------------
	class SightingDevice : public GameObject {
		bool m_captureLink = false;
	public:
		SightingDevice(const shared_ptr<Stage>& stageptr);
		virtual ~SightingDevice() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void SetCaptureLink(bool f) { m_captureLink = f; }
		void ResetCaptureLink() { m_captureLink = false; }
	};
	//--------------------------------------------------------------------------------------
	//�`�F�C���̕����ƃo�[
	//--------------------------------------------------------------------------------------
	class ViewChainLetter : public Sprite {
		weak_ptr<Sprite> m_gage;
		weak_ptr<Sprite> m_gageFram;
		weak_ptr<GameObject> m_gageParent;
		float m_gageDefaltLength;
		float m_gageBounsLength = 0.1f;
		public:
			ViewChainLetter(const shared_ptr<Stage>& stagePtr);
			virtual ~ViewChainLetter() {};
			virtual void OnCreate() override;
			virtual void OnUpdate() override;
			//�Q�[�W�̈ʒu�A�傫�����ϓ�����
			void GageAct();
			//�Q�[�W�̕\����\���𐧌䂷��
			void ViewGage();
	};
	//--------------------------------------------------------------------------------------
	//�`�F�C���̐���
	//--------------------------------------------------------------------------------------
	class ViewChainNum : public GameObject {
	protected:
		int m_places;	// �\����
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		ViewChainNum(const shared_ptr<Stage>& stagePtr);
		virtual ~ViewChainNum() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void ViewNum();
	};
	//--------------------------------------------------------------------------------------
	//�X�R�A�ɔ��ł����`�F�C���̐���
	//--------------------------------------------------------------------------------------
	class FlyingChain : public GameObject {
	protected:
		int m_places;	// �\����
		int m_score;	// �\������X�R�A
		float m_wait;
		float m_leap;
		Vec3 p0 = Vec3(880, -380, 0);
		Vec3 p1 = Vec3(1000,-400,0);
		Vec3 p2 = Vec3(1152, 0, 0);

		bool m_isFly;	//���ł���Ƃ�true
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		FlyingChain(const shared_ptr<Stage>& stagePtr);
		virtual ~FlyingChain() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void ViewNum();
		void Fly();
		void FlySet(int chain);
	};
}