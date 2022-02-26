//=============================================================================
//
//  SkySphereクラス [SkySphere.h]
//  Date   : 2021/1/16
//
//=============================================================================
#pragma once


#include "../Base/Actor3D.h"


class SkySphere : public Actor3D
{
private:

	class Component_StaticMesh* m_Component_StaticMesh;//静的メッシュコンポーネント

public:

	virtual void Init()override;//初期化
	virtual void Update()override;//更新

};