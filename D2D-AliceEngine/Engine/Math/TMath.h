#pragma once

#include <math.h>
#include <random>
#include <Define/Define.h>

/*
* 수학관련입니다. 
* nodiscard는 반환값을 강제로 사용하도록 하는 키워드입니다.
*/

template<typename T>
class TVector2
{
public:
	T x, y;

	TVector2() : x(0), y(0) {}
	TVector2(T _x, T _y) : x(_x), y(_y) {}
	TVector2(T _x) : x(_x), y(_x) {}

	TVector2(const TVector2<T>& left) { x = left.x; y = left.y; }
	TVector2(TVector2<T>& left) { x = left.x; y = left.y; }
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
TVector2<T> operator-(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template<typename T>
TVector2<T> operator+(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<typename T>
struct TBox2
{
public:
	TVector2<T> Min;
	TVector2<T> Max;

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

	TVector2<T> GetCenter()
	{
		return TVector2<T>((Min + Max) * 0.5f);
	}
};

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

		// 원 모양의 구간에 임의의 점을 반환하는 함수.
		[[nodiscard]] static TVector2<T> GetRandomPointInCircle2D(const T& centerX, const T& centerY, const T& radius)
		{
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<T>  angleRandom(0, 2 * static_cast<T>(Define::PI));
			std::uniform_real_distribution<T>  radiusRandom(1, radius*radius);

			T theta = angleRandom(gen);
			T newRadius = std::sqrt(radiusRandom(gen));
			return TVector2<T>(centerX + newRadius * std::cos(theta), centerY + newRadius * std::sin(theta));
		}
	};

	template <typename T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi) 
	{
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	// EaseInOut
	inline float EaseInOut(float a, float b, float elapsedTime, float duration) {
		// 보간 계수 t 계산
		float t = elapsedTime / duration;
		t = clamp(t, 0.0f, 1.0f); // 범위 제한

		// EaseInOut 적용 (SmoothStep)
		float smoothT = t * t * (3.0f - 2.0f * t);

		// 선형 보간
		return a + (b - a) * smoothT;
	}

	// EaseOut
	inline float EaseOut(float a, float b, float elapsedTime, float duration) {
		float t = elapsedTime / duration;
		t = clamp(t, 0.0f, 1.0f); // 범위 제한

		// EaseOut: 느려지는 곡선 (예: 1 - (1 - t)^2)
		float easeT = 1.0f - (1.0f - t) * (1.0f - t);

		return a + (b - a) * easeT;
	}

	// Approximately
	inline bool Approximately(float a, float b, float epsilon = 1e-5f) {
		return std::fabs(a - b) <= epsilon;
	}
}

using FVector2 = TVector2<float>;
using FBox = TBox2<float>;
using FRandom = Math::TRandom<float>;