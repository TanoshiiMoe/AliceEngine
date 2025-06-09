#pragma once

class Transform
{
public:
	D2D1_VECTOR_2F Translation;	// 위치
	float Rotation;				// 회전 (degree)
	D2D1_VECTOR_2F Scale;		// 크기

	Transform() : Translation{ 0.0f,0.0f }, Rotation(0.0f), Scale{ 1.0f,1.0f } {}

	D2D1_VECTOR_2F GetPosition() const
	{
		return Translation;
	}
	void SetPosition(const float& _x, const float& _y)
	{
		Translation.x = _x;
		Translation.y = _y;
	}
	float GetRotation() const
	{
		return Rotation;
	}
	void SetRotation(float rotation)
	{
		Rotation = rotation;
	}
	D2D1_VECTOR_2F GetScale() const
	{
		return Scale;
	}
	void SetScale(const float& scaleX, const float& scaleY)
	{
		Scale.x = scaleX;
		Scale.y = scaleY;
	}

	void SetTransform(const D2D1_VECTOR_2F& location, float rotation, const D2D1_VECTOR_2F& scale)
	{
		Translation = location;
		Rotation = rotation;
		Scale = scale;
	}

	D2D1::Matrix3x2F ToMatrix() const
	{
		D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(Scale.x, Scale.y, D2D1::Point2F(Translation.x, Translation.y));
		D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(Rotation);
		D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(Translation.x, Translation.y);
		return scale * rotation * translation;
	}
};

