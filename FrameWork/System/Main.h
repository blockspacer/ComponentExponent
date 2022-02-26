//=============================================================================
//
//  メイン　[Main.h]
//  Date   : 2021/11/02
//
//=============================================================================

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <iostream>
#include <algorithm>


#pragma warning(push)
#pragma warning(disable:4005)
#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>
#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "xinput.lib")

typedef D3DXVECTOR2 Vector2;//ベクター2
typedef D3DXVECTOR3 Vector3;//ベクター3
typedef D3DXVECTOR4 Vector4;//ベクター4
typedef D3DXCOLOR   Color;  //RGB+a

struct Int2
{
	int x = 0;
	int y = 0;

	inline Int2(int _x, int _y) { x = _x; y = _y; }
	inline Int2() { x = y = 0; }

	inline Int2 operator=(Int2 _arg) { x = _arg.x; y = _arg.y; return Int2(x, y); }
	inline Int2 operator+(Int2 _arg) { x += _arg.x; y += _arg.y; return Int2(x, y); }
	inline Int2 operator-(Int2 _arg) { x -= _arg.x; y -= _arg.y; return Int2(x, y); }
	inline Int2 operator*(Int2 _arg) { x *= _arg.x; y *= _arg.y; return Int2(x, y); }
	inline Int2 operator/(Int2 _arg)
	{ if (x != 0 && _arg.x != 0)x /= _arg.x; if (y != 0 && _arg.y != 0)y /= _arg.y; return Int2(x, y); }
};

constexpr int INVALID_ID = -1;     //エラーハンドル

const Vector2 Vector2_Zero = Vector2{0.0f,0.0f };
const Vector3 Vector3_Zero = Vector3(0.0f,0.0f,0.0f);
const Vector4 Vector4_Zero = Vector4(0.0f,0.0f,0.0f,0.0f);
