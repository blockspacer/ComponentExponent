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
	const int TEMP_DATA_NAME_LEN_MAX = 256;//一時データ名の最大文字数
	const size_t SAVEDATA_START_NAME_SIZE = sizeof(char) * TEMP_DATA_NAME_LEN_MAX;//一時データ名のサイズ

	std::unordered_map<const char*, FILE*>fileList;//開いているファイルリスト


public:

	enum SAVEDATA_TYPE
	{
		write_txt,
		read_txt,

		write_bin,
		read_bin,

		MAX
	};

	//ファイルが開いているか確認する open = true
	inline bool IsOpenFile(const char* _fileName)
	{
		return fileList.count(_fileName) > 0;
	}

	//ファイルをオープンするだけ。
	bool OpenFile(const char* _fileName, SAVEDATA_TYPE _type);

	//ファイルを閉じるだけ。
	void CloseFile(const char* _fileName)
	{
		if (fileList.count(_fileName) <= 0)return;
		fclose(fileList[_fileName]);
		fileList.erase(_fileName);
	}

	//全てのファイルを閉じる
	void CloseAllFiles()
	{
		for (auto& i : fileList)
		{
			fclose(i.second);
		}
		fileList.clear();
	}

	//一つのデータをセーブ
	template <typename T>
	inline bool Save(const char* _fileName, T* _data, size_t _dataSize, const char* _dataName)
	{
		if (fileList.count(_fileName) <= 0)return false;//ファイルを開いてない

		fwrite(_dataName, SAVEDATA_START_NAME_SIZE, 1, fileList[_fileName]);//データ名書き込み
		fwrite(&_dataSize, sizeof(size_t), 1, fileList[_fileName]);//データサイズ書き込み
		fwrite(_data, _dataSize, 1, fileList[_fileName]);//書き込み

		return true;
	}

	//一つのデータをロード
	template <typename T>
	inline bool Load(const char* _fileName, T* _out, const char* _dataName)
	{
		if (fileList.count(_fileName) <= 0)return false;//ファイルを開いてない
		if (!fileList.count(_fileName))return false;//ファイルを開いてない

		//ファイルの先頭までシーク
		if (fseek(fileList[_fileName], 0, SEEK_SET) != 0) 
		{
			MessageBox(nullptr, "ロード時シークエラー", "ERROR", MB_OK);
			return false;
		}

		//EOFまで名前が一致するまで検索
		while (feof(fileList[_fileName]) != EOF || ftell(fileList[_fileName]) != EOF)
		{
			size_t tempSize = {};
			char tempName[512] = {};

			fread(tempName, SAVEDATA_START_NAME_SIZE, 1, fileList[_fileName]);//名前ロード
			
			fread(&tempSize, sizeof(size_t), 1, fileList[_fileName]);//データサイズロード
			if (tempSize == 0)return false;

			//fread_s(&tempSize, sizeof(size_t), sizeof(size_t), 1, fileList[_fileName]);
			//fread_s(&tempSize, sizeof(int), sizeof(int), 1, fileList[_fileName]);

			//T* tempData = new T();
			//fread(tempData, tempSize, 1, fileList[_fileName]);//データロード

			if (strcmp(tempName, _dataName) == 0)//データ名前が一致
			{
				//データをロード
				//if (!_out)_out = new T();
				fread(_out, tempSize, 1, fileList[_fileName]);

				//_out = *tempData;
				return true;
			}

			//名前が一致しなかったので検索位置を移動
			if (fseek(fileList[_fileName], tempSize, SEEK_CUR) != 0)
			{
				MessageBox(nullptr, "ロード時シークエラー", "ERROR", MB_OK);
				return false;
			}
		}

		//検索したけどデータが見つからなかった
		return false;
	}
};