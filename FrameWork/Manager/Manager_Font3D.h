//=============================================================================
//
//  �t�H���g3D�}�l�[�W���[ [Manager_Font3D.h]
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

	void Init() override;  //������
	void Uninit() override;//�I��
	void Draw() override;

	//������̃|�C���^��o�^���ĊO���當����𑀍�
	void AddPrintData(Vector3 _pos, Vector2 _size, std::string& _text, const char* _key);

	//�����w��q����̕����`��
	void Printf3D(Vector3 _pos, Vector2 _size, const char* _text, ...);

	//�����w��q�̂Ȃ�printf�B�����w��q���g��Ȃ��ꍇ�͂��������g�����ق����ǂ��B
	void PrintfStatic3D(Vector3 _pos, Vector2 _size, const char* _text);
};