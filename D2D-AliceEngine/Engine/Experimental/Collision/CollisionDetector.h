#pragma once
#include <Core/ObjectHandler.h>
#include <Experimental/Physics/AABB.h>
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>
#include <Component/BoxComponent.h>

/*
* @briefs : �浹 Ž���� ����ϴ� Ŭ�����Դϴ�
* @detail : ���Ʈ����, SweepAndPrune�� �����մϴ�. 
*/

namespace Physics
{
	// �浹 Ž�� Ŭ����
	class FCollisionDetector
	{
	public:
		FCollisionDetector() {};
		~FCollisionDetector() {};

		static void BruteForceOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects);
		static std::unordered_set<Rigidbody2D*> SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects);
		static bool CheckCollisionCondition(WeakObjectPtr<Collider>& _object);
		static void LoadPreviousCollisions();
		static bool CompareColliderMinX(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b);
		static bool IsOverlapped(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b);
		static void PushOverlappedArea(Collider* a, Collider* b);
		static void PushOverlappedAreaNoMass(Collider* a, Collider* b);
		static bool LineAABBIntersect(const FVector2& p0, const FVector2& p1, const FAABB& aabb, FVector2& outHitPos);

		static float CalcXOverlap(const FAABB& a, const FAABB& b);
		static float CalcYOverlap(const FAABB& a, const FAABB& b);
		static float Width(const FAABB& box);
		static bool IsOnGround(const FAABB& above, const FAABB& below, float threshold = 0.3f);
		static void SavePreviousCollisionData(Collider* src, Collider* tar, std::unordered_set<Rigidbody2D*>& overlappedRigdBodies);
	};
}

