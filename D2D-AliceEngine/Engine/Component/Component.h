#pragma once

#include <string>
#include <memory>
#include <Math/TMath.h>
#include <Core/Tickable.h>
#include <Object/UObject.h>

class gameObject;
class Transform;
class Component : public UObject, public ITickable, public std::enable_shared_from_this<Component>
{
public:
	Component();
	Component(const std::wstring& name);
	~Component() {}
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
	inline void SetOwner(std::weak_ptr<gameObject> object) { owner = object.lock(); }
	inline gameObject* GetOwner() { return owner.lock().get(); }
	const std::wstring& GetOwnerName();
	Transform* GetTransform() const;
	FVector2* GetPivot() const;

protected:
	std::weak_ptr<gameObject> owner;
};