//=============================================================================
//
//  汎用計算　[Math.h]
//  Date   : 2021/11/20
//
//=============================================================================
#pragma once

#include <cmath>
#include "Main.h"
#include "Singleton.h"

#define HalfSize(size) ((size) * 0.5f) //半径取得
#define Size75(size)   ((size) * 0.75f)//75%の値を取得
#define DegToRad(deg)  ((deg)  * Pi / 180)//度数に変換

constexpr float Pi = 3.141592f;//円周率

namespace Math
{
	//線形補間関数
	inline constexpr float lerpf(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	inline constexpr double lerpl(double a, double b, double t, double timeMax)
	{
		return a + t * ((b - a) / timeMax);
	}

	//クランプ関数
	inline constexpr float clamp(float num, float min, float max)
	{
		if (num < min)
		{
			return min;
		}
		else if (num > max)
		{
			return max;
		}
		return num;
	}

	inline Vector3 Vec3MultipleByVec3(Vector3 _A, Vector3 _B)
	{
		Vector3 ret = Vector3_Zero;
		ret.x = _A.x * _B.x;
		ret.y = _A.y * _B.y;
		ret.z = _A.z * _B.z;
		return ret;
	}

	inline Vector3 Vec3DividedByVec3(Vector3 _A, Vector3 _B)
	{
		Vector3 ret = Vector3_Zero;
		ret.x = _A.x / _B.x;
		ret.y = _A.y / _B.y;
		ret.z = _A.z / _B.z;
		return ret;
	}
}



