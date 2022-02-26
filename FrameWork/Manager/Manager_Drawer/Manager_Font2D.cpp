//=============================================================================
//
//  フォント2Dマネージャー [Manager_Font2D.cpp]
//  Date   : 2021/11/28
//
//=============================================================================

#include "../../System/Main.h"
#include "../../System/Exchanger.h"
#include "Manager_Font2D.h"


void Manager_Font2D::Init()
{
	//親初期化
	Manager_Font::Init();


}

void Manager_Font2D::Uninit()
{
	//親終了処理
	Manager_Font::Uninit();

	cacheList.clear();
}

void Manager_Font2D::Draw()
{
	auto itr = cacheList.begin();
	for (; itr != cacheList.end(); itr++)
	{
		PrintfStatic2D(*itr->second.pPos, itr->second.pText->c_str());
	}
}

void Manager_Font2D::DeleteText(const char * _key)
{
	cacheList.erase(_key);
}

void Manager_Font2D::DeleteAllText()
{
	cacheList.clear();
}

void Manager_Font2D::Printf2D(Vector2 _pos, const char * _text, ...)
{
	float allStringLength = 0.0f;
	std::string tempText = "";
	va_list list;
	va_start(list, _text);//可変引数の書式指定子の部分をstringで足した文字列を取得
	tempText = GetStringByArg(_text, list);
	va_end(list);

	//シェーダーセット
	Ms->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

	float offsetX = _pos.x;//文字を出力する場所の1文字目からのoffset
	int lengthOffset = 0;
	int start = 0;

	const float textLenHalfSize = Manager_Font::GetStringDif(tempText);

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
			offsetX += Manager_Font::GetSpaceFontSize(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ");

			continue;
		}

		TCHAR buf[3] = {};
		strcpy(buf, temp.c_str());
		s_FontTextureData* tempTexture = Manager_Font::GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);
		Dc->PSSetShaderResources(0, 1, &tempTexture->rv);

		Rp->Draw2D(
			Vector2{ _pos.x + offsetX + tempTexture->fontSize.x * 0.5f + tempTexture->relativeLeftUpPosFromCenter.x - textLenHalfSize,
			_pos.y + tempTexture->fontSize.y * 0.5f + tempTexture->difY },
			{ tempTexture->fontSize.x,tempTexture->fontSize.y}, fontColor);

		offsetX += tempTexture->nextCenterPoint * charLength;
	}
}

void Manager_Font2D::PrintfStatic2D(Vector2 _pos, const char * _text)
{
	std::string tempText = _text;
	const float textLenHalfSize = GetStringDif(tempText);

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
		strcpy(buf, temp.c_str());

		//文字がスペースならテクスチャは作らずに、テクスチャサイズ分間隔を空ける
		if (temp == " " || temp == "　")
		{
			offsetX += GetSpaceFontSize(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ");

			continue;
		}

		s_FontTextureData* tempTexture = GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);

		Dc->PSSetShaderResources(0, 1, &tempTexture->rv);

		Rp->Draw2D(Vector2{ _pos.x + offsetX + tempTexture->fontSize.x * 0.5f + tempTexture->relativeLeftUpPosFromCenter.x - textLenHalfSize,
			_pos.y + tempTexture->fontSize.y * 0.5f + tempTexture->difY},
			{ tempTexture->fontSize.x,tempTexture->fontSize.y }, fontColor);

		offsetX += tempTexture->nextCenterPoint * charLength;
	}
}

void Manager_Font2D::AddPrintData(Vector2 & _pos, std::string & _text, const char * _key)
{
	if (cacheList.count(_key) == 0)
	{
		s_CachePrint2DData temp;
		temp.pPos = &_pos;
		temp.pText = &_text;
		cacheList[_key] = temp;
	}
}
