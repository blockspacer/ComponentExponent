//=============================================================================
//
//  コンポーネント[Component.h]
//  Date   : 2021/11/1
//
//=============================================================================
#pragma once

#include "../../System/Main.h"

class Component
{
public:
	Component(class Actor* _owner, int _updatePriority) : m_Owner(_owner), m_UpdatePriority(_updatePriority) {}

	virtual void Init()   = 0;//初期化
	virtual void Uninit() = 0;//終了
	virtual void Update() = 0;//更新

	int GetPriority()const  { return m_UpdatePriority; }//優先順取得

	inline void SetActive(bool _set) { m_Active = _set; }//アクティブセット
	inline bool GetActive()const { return m_Active; }//有効か取得

	inline class Actor* GetOwner() { return m_Owner; }//このコンポーネントを所持しているアクター取得

	virtual bool SaveComponentData(const char* _fileName);//コンポーネントのデータをセーブ
	virtual bool LoadComponentData(const char* _fileName);

protected:

	class Actor* m_Owner;//このコンポーネントを所持しているアクター
	int m_UpdatePriority;//更新優先順
	bool m_Active;//有効か
};
