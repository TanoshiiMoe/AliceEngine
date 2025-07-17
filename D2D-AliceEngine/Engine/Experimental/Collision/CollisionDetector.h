#pragma once
#include <Core/ObjectHandler.h>
#include <Experimental/Physics/AABB.h>
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

namespace Physics
{
	// �浹 �˻� ����ƽ �Լ����� ���ִ� Ŭ����.
	class FCollisionDetector
	{
	public:
		FCollisionDetector() {};
		~FCollisionDetector() {};

		static void BruteForceOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects);
		static std::unordered_set<Rigidbody2D*> SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects);
		static bool CompareColliderMinX(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b);
		static bool IsOverlapped(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b);
		static void PushOverlappedArea(Collider* a, Collider* b);
		static void PushOverlappedAreaNoMass(Collider* a, Collider* b);
		static bool LineAABBIntersect(const FVector2& p0, const FVector2& p1, const FAABB& aabb, FVector2& outHitPos);

		static float CalcXOverlap(const FAABB& a, const FAABB& b);
		// y�� ��ħ ���� ���
		static float CalcYOverlap(const FAABB& a, const FAABB& b);
		// aabb ��ü width
		static float Width(const FAABB& box);
		// Ground ���� ����
		static bool IsOnGround(const FAABB& above, const FAABB& below, float threshold = 0.3f);
	};

	struct NodeEntry
	{
		UObject* obj;
		NodeEntry* prev;
		NodeEntry* next;

		NodeEntry()
		{
			prev = nullptr;
			next = nullptr;
			obj = nullptr;
		}
	};

	struct Rectangle
	{
		float x, y, length;
	};

	//class QuadTree
	//{
	//public:
	//	QuadTree();
	//	~QuadTree();
	//
	//	UObject* head;
	//	UINT nodeID = -1;
	//	//  ------------------- 
	//	//  |        |        |
	//	//  |    1   |   2    |
	//	//  |--------|--------|
	//	//  |    3   |   4    |
	//	//  |        |        |
	//	//  ------------------- 
	//	QuadTree* child[2][2];
	//	Rectangle rect;
	//
	//	static void MakeQuadTree(QuadTree* tree, int& nodeID, const int& level);
	//	static void FindNode(QuadTree* tree, const int& level, UObject** Node, const FVector3& minVector, const FVector3& maxVector);
	//	static void UpdateNode(QuadTree* tree, const int& level, UObject* object, const FVector3& minVector, const FVector3& maxVector);
	//	static void ClearNode(QuadTree* tree, const int& level);
	//};
}

