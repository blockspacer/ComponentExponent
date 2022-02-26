#include "Manager_AudioReSource.h"

Manager_AudioResource* Manager_AudioResource::m_Instance;
std::unordered_map<std::string, LPCSTR> Manager_AudioResource::m_Resource;

void Manager_AudioResource::Load_Resource(const char* key, LPCSTR fileName)
{
	if (!m_Resource[key]) {
		m_Resource[key] = fileName;
	}
}

Manager_AudioResource* Manager_AudioResource::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new Manager_AudioResource;
	}

	return m_Instance; 
}
