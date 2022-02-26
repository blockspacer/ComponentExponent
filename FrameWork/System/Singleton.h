//=============================================================================
//
//  ƒVƒ“ƒOƒ‹ƒgƒ“ [Singleton.h]
//  Date   : 2021/11/2
//
//=============================================================================
#pragma once

template <typename T>
class Singleton
{
protected:

	Singleton() {};
	virtual ~Singleton() {};
	Singleton(const Singleton& singleton) {};
	Singleton& operator=(const Singleton& singleton) {};

public:

	static T* Instance()
	{
		static T instance;
		return &instance;
	}

};
