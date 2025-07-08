#pragma once


namespace Physics
{
	template<typename T>
	struct TAABB
	{
		FVector2 minVector = FVector2(FLT_MAX, FLT_MAX);
		FVector2 maxVector = FVector2(FLT_MIN, FLT_MIN);

		//float x, y;
		inline TAABB() : minVector(FLT_MAX), maxVector(FLT_MIN) {}
		inline TAABB(std::vector<FVector2>& _vertices)
		{
			for (const FVector2& vertex : _vertices)
			{
				minVector.x = min(minVector.x, vertex.x);
				minVector.y = min(minVector.y, vertex.y);
				maxVector.x = max(maxVector.x, vertex.x);
				maxVector.y = max(maxVector.y, vertex.y);
			}
		};
		// ToString 메서드 수정
		// 보여주려고 만든거.
		std::string ToString() const
		{
			std::ostringstream oss;
			oss << "minVector : ( X = " << minVector.x << ", Y = " << minVector.y << ", Z = " << minVector.z << " )\n";
			oss << "maxVector : ( X = " << maxVector.x << ", Y = " << maxVector.y << ", Z = " << maxVector.z << " )\n";
			return oss.str();
		}

		bool IsOverlap(const TAABB* aabb)
		{
			if (minVector.x > aabb->maxVector.x ||
				maxVector.x  < aabb->minVector.x ||
				minVector.y >  aabb->maxVector.y ||
				maxVector.y < aabb->minVector.y)
				return false;
			return true;
		}

		FVector2 GetSize()
		{
			return FVector2(maxVector.x - minVector.x, maxVector.y - minVector.y);
		}
	};
}
using FAABB = Physics::TAABB<float>;