//=============================================================================
//
//  階段クラス [Stair.h]
//  Date   : 2021/12/4
//
//=============================================================================
#pragma once


#include "../Base/ActorRB.h"

class Stair : public ActorRB
{
protected:

	class Component_StaticMesh*           m_Component_StaticMesh;          //静的メッシュコンポーネント

	class Component_OBB* m_Component_OBB;

public:

	virtual void Init()override;//初期化
	virtual void Update()override;//更新


};