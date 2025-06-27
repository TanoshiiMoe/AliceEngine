#pragma once

#include <math.h>
#include <random>

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

namespace Math
{
	static std::random_device rd;

	// 두 수 사이의 랜덤한 수를 반환. 반환값이 없으면 오류.
	template<typename T>
	struct TRandom
	{
		[[nodiscard]] static float GetRandomInRange(T Min, T Max)
		{
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<T> dis(Min, Max);
			return dis(gen);
		}
	};
}

using FVector2 = TVector2<float>;
using FRandom = Math::TRandom<float>;