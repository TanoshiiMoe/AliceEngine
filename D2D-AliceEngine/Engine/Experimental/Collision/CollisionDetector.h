#pragma once
#include <Core/ObjectHandler.h>
#include <Experimental/Physics/AABB.h>
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

namespace Physics
{
	// 충돌 검사 스태틱 함수들이 모여있는 클래스.
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

