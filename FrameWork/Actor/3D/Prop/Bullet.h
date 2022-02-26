//=============================================================================
//
//  弾クラス [Bullet.h]
//  Date   : 2021/11/4
//
//=============================================================================
#pragma once


#include "../Base/Actor3D.h"


class Bullet : public Actor3D
{
protected:

	class Component_OBB*     m_Component_OBB;    //OBBコンポーネント

public:

	virtual void Init()override;//初期化
	virtual void Update()override;//更新

};