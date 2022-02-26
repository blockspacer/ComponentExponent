
#include "Main.h"
#include "DataSaver.h"

//todo:拡張子まで名前に含める必要がある
bool DataSaver::OpenFile(const char * _fileName, SAVEDATA_TYPE _type)
{
	if (IsOpenFile(_fileName))
	{
		char temp[512] = {};
		sprintf_s(temp, sizeof(temp), "セーブ呼び出し時に　%s　ファイルが既にOPEN状態です。", _fileName);
		MessageBox(nullptr, temp, "ERROR", MB_OK);
		return false;
	}

	//同じファイルを開いていないので開く

	const char* mode[SAVEDATA_TYPE::MAX] = { "wt","rt","wb","rb" };
	const char* choose = mode[static_cast<int>(_type)];
	std::string tempFileName = SAVEDATA_PATH + std::string(_fileName);
	fopen_s(&fileList[_fileName], tempFileName.c_str(), choose);//ファイルオープン
	if (!fileList[_fileName])
	{
		char temp[512] = {};
		sprintf_s(temp, sizeof(temp), "%s　ファイルが存在しないか、open形式とファイルの形式が違います。", tempFileName.c_str());
		MessageBox(nullptr, temp, "ERROR", MB_OK);
		fileList.erase(_fileName);
		return false;
	}

	return true;
}
