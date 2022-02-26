
#pragma once

#include "Base/Level.h"
#include <list>

class Level_Game : public Level
{
public:
	void Init()override;
	void Uninit()override;
	void Update()override;

private:

	class Camera* m_MainCamera;//�J�����L���b�V���ϐ�
	std::list<class Coin*>coinList;//�R�C�����X�g
};