#pragma once

class SpriteCommon;

class Sprite {
public:
	void Initialize(SpriteCommon* spriteCommon);


private:
	SpriteCommon* spriteCommon_ = nullptr;

	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
};