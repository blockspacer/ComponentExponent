#pragma once

#include <unordered_map>
#include <stdio.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../System/Singleton.h"


class DataSaver : public Singleton<DataSaver>
{
private:

	friend Singleton<DataSaver>;
	const char* SAVEDATA_PATH = { "SaveData/" };
	const int TEMP_DATA_NAME_LEN_MAX = 256;//�ꎞ�f�[�^���̍ő啶����
	const size_t SAVEDATA_START_NAME_SIZE = sizeof(char) * TEMP_DATA_NAME_LEN_MAX;//�ꎞ�f�[�^���̃T�C�Y

	std::unordered_map<const char*, FILE*>fileList;//�J���Ă���t�@�C�����X�g


public:

	enum SAVEDATA_TYPE
	{
		write_txt,
		read_txt,

		write_bin,
		read_bin,

		MAX
	};

	//�t�@�C�����J���Ă��邩�m�F���� open = true
	inline bool IsOpenFile(const char* _fileName)
	{
		return fileList.count(_fileName) > 0;
	}

	//�t�@�C�����I�[�v�����邾���B
	bool OpenFile(const char* _fileName, SAVEDATA_TYPE _type);

	//�t�@�C������邾���B
	void CloseFile(const char* _fileName)
	{
		if (fileList.count(_fileName) <= 0)return;
		fclose(fileList[_fileName]);
		fileList.erase(_fileName);
	}

	//�S�Ẵt�@�C�������
	void CloseAllFiles()
	{
		for (auto& i : fileList)
		{
			fclose(i.second);
		}
		fileList.clear();
	}

	//��̃f�[�^���Z�[�u
	template <typename T>
	inline bool Save(const char* _fileName, T* _data, size_t _dataSize, const char* _dataName)
	{
		if (fileList.count(_fileName) <= 0)return false;//�t�@�C�����J���ĂȂ�

		fwrite(_dataName, SAVEDATA_START_NAME_SIZE, 1, fileList[_fileName]);//�f�[�^����������
		fwrite(&_dataSize, sizeof(size_t), 1, fileList[_fileName]);//�f�[�^�T�C�Y��������
		fwrite(_data, _dataSize, 1, fileList[_fileName]);//��������

		return true;
	}

	//��̃f�[�^�����[�h
	template <typename T>
	inline bool Load(const char* _fileName, T* _out, const char* _dataName)
	{
		if (fileList.count(_fileName) <= 0)return false;//�t�@�C�����J���ĂȂ�
		if (!fileList.count(_fileName))return false;//�t�@�C�����J���ĂȂ�

		//�t�@�C���̐擪�܂ŃV�[�N
		if (fseek(fileList[_fileName], 0, SEEK_SET) != 0) 
		{
			MessageBox(nullptr, "���[�h���V�[�N�G���[", "ERROR", MB_OK);
			return false;
		}

		//EOF�܂Ŗ��O����v����܂Ō���
		while (feof(fileList[_fileName]) != EOF || ftell(fileList[_fileName]) != EOF)
		{
			size_t tempSize = {};
			char tempName[512] = {};

			fread(tempName, SAVEDATA_START_NAME_SIZE, 1, fileList[_fileName]);//���O���[�h
			
			fread(&tempSize, sizeof(size_t), 1, fileList[_fileName]);//�f�[�^�T�C�Y���[�h
			if (tempSize == 0)return false;

			//fread_s(&tempSize, sizeof(size_t), sizeof(size_t), 1, fileList[_fileName]);
			//fread_s(&tempSize, sizeof(int), sizeof(int), 1, fileList[_fileName]);

			//T* tempData = new T();
			//fread(tempData, tempSize, 1, fileList[_fileName]);//�f�[�^���[�h

			if (strcmp(tempName, _dataName) == 0)//�f�[�^���O����v
			{
				//�f�[�^�����[�h
				//if (!_out)_out = new T();
				fread(_out, tempSize, 1, fileList[_fileName]);

				//_out = *tempData;
				return true;
			}

			//���O����v���Ȃ������̂Ō����ʒu���ړ�
			if (fseek(fileList[_fileName], tempSize, SEEK_CUR) != 0)
			{
				MessageBox(nullptr, "���[�h���V�[�N�G���[", "ERROR", MB_OK);
				return false;
			}
		}

		//�����������ǃf�[�^��������Ȃ�����
		return false;
	}
};