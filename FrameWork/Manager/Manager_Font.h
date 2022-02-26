//=============================================================================
//
//  �t�H���g�}�l�[�W���[ [Manager_Font.h]
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
		Vector2 relativeLeftUpPosFromCenter = { 0,0 };//���_���獶��܂ł̋���
		float difY = 0.0f;
	};

	//������̑�����
	enum e_Aligning
	{
		center,//������
		right,//�E����
		left,//������
		MAX
	};

private:

	//�t�H���g�̃e�N�X�`���L���b�V��
	static std::unordered_map<std::string, s_FontTextureData*>fontTextures;
	static bool isInitialized;

protected:

	//�L���b�V���ϐ�
	struct ID3D11DeviceContext* Dc;
	class Renderer_Polygon* Rp;
	class Manager_Shader* Ms;

	//�e�L�X�g�X�^�C���ϐ�
	bool isUseItalic;//�C�^���b�N�̂ɂ��邩
	int fontWeight;//�����̑���
	std::wstring pathFontName;//�t�H���g�̃p�X��̖��O
	std::wstring dataFontName;//�t�H���g�̃f�[�^�̖��O
	e_Aligning aligningMode;//�������ϐ�
	float fontSize;//�t�H���g�T�C�Y
	Color fontColor;//�t�H���g�J���[
	float charLength;//�����Ԃ̋���

	//1�������e�N�X�`��������ăL���b�V������֐�
	s_FontTextureData* CreateSingleCharFontTexture(const char* _fontName,
		const char* _fontDataName, TCHAR* _c, float _fontSize);

	//���p�A�S�p�̃X�y�[�X�p�̃e�N�X�`�����쐬����֐�
	float GetSpaceFontSize(const char* _fontPathName,
		const char* _fontDataName, bool isMini);

	//�L���b�V�������e�N�X�`�����擾
	//���L���b�V������Ă��Ȃ���������̂ŁAnull�͕Ԃ��Ȃ��B
	s_FontTextureData* GetTextureData(const char* _fontName,
		const char* _fontPathName, TCHAR* _c, float _fontSize);

	//�e�L�X�g�̍��[����E�[�܂ł̋������擾
	float GetStringDif(std::string _text);

	//�ψ������珑���w��̓��e��char�ɂ���string���擾
	std::string GetStringByArg(const char* _text, va_list _argList);

public:

	virtual ~Manager_Font() {};

	inline void SetFont(std::wstring _pathFontName, std::wstring _dataFontName)//�g�p����t�H���g��ݒ�
	{
		pathFontName = _pathFontName; 
		dataFontName = _dataFontName;
	}
	inline void SetAilgningMode(e_Aligning _mode) { aligningMode = _mode; }//���������Z�b�g
	inline void SetFontSize(float _size) { fontSize = _size; }//�t�H���g�T�C�Y�ݒ�
	inline void SetFontColor(Color _color) { fontColor = _color; }//�t�H���g�J���[�ݒ�
	inline void SetCharLength(float _set) { charLength = _set; }
	inline void SetFontWeight(int _set) { fontWeight = _set; }
	inline void SetIsUseItalic(bool _set) { isUseItalic = _set; }

	virtual void Init();
	virtual void Uninit();
	virtual void Draw() = 0;
};