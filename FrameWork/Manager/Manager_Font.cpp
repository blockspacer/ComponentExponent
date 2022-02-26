//=============================================================================
//
//  フォントマネージャー [Manager_Font.cpp]
//  Date   : 2021/12/19
//
//=============================================================================
#include <stdarg.h>
#include <codecvt>
#include "Manager_Font.h"
#include "../System/Exchanger.h"

constexpr float DEFAULT_FONT_SIZE(32.0f);
std::unordered_map<std::string, Manager_Font::s_FontTextureData*>Manager_Font::fontTextures;
bool Manager_Font::isInitialized = false;

void Manager_Font::Init()
{
	//継承する変数の初期化
	//※フォントの設定は文字をキャッシュする前に設定しておく必要がある
	{
		Dc = Renderer_DX11::Instance()->GetDeviceContext();
		Rp = Renderer_Polygon::Instance();
		Ms = Manager_Shader::Instance();

		aligningMode = e_Aligning::center;//真ん中揃え

		pathFontName = L"none";
		dataFontName = L"none";

		fontSize = DEFAULT_FONT_SIZE;
		fontColor = Color(1, 1, 1, 1);
		fontWeight = FW_NORMAL;
		isUseItalic = false;
	}

	//static変数の初期化は一度のみ
	if (!isInitialized)
	{

		//a～z
		const std::string engMini = { "abcdefghijklmnopqrstuvwxyz" };
		for (unsigned int i = 0; i < engMini.length(); i++)
		{
			TCHAR buf[3] = {};
			strcpy_s(buf, sizeof(buf), engMini.substr(i, 1).c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//A～Z
		const std::string engBig = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
		for (unsigned int i = 0; i < engBig.length(); i++)
		{
			TCHAR buf[3] = {};
			strcpy_s(buf, sizeof(buf), engBig.substr(i, 1).c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//0～9
		const std::string halfNum = { "0123456789" };
		for (unsigned int i = 0; i < halfNum.length(); i++)
		{
			TCHAR buf[3] = {};
			strcpy_s(buf, sizeof(buf), halfNum.substr(i, 1).c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//あ～ん
		const std::string hira = { "あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをん" };
		for (unsigned int i = 0; i < hira.length(); i += 2)
		{
			TCHAR buf[3] = {};
			std::string temp = hira.substr(i, 2);
			strcpy_s(buf, sizeof(buf), temp.c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//ア～ン
		const std::string kana = { "アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン" };
		for (unsigned int i = 0; i < kana.length(); i += 2)
		{
			TCHAR buf[3] = {};
			std::string temp = kana.substr(i, 2);
			strcpy_s(buf, sizeof(buf), temp.c_str());
			GetTextureData("none", "none", buf, fontSize);
		}
		isInitialized = true;
	}
}

void Manager_Font::Uninit()
{
	//キャッシュした全テクスチャをリリース。
	if (!fontTextures.empty())
	{
		auto itr = fontTextures.begin();
		for (; itr != fontTextures.end(); itr++)
		{
			if (itr->second->rv)
			{
				itr->second->rv->Release();
				itr->second->rv = nullptr;
			}
			delete itr->second;
			itr->second = nullptr;
		}
		fontTextures.clear();
	}

	isInitialized = false;
}

Manager_Font::s_FontTextureData * Manager_Font::CreateSingleCharFontTexture(const char * _fontPathName,
	const char * _fontDataName, TCHAR * _c, float _fontSize)
{
	ID3D11Device* Dv = Renderer_DX11::Instance()->GetDevice();

	LOGFONT lf = {// フォントハンドルの生成
		static_cast<LONG>(_fontSize),
		0,//文字セル、文字の高さ
		0,//平均文字幅
		0,//ベースラインとX軸との距離
		fontWeight,//フォントの太さ
		isUseItalic,//イタリックにするかどうか
		0,
		0,
		SHIFTJIS_CHARSET,//todo:フォントのキャラセットによって変える
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
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
		return nullptr;
	}

	char tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "/ROM/Font/%s.ttf", _fontPathName);
	std::wstring tempName2 = Exchanger::STRING_TO_WSTRING(tempName);
	//フォントロード
	DESIGNVECTOR design = {};
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
#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
	code = (UINT)*_c;
#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
	if (IsDBCSLeadByte(*_c))
		code = (BYTE)_c[0] << 8 | (BYTE)_c[1];
	else
		code = _c[0];
#endif

	// フォントビットマップ取得
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	BYTE* ptr = new BYTE[size];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);
	int Level = 17;

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);


	//--------------------------------
	// 書き込み可能テクスチャ作成
	//--------------------------------

	//フォントビットマップの横幅から4バイトアラインなビットマップ横幅を計算
	float fontWidth = ((GM.gmBlackBoxX + 3) / 4 * 4.0f);
	float fontHeight = static_cast<float>(GM.gmBlackBoxY);

	// テクスチャ作成
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = static_cast<UINT>(fontWidth);
	desc.Height = GM.gmBlackBoxY;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)タイプ
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;			// 動的（書き込みするための必須条件）
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// シェーダリソースとして使う
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUからアクセスして書き込みOK

	ID3D11Texture2D* tex2D = nullptr;
	hr = Dv->CreateTexture2D(&desc, 0, &tex2D);
	if (FAILED(hr))MessageBox(NULL, "フォントテクスチャ作成失敗", "ERROR", MB_OK);

	//テクスチャ情報構造体作成
	s_FontTextureData* fontTextureStruct = new s_FontTextureData();
	fontTextureStruct->nextCenterPoint = GM.gmCellIncX;
	fontTextureStruct->relativeLeftUpPosFromCenter = Vector2(static_cast<float>(GM.gmptGlyphOrigin.x), static_cast<float>(GM.gmptGlyphOrigin.y));

	static int testRandom = 0;

	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = Dc->Map(
		tex2D,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource);
	if (FAILED(hr))MessageBox(NULL, "テクスチャマップ作成失敗", "ERROR", MB_OK);
	// ここで書き込む
	BYTE* pBits = (BYTE*)hMappedResource.pData;

	// フォント情報の書き込み
	fontTextureStruct->fontSize = Vector2(fontWidth, fontHeight);
	fontTextureStruct->difY = ((float)TM.tmAscent - (float)GM.gmBlackBoxY) + ((float)GM.gmBlackBoxY - (float)GM.gmptGlyphOrigin.y);

	unsigned int x, y;
	DWORD Alpha, Color;
	DWORD testColor[4] = { 0xffff0000,0xff00ff00, 0xff0000ff, 0xffffffff };
	memset(pBits, 0, static_cast<size_t>(fontWidth * GM.gmBlackBoxY));
	for (y = 0; y < GM.gmBlackBoxY; y++)
	{
		for (x = 0; x < fontWidth; x++)
		{
			Alpha = (255 * ptr[((int)fontWidth / 4 * 4) * y + x]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);

			//Color = 0xffffffff;
			//Color = testColor[testRandom];

			memcpy((BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));//色情報書き込み
		}
	}

	Dc->Unmap(tex2D, 0);
	testRandom = (testRandom + 1) % 4;

	// テクスチャ情報を取得する
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc(&texDesc);

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// ShaderResourceViewを作成する
	hr = Dv->CreateShaderResourceView(tex2D, &srvDesc, &fontTextureStruct->rv);

	TCHAR tempFontDataName[256] = {};
	sprintf_s(tempFontDataName, sizeof(tempFontDataName), "%s_%s_itl%s_wg%d", _fontDataName, _c, Exchanger::BOOL_TO_STRING(isUseItalic).c_str(), fontWeight);
	fontTextures[tempFontDataName] = fontTextureStruct;

	delete[] ptr;
	if (tex2D)tex2D->Release();

	return fontTextureStruct;
}

float Manager_Font::GetSpaceFontSize(const char * _fontPathName, const char * _fontDataName, bool isMini)
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

	return GM.gmCellIncX;
}

Manager_Font::s_FontTextureData * Manager_Font::GetTextureData(const char * _fontDataName,
	const char * _fontPathName, TCHAR * _c, float _fontSize)
{
	//todo:サイズ違いをどうするか
	TCHAR tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "%s_%s_itl%s_wg%d", _fontDataName, _c, Exchanger::BOOL_TO_STRING(isUseItalic).c_str(), fontWeight);

	//テクスチャが存在していたらそれをRETURN
	if (fontTextures.count(tempName) > 0)
	{
		return fontTextures[tempName];
	}

	//テクスチャがなかったらキャッシュ
	return CreateSingleCharFontTexture(_fontPathName, _fontDataName, _c, _fontSize);
}

float Manager_Font::GetStringDif(std::string _text)
{
	//左揃えなら引く分が無いので計算前にreturn。
	if (aligningMode == e_Aligning::left)return 0.0f;

	int lengthOffset = 0;
	int start = 0;
	float dispTextLength = 0.0f;//表示する全文字の左端から右端までの距離
	float offsetX = 0.0f;

	//全角半角含めた文字数から左に引くoffsetを取得
	int i = 0;
	for (unsigned int byte = 0; byte < _text.length(); i++)
	{
		lengthOffset = 0;
		start = 0;

		//文字のバイト数判別
		if (IsDBCSLeadByte(_text[byte]))//2byte
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

		std::string temp = _text.substr(start, lengthOffset);

		//文字がスペースならテクスチャは作らずに、テクスチャサイズ分間隔を空ける
		if (temp == " " || temp == "　")
		{
			dispTextLength += GetSpaceFontSize(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ") * charLength;
			continue;
		}

		TCHAR buf[3] = {};
		//strcpy(buf, temp.c_str());
		strcpy_s(buf, sizeof(buf), temp.c_str());
		s_FontTextureData* tempTexture = GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);

		dispTextLength += tempTexture->nextCenterPoint * charLength;
	}

	//揃え方によって最初の文字を表示する場所まで引くサイズを決定
	if (aligningMode == e_Aligning::center)
	{
		return dispTextLength * 0.5f;
	}
	else//right
	{
		return dispTextLength;
	}
}

std::string Manager_Font::GetStringByArg(const char * _text, va_list _argList)
{
	std::string _ret;

	for (unsigned int i = 0; i < strlen(_text); ++i)
	{
		if (_text[i] == '%')
		{
			if (_text[i + 1] == 'd')
			{
				int temp = va_arg(_argList, int);
				_ret += Exchanger::INT_TO_STRING(temp);
				i++;
				continue;
			}
			else if (_text[i + 1] == 'f')
			{
				double temp = va_arg(_argList, double);
				_ret += Exchanger::FLOAT_TO_STRING(static_cast<float>(temp));
				i++;
				continue;
			}
			else if (_text[i + 1] == 'l' && _text[i + 2] == 'f')
			{
				double temp = va_arg(_argList, double);
				_ret += Exchanger::DOUBLE_TO_STRING(temp);
				i += 2;
				continue;
			}
			else if (_text[i + 1] == 'c')
			{
				char temp = va_arg(_argList, char);
				_ret += temp;

				i++;
				continue;
			}
			else if (_text[i + 1] == 's')
			{
				_ret += va_arg(_argList, const char*);
				i++;
				continue;
			}
		}
		_ret += _text[i];
	}

	return _ret;
}

