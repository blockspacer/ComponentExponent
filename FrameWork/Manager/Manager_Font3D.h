//=============================================================================
//
//  フォント3Dマネージャー [Manager_Font3D.h]
//  Date   : 2021/12/19
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../System/Singleton.h"
#include "Manager_Font.h"

class Manager_Font3D : public Manager_Font
{
private:

	struct s_CachePrint3DData
	{
		Vector3 pos;
		std::string* pText;
		Vector2 size;
	};
	std::unordered_map<const char*, s_CachePrint3DData>cacheList;

	float GetStringDif3D(const char* _text, float _polySizeX);
	float GetSpaceFontSize3D(const char* _fontPathName, const char* _fontDataName, bool isMini);

public:

	void Init() override;  //初期化
	void Uninit() override;//終了
	void Draw() override;

	//文字列のポインタを登録して外から文字列を操作
	void AddPrintData(Vector3 _pos, Vector2 _size, std::string& _text, const char* _key);

	//書式指定子ありの文字描画
	void Printf3D(Vector3 _pos, Vector2 _size, const char* _text, ...);

	//書式指定子のないprintf。書式指定子を使わない場合はこっちを使ったほうが良い。
	void PrintfStatic3D(Vector3 _pos, Vector2 _size, const char* _text);
};