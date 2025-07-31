#pragma once

#include <string>
#include <memory>
#include <Math/TMath.h>
#include <Core/Tickable.h>
#include <Object/UObject.h>

class gameObject;
class Canvas;
class Transform;
class TransformComponent;
class Component : public UObject, public ITickable
{
public:
	Component();
	Component(const std::wstring& name);
	~Component();
public:
	virtual void Initialize() override {}
	virtual void Update() override {}
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() override {}

	// Render가 필요없는 Component도 있음
	virtual void Render() {};

	virtual void Awake() {};
	virtual void OnStart() {}
	virtual void OnEnd() {}
	virtual void OnCreate() {}
	virtual void OnDestroy() {}

public:
	inline void SetOwner(WeakObjectPtr<gameObject> object) { owner = object; }
	inline gameObject* GetOwner() { return owner.Get(); }
	const std::wstring& GetOwnerName();
	Transform* GetTransform() const;
	TransformComponent* GetTransformComp() const;
	FVector2* GetPivot() const;

protected:
	WeakObjectPtr<gameObject> owner;

	//-- UI를 위한 것
public:
	void SetCanvas(WeakObjectPtr<Canvas> object);
	Canvas* GetCanvas();
	Transform* GetCanvasTransform();

protected:
	WeakObjectPtr<Canvas> canvas;
	//Transform relativeTransform;
};