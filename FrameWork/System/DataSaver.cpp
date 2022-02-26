
#include "Main.h"
#include "DataSaver.h"

//todo:�g���q�܂Ŗ��O�Ɋ܂߂�K�v������
bool DataSaver::OpenFile(const char * _fileName, SAVEDATA_TYPE _type)
{
	if (IsOpenFile(_fileName))
	{
		char temp[512] = {};
		sprintf_s(temp, sizeof(temp), "�Z�[�u�Ăяo�����Ɂ@%s�@�t�@�C��������OPEN��Ԃł��B", _fileName);
		MessageBox(nullptr, temp, "ERROR", MB_OK);
		return false;
	}

	//�����t�@�C�����J���Ă��Ȃ��̂ŊJ��

	const char* mode[SAVEDATA_TYPE::MAX] = { "wt","rt","wb","rb" };
	const char* choose = mode[static_cast<int>(_type)];
	std::string tempFileName = SAVEDATA_PATH + std::string(_fileName);
	fopen_s(&fileList[_fileName], tempFileName.c_str(), choose);//�t�@�C���I�[�v��
	if (!fileList[_fileName])
	{
		char temp[512] = {};
		sprintf_s(temp, sizeof(temp), "%s�@�t�@�C�������݂��Ȃ����Aopen�`���ƃt�@�C���̌`�����Ⴂ�܂��B", tempFileName.c_str());
		MessageBox(nullptr, temp, "ERROR", MB_OK);
		fileList.erase(_fileName);
		return false;
	}

	return true;
}
