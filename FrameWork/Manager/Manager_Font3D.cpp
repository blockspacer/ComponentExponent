//=============================================================================
//
//  �t�H���g3D�}�l�[�W���[ [Manager_Font3D.cpp]
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

		//�����̃o�C�g������
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

		//�������X�y�[�X�Ȃ�e�N�X�`���͍�炸�ɁA�e�N�X�`���T�C�Y���Ԋu���󂯂�
		if (temp == " " || temp == "�@")
		{
			float tempSpace = GetSpaceFontSize3D(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ");
			offsetX += tempSpace / fontSize;

			continue;
		}

		//�e�N�X�`���擾
		s_FontTextureData* tempTexture = GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);

		//�t�H���g�T�C�Y�̑傫�����̎����擾
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
	LOGFONT lf = {// �t�H���g�n���h���̐���
		static_cast<LONG>(fontSize),
		0,0,0,0,0,0,0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"none"
	};
	sprintf_s(lf.lfFaceName, sizeof(lf.lfFaceName), _fontDataName);//�t�H���g�����Z�b�g

	HFONT hFont = CreateFontIndirect(&lf);
	if (!(hFont))
	{
		char errorCode[256] = {};
		sprintf_s(errorCode, sizeof(errorCode), "%s - �t�H���g�n���h���쐬���s", _fontDataName);
		MessageBox(NULL, errorCode, "ERROR", MB_OK);
		return fontSize;
	}

	char tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "../ROM/Font/%s.ttf", _fontPathName);
	std::wstring tempName2 = Exchanger::STRING_TO_WSTRING(tempName);
	//�t�H���g���[�h
	DESIGNVECTOR design;
	HRESULT hr = AddFontResourceExW(
		tempName2.c_str(),
		FR_PRIVATE,
		&design
	);
	if (FAILED(hr))MessageBox(NULL, "�t�H���g���[�h�G���[", "ERROR", MB_OK);

	// �f�o�C�X�R���e�L�X�g�擾
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// �����R�[�h�擾
	UINT code = 0;
	TCHAR buf[3] = {};
	std::string tempSpaceString = isMini ? " " : "�@";
	strcpy_s(buf, sizeof(buf), tempSpaceString.c_str());

#if _UNICODE
	// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ��ł�
	code = (UINT)*_c;
#else
	// �}���`�o�C�g�����̏ꍇ�A
	// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
	// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]�ł�
	if (IsDBCSLeadByte(*buf))
		code = (BYTE)buf[0] << 8 | (BYTE)buf[1];
	else
		code = buf[0];
#endif

	// �t�H���g�r�b�g�}�b�v�擾
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, 0, NULL, &Mat);
	BYTE* ptr = new BYTE[size];
	GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, size, ptr, &Mat);

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return static_cast<float>(TM.tmAveCharWidth);
}

void Manager_Font3D::Init()
{
	//�e������
	Manager_Font::Init();
}

void Manager_Font3D::Uninit()
{
	//�e�I������
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
	va_start(list, _text);//�ψ����̏����w��q�̕�����string�ő�������������擾
	tempText = GetStringByArg(_text, list);
	va_end(list);

	//�|���S���`��
	Ms->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

	float offsetX = _pos.x;//�������o�͂���ꏊ��1�����ڂ����offset
	int lengthOffset = 0;
	int start = 0;

	const float textLenHalfSize = GetStringDif(tempText);

	//1�������e�N�X�`�����擾	- char++
	for (unsigned int byte = 0; byte < tempText.length();)
	{
		lengthOffset = 0;
		start = 0;

		//�����̃o�C�g������
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

		//�������X�y�[�X�Ȃ�e�N�X�`���͍�炸�ɁA�e�N�X�`���T�C�Y���Ԋu���󂯂�
		if (temp == " " || temp == "�@")
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

	//�|���S���`��
	Ms->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

	float offsetX = _pos.x;//�������o�͂���ꏊ��1�����ڂ����offset
	//1�������e�N�X�`�����擾	- char++
	for (unsigned int byte = 0, charNum = 0; byte < tempText.length(); charNum++)
	{
		int length = 0;
		int start = 0;

		//�����̃o�C�g������
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

		//�������X�y�[�X�Ȃ�e�N�X�`���͍�炸�ɁA�e�N�X�`���T�C�Y���Ԋu���󂯂�
		if (temp == " " || temp == "�@")
		{
			float tempSpace = GetSpaceFontSize3D(Exchanger::WSTRING_TO_STRING(pathFontName).c_str(),
				Exchanger::WSTRING_TO_STRING(dataFontName).c_str(), temp == " ");
			offsetX += tempSpace / fontSize;
			continue;
		}

		//�e�N�X�`���擾
		s_FontTextureData* tempTexture = GetTextureData(Exchanger::WSTRING_TO_STRING(dataFontName).c_str(),
			Exchanger::WSTRING_TO_STRING(pathFontName).c_str(), buf, fontSize);

		Dc->PSSetShaderResources(0, 1, &tempTexture->rv);//�e�N�X�`���Z�b�g

		//�t�H���g�T�C�Y�̑傫�����̎����擾
		float tempDivide = max(tempTexture->fontSize.x, tempTexture->fontSize.y);

		//�t�H���g�`��
		Rp->Draw3D(Vector3{ _pos.x + 1 * charNum,_pos.y,_pos.z },
			{ ((tempTexture->fontSize.x / tempDivide) * _size.x),(tempTexture->fontSize.y / tempDivide) * _size.y }, { 0,0,0 }, fontColor);

		//���̃|���S���̈ʒu
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
