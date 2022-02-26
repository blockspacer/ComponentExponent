//=============================================================================
//
//  �t�H���g�}�l�[�W���[ [Manager_Font.cpp]
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
	//�p������ϐ��̏�����
	//���t�H���g�̐ݒ�͕������L���b�V������O�ɐݒ肵�Ă����K�v������
	{
		Dc = Renderer_DX11::Instance()->GetDeviceContext();
		Rp = Renderer_Polygon::Instance();
		Ms = Manager_Shader::Instance();

		aligningMode = e_Aligning::center;//�^�񒆑���

		pathFontName = L"none";
		dataFontName = L"none";

		fontSize = DEFAULT_FONT_SIZE;
		fontColor = Color(1, 1, 1, 1);
		fontWeight = FW_NORMAL;
		isUseItalic = false;
	}

	//static�ϐ��̏������͈�x�̂�
	if (!isInitialized)
	{

		//a�`z
		const std::string engMini = { "abcdefghijklmnopqrstuvwxyz" };
		for (unsigned int i = 0; i < engMini.length(); i++)
		{
			TCHAR buf[3] = {};
			strcpy_s(buf, sizeof(buf), engMini.substr(i, 1).c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//A�`Z
		const std::string engBig = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
		for (unsigned int i = 0; i < engBig.length(); i++)
		{
			TCHAR buf[3] = {};
			strcpy_s(buf, sizeof(buf), engBig.substr(i, 1).c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//0�`9
		const std::string halfNum = { "0123456789" };
		for (unsigned int i = 0; i < halfNum.length(); i++)
		{
			TCHAR buf[3] = {};
			strcpy_s(buf, sizeof(buf), halfNum.substr(i, 1).c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//���`��
		const std::string hira = { "�����������������������������������ĂƂȂɂʂ˂̂͂Ђӂւق܂݂ނ߂��������������" };
		for (unsigned int i = 0; i < hira.length(); i += 2)
		{
			TCHAR buf[3] = {};
			std::string temp = hira.substr(i, 2);
			strcpy_s(buf, sizeof(buf), temp.c_str());
			GetTextureData("none", "none", buf, fontSize);
		}

		//�A�`��
		const std::string kana = { "�A�C�E�G�I�J�L�N�P�R�T�V�X�Z�\�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}�~����������������������������" };
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
	//�L���b�V�������S�e�N�X�`���������[�X�B
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

	LOGFONT lf = {// �t�H���g�n���h���̐���
		static_cast<LONG>(_fontSize),
		0,//�����Z���A�����̍���
		0,//���ϕ�����
		0,//�x�[�X���C����X���Ƃ̋���
		fontWeight,//�t�H���g�̑���
		isUseItalic,//�C�^���b�N�ɂ��邩�ǂ���
		0,
		0,
		SHIFTJIS_CHARSET,//todo:�t�H���g�̃L�����Z�b�g�ɂ���ĕς���
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
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
		return nullptr;
	}

	char tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "/ROM/Font/%s.ttf", _fontPathName);
	std::wstring tempName2 = Exchanger::STRING_TO_WSTRING(tempName);
	//�t�H���g���[�h
	DESIGNVECTOR design = {};
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
#if _UNICODE
	// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ��ł�
	code = (UINT)*_c;
#else
	// �}���`�o�C�g�����̏ꍇ�A
	// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
	// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]�ł�
	if (IsDBCSLeadByte(*_c))
		code = (BYTE)_c[0] << 8 | (BYTE)_c[1];
	else
		code = _c[0];
#endif

	// �t�H���g�r�b�g�}�b�v�擾
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	BYTE* ptr = new BYTE[size];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);
	int Level = 17;

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);


	//--------------------------------
	// �������݉\�e�N�X�`���쐬
	//--------------------------------

	//�t�H���g�r�b�g�}�b�v�̉�������4�o�C�g�A���C���ȃr�b�g�}�b�v�������v�Z
	float fontWidth = ((GM.gmBlackBoxX + 3) / 4 * 4.0f);
	float fontHeight = static_cast<float>(GM.gmBlackBoxY);

	// �e�N�X�`���쐬
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = static_cast<UINT>(fontWidth);
	desc.Height = GM.gmBlackBoxY;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)�^�C�v
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;			// ���I�i�������݂��邽�߂̕K�{�����j
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_���\�[�X�Ƃ��Ďg��
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU����A�N�Z�X���ď�������OK

	ID3D11Texture2D* tex2D = nullptr;
	hr = Dv->CreateTexture2D(&desc, 0, &tex2D);
	if (FAILED(hr))MessageBox(NULL, "�t�H���g�e�N�X�`���쐬���s", "ERROR", MB_OK);

	//�e�N�X�`�����\���̍쐬
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
	if (FAILED(hr))MessageBox(NULL, "�e�N�X�`���}�b�v�쐬���s", "ERROR", MB_OK);
	// �����ŏ�������
	BYTE* pBits = (BYTE*)hMappedResource.pData;

	// �t�H���g���̏�������
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

			memcpy((BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));//�F��񏑂�����
		}
	}

	Dc->Unmap(tex2D, 0);
	testRandom = (testRandom + 1) % 4;

	// �e�N�X�`�������擾����
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc(&texDesc);

	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// ShaderResourceView���쐬����
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

	return GM.gmCellIncX;
}

Manager_Font::s_FontTextureData * Manager_Font::GetTextureData(const char * _fontDataName,
	const char * _fontPathName, TCHAR * _c, float _fontSize)
{
	//todo:�T�C�Y�Ⴂ���ǂ����邩
	TCHAR tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "%s_%s_itl%s_wg%d", _fontDataName, _c, Exchanger::BOOL_TO_STRING(isUseItalic).c_str(), fontWeight);

	//�e�N�X�`�������݂��Ă����炻���RETURN
	if (fontTextures.count(tempName) > 0)
	{
		return fontTextures[tempName];
	}

	//�e�N�X�`�����Ȃ�������L���b�V��
	return CreateSingleCharFontTexture(_fontPathName, _fontDataName, _c, _fontSize);
}

float Manager_Font::GetStringDif(std::string _text)
{
	//�������Ȃ�������������̂Ōv�Z�O��return�B
	if (aligningMode == e_Aligning::left)return 0.0f;

	int lengthOffset = 0;
	int start = 0;
	float dispTextLength = 0.0f;//�\������S�����̍��[����E�[�܂ł̋���
	float offsetX = 0.0f;

	//�S�p���p�܂߂����������獶�Ɉ���offset���擾
	int i = 0;
	for (unsigned int byte = 0; byte < _text.length(); i++)
	{
		lengthOffset = 0;
		start = 0;

		//�����̃o�C�g������
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

		//�������X�y�[�X�Ȃ�e�N�X�`���͍�炸�ɁA�e�N�X�`���T�C�Y���Ԋu���󂯂�
		if (temp == " " || temp == "�@")
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

	//�������ɂ���čŏ��̕�����\������ꏊ�܂ň����T�C�Y������
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

