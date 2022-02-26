//=============================================================================
//
//  フォントマネージャー [Manager_Font.h]
//  Date   : 2021/12/19
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../System/Main.h"
#include "../System/Singleton.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Renderer/Renderer_Polygon.h"
#include "../Manager/Manager_Shader.h"


class Manager_Font
{
public:

	struct s_FontTextureData
	{
		ID3D11ShaderResourceView* rv = nullptr;
		Vector2 fontSize = { 0,0 };
		float nextCenterPoint = 0.0f;
		Vector2 relativeLeftUpPosFromCenter = { 0,0 };//原点から左上までの距離
		float difY = 0.0f;
	};

	//文字列の揃え方
	enum e_Aligning
	{
		center,//中揃え
		right,//右揃え
		left,//左揃え
		MAX
	};

private:

	//フォントのテクスチャキャッシュ
	static std::unordered_map<std::string, s_FontTextureData*>fontTextures;
	static bool isInitialized;

protected:

	//キャッシュ変数
	struct ID3D11DeviceContext* Dc;
	class Renderer_Polygon* Rp;
	class Manager_Shader* Ms;

	//テキストスタイル変数
	bool isUseItalic;//イタリック体にするか
	int fontWeight;//文字の太さ
	std::wstring pathFontName;//フォントのパス上の名前
	std::wstring dataFontName;//フォントのデータの名前
	e_Aligning aligningMode;//揃え方変数
	float fontSize;//フォントサイズ
	Color fontColor;//フォントカラー
	float charLength;//文字間の距離

	//1文字分テクスチャを作ってキャッシュする関数
	s_FontTextureData* CreateSingleCharFontTexture(const char* _fontName,
		const char* _fontDataName, TCHAR* _c, float _fontSize);

	//半角、全角のスペース用のテクスチャを作成する関数
	float GetSpaceFontSize(const char* _fontPathName,
		const char* _fontDataName, bool isMini);

	//キャッシュしたテクスチャを取得
	//※キャッシュされていなかったら作るので、nullは返さない。
	s_FontTextureData* GetTextureData(const char* _fontName,
		const char* _fontPathName, TCHAR* _c, float _fontSize);

	//テキストの左端から右端までの距離を取得
	float GetStringDif(std::string _text);

	//可変引数から書式指定の内容をcharにしたstringを取得
	std::string GetStringByArg(const char* _text, va_list _argList);

public:

	virtual ~Manager_Font() {};

	inline void SetFont(std::wstring _pathFontName, std::wstring _dataFontName)//使用するフォントを設定
	{
		pathFontName = _pathFontName; 
		dataFontName = _dataFontName;
	}
	inline void SetAilgningMode(e_Aligning _mode) { aligningMode = _mode; }//揃え方をセット
	inline void SetFontSize(float _size) { fontSize = _size; }//フォントサイズ設定
	inline void SetFontColor(Color _color) { fontColor = _color; }//フォントカラー設定
	inline void SetCharLength(float _set) { charLength = _set; }
	inline void SetFontWeight(int _set) { fontWeight = _set; }
	inline void SetIsUseItalic(bool _set) { isUseItalic = _set; }

	virtual void Init();
	virtual void Uninit();
	virtual void Draw() = 0;
};