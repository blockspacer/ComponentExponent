
#include "Window_ImGui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"


void Window_ImGui::SetDrawData()
{
	//このウィンドウの描画開始
	ImGui::Begin(windowName.c_str(), &isDisp);
	//ImGui::Begin(windowName.c_str());

	//BOOL表示
	auto boolItr = boolList.begin();
	for (; boolItr != boolList.end(); boolItr++)
	{
		ImGui::Checkbox(boolItr->first.c_str(), boolItr->second);
	}

	//INT表示
	auto intItr = intList.begin();
	for (; intItr != intList.end(); intItr++)
	{
		ImGui::SliderInt(intItr->first.c_str(), intItr->second.data, intItr->second.min, intItr->second.max );
	}

	//FLOAT表示
	auto floatItr = floatList.begin();
	for (; floatItr != floatList.end(); floatItr++)
	{
		ImGui::SliderFloat(floatItr->first.c_str(), floatItr->second.data, floatItr->second.min, floatItr->second.max);
	}

	//VECTOR2表示
	auto vec2Itr = vec2List.begin();
	for (; vec2Itr != vec2List.end(); vec2Itr++)
	{
		ImGui::SliderFloat2(vec2Itr->first.c_str(), *vec2Itr->second.data, vec2Itr->second.min, vec2Itr->second.max);
	}

	//VECTOR3表示
	auto vec3Itr = vec3List.begin();
	for (; vec3Itr != vec3List.end(); vec3Itr++)
	{
		//ImGui::SliderFloat3(vec3Itr->first.c_str(), *vec3Itr->second.data, vec3Itr->second.min, vec3Itr->second.max);
		ImGui::DragFloat3(vec3Itr->first.c_str(), *vec3Itr->second.data, 0.1f,vec3Itr->second.min, vec3Itr->second.max);
	}

	//VECTOR4表示
	auto vec4Itr = vec4List.begin();
	for (; vec4Itr != vec4List.end(); vec4Itr++)
	{
		ImGui::SliderFloat4(vec4Itr->first.c_str(), *vec4Itr->second.data, vec4Itr->second.min, vec4Itr->second.max);
	}

	//STRING表示
	auto stringItr = stringList.begin();
	for (; stringItr != stringList.end(); stringItr++)
	{
		ImGui::Text(stringItr->second->c_str());
	}

	//ボタン表示
	auto buttonItr = buttonList.begin();
	for (; buttonItr != buttonList.end(); buttonItr++)
	{
		memset(buttonItr->second.link, false, sizeof(bool));

		//押された時に現在の状態を変える
		if (ImGui::Button(buttonItr->first.c_str(), buttonItr->second.size))
		{
			memset(buttonItr->second.link, true, sizeof(bool));
		}
	}

	auto colorItr = colorList.begin();
	for (; colorItr != colorList.end(); colorItr++)
	{
		if (ImGui::ColorEdit4(colorItr->first.c_str(), colorItr->second.color))
		{
			colorItr->second.link->x = colorItr->second.color[0];
			colorItr->second.link->y = colorItr->second.color[1];
			colorItr->second.link->z = colorItr->second.color[2];
			colorItr->second.link->w = colorItr->second.color[3];
		}
	}

	//デバッグログ表示
	for (auto& i : logList)
	{
		ImGui::TextColored(ImVec4(i.color.x, i.color.y, i.color.z, i.color.w), i.data.c_str());
	}

	//ウィンドウ設定
	//ImGui::SetNextWindowPos(ImVec2(windowPosition.x, windowPosition.y), ImGuiCond_Always);//ポジション
	//ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y),ImGuiCond_Once);//サイズ
	//ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y),ImGuiCond_Always);//サイズ

	//このウィンドウの描画終了
	ImGui::End();
}

void Window_ImGui::Init()
{
	boolList.clear();
	intList.clear();
	floatList.clear();
	stringList.clear();
	vec2List.clear();
	vec3List.clear();
	vec4List.clear();
	buttonList.clear();
	colorList.clear();
	logList.clear();

	isDisp = true;
}

void Window_ImGui::Update()
{
	for (auto& i : logList)
	{
		i.cnt++;
	}
	logList.remove_if([](s_LogData log) {return (log.cnt > log.dispTime) && log.isDestroy; });
}

void Window_ImGui::Uninit()
{
	boolList.clear();
	intList.clear();
	floatList.clear();
	stringList.clear();
	vec2List.clear();
	vec3List.clear();
	vec4List.clear();
	buttonList.clear();
	colorList.clear();
	logList.clear();
}

void Window_ImGui::SendBoolData(bool* _data, std::string _dataName)
{
	if (boolList.count(_dataName) == 0)
	{
		boolList[_dataName] = _data;
	}
}

void Window_ImGui::SendIntData(int* _data, std::string _dataName, int min, int max)
{
	if (intList.count(_dataName) == 0)
	{
		s_GuiIntData temp;
		temp.data = _data;
		temp.min = min;
		temp.max = max;
		intList[_dataName] = temp;
	}
}

void Window_ImGui::SendFloatData(float* _data, std::string _dataName, float min, float max)
{
	if (floatList.count(_dataName) == 0)
	{
		s_GuiFloatData temp;
		temp.data = _data;
		temp.min = min;
		temp.max = max;
		floatList[_dataName] = temp;
	}
}

void Window_ImGui::SendStringData(std::string* _data, std::string _dataName)
{
	if (stringList.count(_dataName) == 0)
	{
		stringList[_dataName] = _data;
	}
}

void Window_ImGui::SendVector2Data(Vector2* _data, std::string _dataName, float min, float max)
{
	if (vec2List.count(_dataName) == 0)
	{
		s_GuiVec2Data temp;
		temp.data = _data;
		temp.min = min;
		temp.max = max;
		vec2List[_dataName] = temp;
	}
}

void Window_ImGui::SendVector3Data(Vector3* _data, std::string _dataName, float min, float max)
{
	if (vec3List.count(_dataName) == 0)
	{
		s_GuiVec3Data temp;
		temp.data = _data;
		temp.min = min;
		temp.max = max;
		vec3List[_dataName] = temp;
	}
}

void Window_ImGui::SendVector4Data(Vector4* _data, std::string _dataName, float min, float max)
{
	if (vec4List.count(_dataName) == 0)
	{
		s_GuiVec4Data temp;
		temp.data = _data;
		temp.min = min;
		temp.max = max;
		vec4List[_dataName] = temp;
	}
}

void Window_ImGui::AddButton(const char* _buttonName, bool & _link, ImVec2 _buttonSize)
{
	if (buttonList.count(_buttonName) == 0)
	{
		s_ButtonData temp;
		temp.link = &_link;
		temp.size = _buttonSize;
		buttonList[_buttonName] = temp;
	}
}

void Window_ImGui::AddColorPicker(const char * _colorPickerName, Vector4& _link)
{
	if (colorList.count(_colorPickerName) == 0 && _link)
	{
		s_GuiColorData temp;
		temp.link = &_link;
		temp.color[0] = _link.x;
		temp.color[1] = _link.y;
		temp.color[2] = _link.z;
		temp.color[3] = _link.w;
		colorList[_colorPickerName] = temp;
	}
}

void Window_ImGui::DebugLog(std::string _data, std::string _logName, Vector4 _color, int _dispCnt, bool _isUseKeyCheck)
{
	//配列のサイズを超える前にLOG全削除
	if (logList.max_size() <= logList.size())
	{
		logList.clear();
	}

	//一回のみの追加
	if (_isUseKeyCheck)
	{
		//同じキーを持つログがあったら登録しない
		for (auto& i : logList)
		{
			if (i.key == _logName)
			{
				i.data = _data;
				i.color = _color;
				return;
			}
		}

		s_LogData temp;
		temp.data = _data;
		temp.dispTime = _dispCnt;
		temp.key = _logName;
		temp.cnt = 0;
		temp.isDestroy = true;
		temp.color = _color;

		logList.push_back(temp);
	}
	else//Updateで毎フレーム追加
	{
		s_LogData temp;
		temp.data = _data;
		temp.dispTime = _dispCnt;
		temp.key = _logName;
		temp.cnt = 0;
		temp.isDestroy = true;
		temp.color = _color;

		char tempName[256] = {};
		sprintf(tempName, "%s", _logName.c_str());
		if (_isUseKeyCheck)
		{
			sprintf(tempName, "%s_%d", _logName.c_str(), logList.size());
		}
		temp.key = tempName;

		logList.push_back(temp);
	}
}

void Window_ImGui::DebugLogDontDestroy(std::string _data, std::string _key, Vector4 _color)
{
	//配列のサイズを超える前にLOG全削除
	if (logList.max_size() <= logList.size())
	{
		logList.clear();
	}

	//同じキーを持つログがあったら登録しない
	for (auto& i : logList)
	{
		if (i.key == _key)
		{
			i.data = _data;
			i.color = _color;
			return;
		}
	}

	s_LogData temp;
	temp.data = _data;
	temp.dispTime = -1;
	temp.key = _key;
	temp.cnt = 0;
	temp.isDestroy = false;
	temp.color = _color;

	logList.push_back(temp);
}
