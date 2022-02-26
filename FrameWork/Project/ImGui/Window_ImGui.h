#pragma once

#include "../../System/Main.h"
#include "../ImGui/imgui.h"
#include <string>
#include <unordered_map>
#include <list>

class Window_ImGui
{
public:

	Window_ImGui() {};
	~Window_ImGui() {};


	void SetDrawData();//�`��p�f�[�^���Z�b�g����֐�
	void Init();
	void Update();
	void Uninit();

	void SendBoolData(bool* _bool, std::string _dataName);//BOOL�Z�b�g�B
	void SendIntData(int* _int, std::string _dataName, int _min, int _max);//INT�Z�b�g
	void SendFloatData(float* _float, std::string _dataName, float min, float max);//FLOAT�Z�b�g
	void SendStringData(std::string* _float, std::string _dataName);//�X�g�����O�Z�b�g
	void SendVector2Data(Vector2* _data, std::string _dataName, float min, float max);//VEC2�Z�b�g
	void SendVector3Data(Vector3* _data, std::string _dataName, float min, float max);//VEC3�Z�b�g
	void SendVector4Data(Vector4* _data, std::string _dataName, float min, float max);//VEC4�Z�b�g
	void DebugLog(std::string, std::string _logName = "none", Vector4 _color = Vector4(1, 1, 1, 1),
		int dispCnt = (int)(60 * 2), bool _isUseKeyCheck = false);
	void DebugLogDontDestroy(std::string _data, std::string _key, Vector4 _color = Vector4(1, 1, 1, 1));
	void AddButton(const char* _buttonName, bool& _link, ImVec2 _buttonSize = ImVec2(128.0f, 32.0f));//�E�B���h�E�Ƀ{�^���ǉ�
	void AddColorPicker(const char* _colorPickerName, Vector4& _link);

	void SetIsDisp(bool _set) { isDisp = _set; }
	inline void SetWindowSize(Vector2 _size) { windowSize = _size; }//�E�B���h�E�T�C�Y�ύX
	inline void SetWindowPosition(Vector2 _pos) { windowPosition = _pos; };
	inline void SetWindowName(std::string _set) { windowName = _set; }//�E�B���h�E����ς���֐�
	inline void SetWindowColor(Vector4 _color) { windowColor = _color; };

private:
	
	struct s_GuiIntData//GUI�pINT�f�[�^
	{
		int* data = nullptr;
		int min = -1000;
		int max = 1000;
	};

	struct s_GuiFloatData//GUI�pFLOAT�f�[�^
	{
		float* data = nullptr;
		float min = -1000.0f;
		float max = 1000.0f;
	};

	struct s_GuiVec2Data//GUI�pVEC2�f�[�^
	{
		Vector2* data = nullptr;
		float min = -1000.0f;
		float max = 1000.f;
	};

	struct s_GuiVec3Data//GUI�pVEC3�f�[�^
	{
		Vector3* data = nullptr;
		float min = -1000.0f;
		float max = 1000.0f;
	};

	struct s_GuiVec4Data//GUI�pVEC4�f�[�^
	{
		Vector4* data = nullptr;
		float min = -1000.0f;
		float max = 1000.0f;
	};

	struct s_LogData
	{
		std::string key = "";
		std::string data = "";
		int dispTime = 60 * 2;
		int cnt = 0;
		bool isDestroy = true;
		Vector4 color = Vector4(1, 1, 1, 1);
	};

	struct s_ButtonData
	{
		bool* link;
		ImVec2 size;
	};

	struct s_GuiColorData
	{
		float color[4] = {};
		Vector4* link;
	};

	std::string windowName;//�E�B���h�E��
	bool isDisp;//�\�����邩�ǂ���
	Vector2 windowPosition;
	Vector2 windowSize;
	Vector4 windowColor;

	//�f�o�b�O����ϐ��B�̃|�C���^��ۑ����Ă����z��B
	std::unordered_map<std::string, bool*>boolList;
	std::unordered_map<std::string, s_GuiIntData>intList;
	std::unordered_map<std::string, s_GuiFloatData>floatList;
	std::unordered_map<std::string, std::string*>stringList;
	std::unordered_map<std::string, s_GuiVec2Data>vec2List;
	std::unordered_map<std::string, s_GuiVec3Data>vec3List;
	std::unordered_map<std::string, s_GuiVec4Data>vec4List;
	std::unordered_map<std::string, s_ButtonData>buttonList;
	std::unordered_map<std::string, s_GuiColorData>colorList;
	std::list<s_LogData>logList;
};
