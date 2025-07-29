#pragma once

#include "pch.h"
#include "Component/Component.h"

/*
* @brief Transform Ŭ����.
* @details 2D �������� ��ġ, ȸ��, ũ�⸦ �����մϴ�.
* 
* ToMatrix() : ��ġ, ȸ��, ũ�⸦ �ݿ��� ��Ʈ������ ��ȯ
* ToMatrix(const D2D1_POINT_2F& pivot) : �־��� �ǹ��� �������� ��ġ, ȸ��, ũ�⸦ �ݿ��� ��Ʈ������ ��ȯ
* SetFromMatrix(const D2D1::Matrix3x2F& mat) : ��Ʈ�������� ��ġ, ȸ��, ũ�⸦ �����Ͽ� ����
*/

class Transform
{
public:
	D2D1_VECTOR_2F Translation;	// ��ġ
	float Rotation;				// ȸ�� (degree)
	D2D1_VECTOR_2F Scale;		// ũ��

	D2D1::Matrix3x2F cachedMatrix = D2D1::Matrix3x2F::Identity();
	bool dirty = true;

	Transform() : Translation{ 0.0f, 0.0f }, Rotation(0.0f), Scale{ 1.0f,1.0f } {}
	~Transform() {}

	D2D1_VECTOR_2F GetPosition() const
	{
		return Translation;
	}
	void SetPosition(const float& _x, const float& _y)
	{
		Translation.x = _x;
		Translation.y = _y;
		dirty = true;
	}
	float GetRotation() const
	{
		return Rotation;
	}
	void SetRotation(float rotation)
	{
		Rotation = rotation;
		dirty = true;
	}
	D2D1_VECTOR_2F GetScale() const
	{
		return Scale;
	}
	void SetScale(const float& scaleX, const float& scaleY)
	{
		Scale.x = scaleX;
		Scale.y = scaleY;
		dirty = true;
	}

	void SetTransform(const D2D1_VECTOR_2F& location, float rotation, const D2D1_VECTOR_2F& scale)
	{
		Translation = location;
		Rotation = rotation;
		Scale = scale;
		dirty = true;
	}

	D2D1::Matrix3x2F ToMatrix()
	{
		if (dirty)
		{
			D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(Scale.x, Scale.y);
			D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(Rotation);
			D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(Translation.x, Translation.y);
			cachedMatrix =  scale * rotation * translation;
			dirty = false;
		}
		return cachedMatrix;
	}

	D2D1::Matrix3x2F ToMatrix(const D2D1_POINT_2F& pivot)
	{
		if (dirty)
		{
			auto scale = D2D1::Matrix3x2F::Scale(Scale.x, Scale.y, pivot);
			auto rotate = D2D1::Matrix3x2F::Rotation(Rotation, pivot);
			auto translate = D2D1::Matrix3x2F::Translation(Translation.x, Translation.y);
			cachedMatrix = scale * rotate * translate;
			dirty = false;
		}
		return cachedMatrix;
	}

	void SetFromMatrix(const D2D1::Matrix3x2F& mat)
	{
		// ��ġ
		Translation.x = mat.dx;
		Translation.y = mat.dy;

		// ������
		Scale.x = std::sqrt(mat.m11 * mat.m11 + mat.m12 * mat.m12);
		Scale.y = std::sqrt(mat.m21 * mat.m21 + mat.m22 * mat.m22);

		// ȸ�� (���� �� ��)
		float radians = std::atan2(mat.m12, mat.m11);
		Rotation = radians * (180.0f / Define::PI);

		cachedMatrix = mat;
	}

	D2D1::Matrix3x2F ToMatrixWithOutScale()
	{
		if (dirty)
		{
			D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(Rotation);
			D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(Translation.x, Translation.y);
			cachedMatrix = rotation * translation;
			dirty = false;
		}
		return cachedMatrix;
	}
};

