#include "pch.h"
#include "Object.h"

void Object::Initialize()
{
	if (m_localTransform == nullptr)
	{
		m_localTransform = new Transform();
	}
}

void Object::LoadData()
{
}

void Object::Release()
{
}
