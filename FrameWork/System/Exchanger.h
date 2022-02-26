//=============================================================================
//
//  ïœä∑ã@ [Exchanger.h]
//  Date   : 2021/11/2
//
//=============================================================================
#pragma once


#include <string>


class Exchanger
{
public:

	static std::string INT_TO_STRING(int _value);
	static std::string FLOAT_TO_STRING(float _value);
	static std::string DOUBLE_TO_STRING(double _value);
	static std::string BOOL_TO_STRING(bool _value);
	static std::string VEC2_TO_STRING(Vector2 _value);
	static std::string VEC3_TO_STRING(Vector3 _value);
	static std::string VEC4_TO_STRING(Vector4 _value);
	static std::string WSTRING_TO_STRING(std::wstring _text);
	static std::wstring STRING_TO_WSTRING(std::string _text);

	//todo:
	static int STRING_TO_INT(const char* _string);
	static int CHAR_TO_INT(const char _char);
	static float STRING_TO_FLOAT(const char* _string);
	static double STRING_TO_DOUBLE(const char* _string);

	//Colorån
	static Color VEC4_TO_COLOR(Vector4 _color);
	static Vector4 COLOR_TO_VEC4(Color _color);

	//todo:quatunion to eurerÅ@ÇªÇÃãtÇ‡ÅB

};
