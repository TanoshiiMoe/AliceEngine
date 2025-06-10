#pragma once
class IRenderer
{
public:
	virtual void Initialize();
	virtual void Render();
	virtual void Release();
};

