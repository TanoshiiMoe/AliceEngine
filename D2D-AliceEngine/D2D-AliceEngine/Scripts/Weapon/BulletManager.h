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
	void FireBullet(const FVector2& start, const FVector2& target, const FVector2& initVelocity);

	gameObject* GetPlayer() const { return m_player; }
	void SetPlayer(gameObject* player) { m_player = player; }

	gameObject* m_player = nullptr;
};

