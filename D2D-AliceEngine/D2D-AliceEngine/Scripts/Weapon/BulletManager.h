#pragma once
#include <Core/Singleton.h>
#include <Math/TMath.h>
#include <Core/ObjectHandler.h>

class gameObject;
class Bullet;
class BulletManager : public Singleton<BulletManager>
{ 
public:
	BulletManager();
	virtual ~BulletManager();

	std::vector<WeakObjectPtr<Bullet>> bullets;
	void FireBullet(const FVector2& start, const FVector2& target);
};

