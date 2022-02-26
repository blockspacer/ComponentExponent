//=============================================================================
//
//  OBB�R���|�[�l���g�}�l�[�W���[ [Manager_OBB.h]
//  Date   : 2021/11/10
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../../System/Main.h"
#include "ManagerComponent_MultiArray_Base.h"

//�R���W�����^�C�v
enum ACTOR_TYPE
{
	COLLISION_PLAYER,//�v���C���[
	COLLISION_ENEMY,//�G�l�~�[
	COLLISION_BULLET,//�e
	COLLISION_BLOCK,//�u���b�N
	COLLISION_ITEM,//�A�C�e��
	COLLISION_CAMERA,//�J����
	MAX_ACTOR_TYPE,//�A�N�^�[�^�C�v��
};

//�����}�e���A��
enum PHYSICS_MATERIAL
{
	PM_CONCRETE,//�R���N���[�g
	PM_METAL,   //����
	PM_SAND,    //��
	MAX_PM_TYPE,//�����}�e���A���ő吔
};

//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_OBB
{
public:

	struct s_ConpareOBBInfo
	{
		float caveinLength = 0.0f;
		Vector3 caveinDir = { 0,0,0 };
		bool isHit = false;
	};

	struct s_RaycastHit
	{
		float length = 0.0f;
		class Actor* hitObj = nullptr;
		Vector3 hitPos = Vector3_Zero;
	};

private:

	struct s_OBBCompData
	{
		Vector3 axisX = { 1,0,0 };
		Vector3 axisY = { 0,1,0 };
		Vector3 axisZ = { 0,0,1 };
		float lengthX = 0.0f;
		float lengthY = 0.0f;
		float lengthZ = 0.0f;
	};

	std::list<class Component_OBB*>m_ComponentList[ACTOR_TYPE::MAX_ACTOR_TYPE];

	bool CompareLength(s_OBBCompData* _A, s_OBBCompData* _B, Vector3* _separate, Vector3* _distance);

	bool CompareLengthAndGetCaveinLength(s_OBBCompData * _A,
		s_OBBCompData * _B, Vector3 * _separate, Vector3 * _distance, 
		class Component_OBB* _Acmp, class Component_OBB* _Bcmp);

	struct s_DrawData
	{
		Vector3 startPos;
		Vector3 endPos;
		Color color;
		int time;
		int timeMax;
	};

	std::list<s_DrawData>debugDrawRayList;

public:

	void Update();//�X�V
	void Draw();  //�`��
	void Uninit();//�I��

	void AddComponent(class Component_OBB* _add,int _layer)
	{
		m_ComponentList[_layer].push_back(_add);
	}

	void RemoveComponent(class Component_OBB* _del);

	//OBB�̃I�[�o�[���b�v����
	bool OverlapJudgeOBB(Component_OBB* _component_OBBA, Component_OBB* _component_OBBB);
	//OBB�u���b�N����
	bool BlockJudgeOBB(Component_OBB* _component_OBBA, Component_OBB* _component_OBBB);
	//OBB�Ɠ_�̔���
	bool OverlapJudgeOBBandPoint(Component_OBB* _component_OBB, Vector3 _point);

	//���C�L���X�g
	//_ignoreMySelf�͖����������������g��Actor*�B_ignoreList�͖����������A�N�^�[�^�C�v�Blist�B
	bool Raycast(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		 Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//�����}�e���A����Ԃ����C�L���X�g
    //_ignoreMySelf�͖����������������g��Actor*�B_ignoreList�͖����������A�N�^�[�^�C�v�Blist�B
	int RaycastPM(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//_ignoreMySelf�͖����������������g��Actor*�B_ignoreList�͖����������A�N�^�[�^�C�v�Blist�B
	std::vector<s_RaycastHit> RaycastGetHit(Vector3 _startPos, Vector3 _endPos, 
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		 Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);
};

