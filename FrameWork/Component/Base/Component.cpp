//=============================================================================
//
//  コンポーネント[Component.h]
//  Date   : 2021/11/1
//
//=============================================================================
#pragma once

#include <stdio.h>
#include "../../Actor/Base/Actor.h"
#include "Component.h"
#include "../../System/DataSaver.h"

void Component::Init()
{
	m_Active = true;//有効化
}

bool Component::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component>(_fileName, this, sizeof(Component), tempName))
	{
		return false;
	}
	return true;
}

bool Component::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}
