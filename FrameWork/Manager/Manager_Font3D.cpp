//=============================================================================
//
//  フォント3Dマネージャー [Manager_Font3D.cpp]
//  Date   : 2021/12/19
//
//=============================================================================

#include "Manager_Font3D.h"
#include "../System/Exchanger.h"


float Manager_Font3D::GetStringDif3D(const char * _text,float _polySizeX)
{
	if (aligningMode == e_Aligning::left)return 0.0f;

	std::string tempText = _text;
	float offsetX = 0.0f;

	for (unsigned int byte = 0, charNum = 0; byte < tempText.length(); charNum++)
	{
		int length = 0;
		int start = 0;

		//文字のバイト数判別
		if (IsDBCSLeadByte(tempText[byte]))//2byte
		{
			start = byte;
			byte += 2;
			length = 2;
		}
		else//1byte
		{
			start = byte;
			byte++;
			length = 1;
		}

		TCHAR buf[3] = {};
		std::string temp = tempText.substr(start, length);
		strcpy_s(buf, sizeof(buf), temp.c_str());

		//文字がスペースならテクスチャは作らずに、テクスチャサイズ分間隔を空ける
		if (temp == " " || temp == "　")
		{
			float tempSpace = GetSpaceFontSize3D(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ");
			offsetX += tempSpace / fontSize;

			continue;
		}

		//テクスチャ取得
		s_FontTextureData* tempTexture = GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);

		//フォントサイズの大きい方の軸を取得
		float tempDivide = max(tempTexture->fontSize.x, tempTexture->fontSize.y);

		offsetX += ((tempTexture->fontSize.x / tempDivide));
	}

	if (aligningMode == e_Aligning::center)
	{
		return offsetX * 0.5f;
	}

	return offsetX;
}

float Manager_Font3D::GetSpaceFontSize3D(const char * _fontPathName, const char * _fontDataName, bool isMini)
{
	LOGFONT lf = {// フォントハンドルの生成
		static_cast<LONG>(fontSize),
		0,0,0,0,0,0,0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"none"
	};
	sprintf_s(lf.lfFaceName, sizeof(lf.lfFaceName), _fontDataName);//フォント名をセット

	HFONT hFont = CreateFontIndirect(&lf);
	if (!(hFont))
	{
		char errorCode[256] = {};
		sprintf_s(errorCode, sizeof(errorCode), "%s - フォントハンドル作成失敗", _fontDataName);
		MessageBox(NULL, errorCode, "ERROR", MB_OK);
		return fontSize;
	}

	char tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "../ROM/Font/%s.ttf", _fontPathName);
	std::wstring tempName2 = Exchanger::STRING_TO_WSTRING(tempName);
	//フォントロード
	DESIGNVECTOR design;
	HRESULT hr = AddFontResourceExW(
		tempName2.c_str(),
		FR_PRIVATE,
		&design
	);
	if (FAILED(hr))MessageBox(NULL, "フォントロードエラー", "ERROR", MB_OK);

	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// 文字コード取得
	UINT code = 0;
	TCHAR buf[3] = {};
	std::string tempSpaceString = isMini ? " " : "　";
	strcpy_s(buf, sizeof(buf), tempSpaceString.c_str());

#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
	code = (UINT)*_c;
#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
	if (IsDBCSLeadByte(*buf))
		code = (BYTE)buf[0] << 8 | (BYTE)buf[1];
	else
		code = buf[0];
#endif

	// フォントビットマップ取得
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, 0, NULL, &Mat);
	BYTE* ptr = new BYTE[size];
	GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, size, ptr, &Mat);

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return static_cast<float>(TM.tmAveCharWidth);
}

void Manager_Font3D::Init()
{
	//親初期化
	Manager_Font::Init();
}

void Manager_Font3D::Uninit()
{
	//親終了処理
	Manager_Font::Uninit();

	cacheList.clear();
}

void Manager_Font3D::Draw()
{
	auto itr = cacheList.begin();
	for (; itr != cacheList.end(); itr++)
	{
		PrintfStatic3D(itr->second.pos, itr->second.size, itr->second.pText->c_str());
	}
}

void Manager_Font3D::Printf3D(Vector3 _pos, Vector2 _size, const char * _text, ...)
{
	float allStringLength = 0.0f;
	std::string tempText = "";
	va_list list;
	va_start(list, _text);//可変引数の書式指定子の部分をstringで足した文字列を取得
	tempText = GetStringByArg(_text, list);
	va_end(list);

	//ポリゴン描画
	Ms->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

	float offsetX = _pos.x;//文字を出力する場所の1文字目からのoffset
	int lengthOffset = 0;
	int start = 0;

	const float textLenHalfSize = GetStringDif(tempText);

	//1文字ずつテクスチャを取得	- char++
	for (unsigned int byte = 0; byte < tempText.length();)
	{
		lengthOffset = 0;
		start = 0;

		//文字のバイト数判別
		if (IsDBCSLeadByte(tempText[byte]))//2byte
		{
			start = byte;
			byte += 2;
			lengthOffset = 2;
		}
		else//1byte
		{
			start = byte;
			byte++;
			lengthOffset = 1;
		}

		std::string temp = tempText.substr(start, lengthOffset);

		//文字がスペースならテクスチャは作らずに、テクスチャサイズ分間隔を空ける
		if (temp == " " || temp == "　")
		{
			float tempSpace = GetSpaceFontSize3D(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ");
			offsetX += tempSpace / fontSize;

			continue;
		}

		TCHAR buf[3] = {};
		//strcpy(buf, temp.c_str());
		strcpy_s(buf, sizeof(buf), temp.c_str());
		s_FontTextureData* tempTexture = GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);
		Dc->PSSetShaderResources(0, 1, &tempTexture->rv);

		Rp->Draw3D(
			Vector3{ _pos.x + offsetX + tempTexture->fontSize.x * 0.5f + tempTexture->relativeLeftUpPosFromCenter.x - textLenHalfSize,
			_pos.y + tempTexture->fontSize.y * 0.5f + tempTexture->difY,_pos.z },
			{ 1.0f / tempTexture->fontSize.x, 1.0f / tempTexture->fontSize.y }, { 0,0,0 }, fontColor);

		offsetX += tempTexture->nextCenterPoint * charLength;
	}
}

void Manager_Font3D::PrintfStatic3D(Vector3 _pos, Vector2 _size, const char * _text)
{
	std::string tempText = _text;
	const float textLenHalfSize = GetStringDif3D(tempText.c_str(), _size.x);

	//ポリゴン描画
	Ms->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

	float offsetX = _pos.x;//文字を出力する場所の1文字目からのoffset
	//1文字ずつテクスチャを取得	- char++
	for (unsigned int byte = 0, charNum = 0; byte < tempText.length(); charNum++)
	{
		int length = 0;
		int start = 0;

		//文字のバイト数判別
		if (IsDBCSLeadByte(tempText[byte]))//2byte
		{
			start = byte;
			byte += 2;
			length = 2;
		}
		else//1byte
		{
			start = byte;
			byte++;
			length = 1;
		}

		TCHAR buf[3] = {};
		std::string temp = tempText.substr(start, length);
		strcpy_s(buf, sizeof(buf), temp.c_str());

		//文字がスペースならテクスチャは作らずに、テクスチャサイズ分間隔を空ける
		if (temp == " " || temp == "　")
		{
			float tempSpace = GetSpaceFontSize3D(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ");
			offsetX += tempSpace / fontSize;
			continue;
		}

		//テクスチャ取得
		s_FontTextureData* tempTexture = GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);

		Dc->PSSetShaderResources(0, 1, &tempTexture->rv);//テクスチャセット

		//フォントサイズの大きい方の軸を取得
		float tempDivide = max(tempTexture->fontSize.x, tempTexture->fontSize.y);

		//フォント描画
		Rp->Draw3D(Vector3{ _pos.x + 1 * charNum,_pos.y,_pos.z },
			{ ((tempTexture->fontSize.x / tempDivide) * _size.x),(tempTexture->fontSize.y / tempDivide) * _size.y }, { 0,0,0 }, fontColor);

		//次のポリゴンの位置
		offsetX += (tempTexture->fontSize.x / tempDivide) * _size.x;
	}
}

void Manager_Font3D::AddPrintData(Vector3 _pos, Vector2 _size, std::string & _text, const char * _key)
{
	if (cacheList.count(_key) == 0)
	{
		s_CachePrint3DData temp;
		temp.pos = _pos;
		temp.size = _size;
		temp.pText = &_text;
		cacheList[_key] = temp;
	}
}
