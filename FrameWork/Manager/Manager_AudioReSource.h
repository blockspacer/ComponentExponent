#pragma once

#include "../System/WaveLoder.h"
#include <unordered_map>
#include "../System/Singleton.h"

class Manager_AudioResource : public Singleton<Manager_AudioResource>
{
public:

	void Load_Resource(const char* key,LPCSTR fileName);
	LPCSTR GetResource(const char* key) { return m_Resource[key]; }

private:

	Manager_AudioResource() {};
	~Manager_AudioResource() {};

	friend Singleton<Manager_AudioResource>;

	std::unordered_map<std::string, LPCSTR> m_Resource;
};
