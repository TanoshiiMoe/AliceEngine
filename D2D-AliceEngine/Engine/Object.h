#pragma once
#include "pch.h"
#include "Transform.h"
#include "BitmapImage.h"

class Object
{
public:
	Object() 
	{
		m_transform = new Transform();
	}
	virtual ~Object() 
	{
		m_transform = nullptr;
	}

	virtual void Initialize();
	virtual void LoadData();
	virtual void Release();

	DoubleLinkedList<Transform*> m_sceneGraph;
	Transform* m_transform; // Transform ÄÄÆ÷³ÍÆ®
	BitmapImage m_bitmapImage; // BitmapImage ÄÄÆ÷³ÍÆ®
};

