//=============================================================================
//
//  �t�H���g2D�}�l�[�W���[ [Manager_Font2D.h]
//  Date   : 2021/12/19
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../../System/Singleton.h"
#include "../Manager_Font.h"

class Manager_Font2D : public Manager_Font, public Singleton<Manager_Font2D>
{
private:

	friend Singleton<Manager_Font2D>;
	struct s_CachePrint2DData
	{
		Vector2* pPos;
		std::string* pText;
	};
	std::unordered_map<const char*, s_CachePrint2DData>cacheList;

public:

	void Init() override;  //������
	void Uninit() override;//�I��
	void Draw() override;

	void DeleteText(const char* _key);

	void DeleteAllText();

	//�����w��q����̕����`��
	void Printf2D(Vector2 _pos, const char* _text, ...);

	//�����w��q�̂Ȃ�printf�B�����w��q���g��Ȃ��ꍇ�͂��������g�����ق����ǂ��B
	void PrintfStatic2D(Vector2 _pos, const char* _text);

	//������̃|�C���^��o�^���ĊO���當����𑀍�
	void AddPrintData(Vector2& _pos, std::string& _text, const char* _key);
};