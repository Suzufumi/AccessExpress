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

		// ���_(Vertex)�̔z������
		vector<VertexPositionTexture> vertices = {
			{Vec3(-half_w,+half_h,0.0f),Vec2(start_x,start_y)},	// 0�Ԗڂ̒��_
			{Vec3(+half_w,+half_h,0.0f),Vec2(end_x,start_y)},	// 1�Ԗڂ̒��_
			{Vec3(-half_w,-half_h,0.0f),Vec2(start_x,end_y)},	// 2�Ԗڂ̒��_
			{Vec3(+half_w,-half_h,0.0f),Vec2(end_x,end_y)}	// 3�Ԗڂ̒��_
		};

		// ���_�C���f�b�N�X(���_�̕��я�)
		vector<uint16_t> indices = {
			0, 1, 2,	// ��ڂ̃|���S��
			2, 1, 3		// ��ڂ̃|���S��
		};
		// PT = Position��Texture
		auto drawComp = AddComponent<PTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(_key);
		SetAlphaActive(true);	// �����x�𔽉f���邩�ǂ����̃t���O

		this->SetPosition(Vec2(0, 0));

	}
}
