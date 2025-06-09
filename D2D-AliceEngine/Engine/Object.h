#pragma once
#include "pch.h"
#include "Transform.h"
#include "BitmapImage.h"

class Object
{
public:
	Object() 
	{
		m_localTransform = new Transform();
	}
	virtual ~Object() 
	{
		m_localTransform = nullptr;
	}

	virtual void Initialize();
	virtual void LoadData();
	virtual void Release();

	DoubleLinkedList<Transform*> childList;
	Transform* m_localTransform; // Transform ÄÄÆ÷³ÍÆ®
	Transform* m_worldTransform; // Transform ÄÄÆ÷³ÍÆ®
	BitmapImage m_bitmapImage; // BitmapImage ÄÄÆ÷³ÍÆ®
};

