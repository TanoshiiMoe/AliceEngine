#include "pch.h"
#include "Object.h"

void Object::Initialize()
{
	if (m_transform == nullptr)
	{
		m_transform = new Transform();
	}
}

void Object::LoadData()
{
}

void Object::Release()
{
}
