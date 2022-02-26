//=============================================================================
//
// キューブクラス [Cube.h]
//  Date   : 2021/11/4
//
//=============================================================================
#pragma once


#include "../Base/ActorRB.h"

class Cube : public ActorRB
{
protected:

	class Component_StaticMesh* m_Component_StaticMesh;//静的メッシュコンポーネント
	const char* texName;

public:

	Cube() :texName(nullptr) {}
	Cube(const char* _texName) :texName(_texName) {}

	virtual ~Cube() {}

	virtual void Init()override;//初期化
	void Uninit() override;

};