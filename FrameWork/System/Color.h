//=============================================================================
//
//  色定義　[Color.h]
//  Date   : 2021/11/20
//
//=============================================================================
#pragma once

#include "../System/Main.h"

namespace ColorSpace
{
	Vector4 White = Vector4(1.0f, 1.0f, 1.0f, 1.0f);//白
	Vector4 Black = Vector4(0.0f, 0.0f, 0.0f, 1.0f);//黒

	Vector4 Red   = Vector4(1.0f, 0.0f, 0.0f, 1.0f);//赤
	Vector4 Green = Vector4(0.0f, 1.0f, 0.0f, 1.0f);//緑
	Vector4 Blue  = Vector4(0.0f, 0.0f, 1.0f, 1.0f);//青

	Vector4 Cyan    = Vector4(0.0f, 1.0f, 1.0f, 1.0f);//シアン
	Vector4 Magenta = Vector4(1.0f, 0.0f, 1.0f, 1.0f);//マゼンタ
	Vector4 Yellow  = Vector4(1.0f, 1.0f, 0.0f, 1.0f);//イエロー

	Vector4 Purple  = Vector4(0.5f, 0.0f, 0.5f, 1.0f);//パープル
}