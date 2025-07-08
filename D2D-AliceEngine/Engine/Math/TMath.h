#pragma once

#include <math.h>
#include <random>
#include <Define/Define.h>

namespace Math
{
	static std::random_device rd;
	
	template<typename T>
	struct TRandom
	{
		// 두 수 사이의 랜덤한 수를 반환. 반환값이 없으면 오류.
		[[nodiscard]] static float GetRandomInRange(T Min, T Max)
		{
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<T> dis(Min, Max);
			return dis(gen);
		}

		// 도넛 모양의 구간에 임의의 점을 반환하는 함수.
		[[nodiscard]] static TVector2<T> GetRandomPointInTorus2D(const T& centerX, const T& centerY, const T& innerRadius, const T& outerRadius)
		{
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<T>  angleRandom(0, 2 * static_cast<T>(Define::PI));
			std::uniform_real_distribution<T>  radiusRandom(innerRadius * innerRadius, outerRadius * outerRadius);

			T theta = angleRandom(gen);
			T newRadius = std::sqrt(radiusRandom(gen));
			return TVector2<T>(centerX + newRadius * std::cos(theta), centerY + newRadius * std::sin(theta));
		}
	};

	template<typename T>
	class TVector2
	{
	public:
		T x, y;

		TVector2() : x(0), y(0) {}
		TVector2(T _x, T _y) : x(_x), y(_y) {}
		TVector2(T _x) : x(_x), y(_x) {}

		TVector2(const TVector2<T>& left) { x = left.x; y = left.y; }
		~TVector2() {}

		TVector2 operator*(const float& value)
		{
			return TVector2(x * value, y * value);
		}

		TVector2 operator/(const float& value)
		{
			return TVector2(x / value, y / value);
		}

		TVector2 operator+(const TVector2& rhs)
		{
			return TVector2(x + rhs.x, y + rhs.y);
		}
		TVector2 operator-(const TVector2& rhs)
		{
			return TVector2(x - rhs.x, y - rhs.y);
		}
		bool operator!=(const TVector2& rhs)
		{
			return x != rhs.x && y != rhs.y;
		}
		bool operator==(const TVector2& rhs)
		{
			return x == rhs.x && y == rhs.y;
		}
		void operator*=(const TVector2& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
		}
		void operator+=(const TVector2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
		}
		void operator-=(const TVector2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		}
		void operator=(const TVector2& rhs)
		{
			x = rhs.x;
			y = rhs.y;
		}

		bool IsZero()
		{
			return x == 0 && y == 0;
		}

		float Dot(const TVector2& v1)
		{
			return v1.x * x + v1.y * y;
		}

		TVector2 Normalize()
		{
			return TVector2(x / Length(), y / Length());
		}

		float Length()
		{
			return sqrt(LengthSQ());
		}

		float LengthSQ()
		{
			return x * x + y * y;
		}
	};


	template<typename T>
	struct TBox2
	{
	public:
		TVector<T> Min;
		TVector<T> Max;

		TBox2<T>()
		{

		}

		TBox2<T>(const T& minx, const T& miny, const T& maxx, const T& maxy)
		{
			Min.x = minx;
			Min.y = miny;
			Max.x = maxx;
			Max.y = maxy;
		}

		TBox2<T>(const TBox2<T>& inbox)
		{
			Min = inbox.Min;
			Max = inbox.Max;
		}

		TVector<T> GetCenter()
		{
			return TVector<T>((Min + Max) * 0.5f);
		}

	};
}

using FVector2 = Math::TVector2<float>;
using FRandom = Math::TRandom<float>;
using FBox = Math::TBox2<float>;