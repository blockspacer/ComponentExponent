//=============================================================================
//
//  OBB�R���|�[�l���g [Component_OBB.h]
//  Author : Ryosuke Kotera
//  Date   : 2021/10/18
//
//=============================================================================
#pragma once

#include "Base/Component.h"
#include "../Manager/Manager_Drawer/Manager_OBB.h"

class Component_OBB : public Component
{
public:

	Component_OBB(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}

	void Init()   override;//������
	void Uninit() override;//�I��
	void Update() override;//�X�V
	
	//�`��
	void Draw();

	//���F���ݒ�
	inline void SetVisibility(bool _visibility) { m_Visibility = _visibility; }

	//�X�P�[���ݒ�
	inline void SetScale(Vector3 _scale) { m_Scale = _scale; }

	//�X�P�[���擾
	inline Vector3 GetScale() const { return m_Scale; };

	// ���W�擾
	inline Vector3 GetPosition() const { return m_Position; };

	//��]�擾
	inline Vector3 GetRotation() const { return m_Rotation; }

	//���F���擾
	inline bool GetVisibility()  const { return  m_Visibility; }

	//�F�ݒ�
	inline void SetColor(Color _color) { m_Color = _color; }

	//��]�Z�b�g
	inline void SetRotation(Vector3 _rot) { m_Rotation = _rot; }

	//���C�ɔ������邩���擾�E�ݒ�
	inline void SetIsReactRay(bool _set) { m_isReactRay = _set; }
	inline bool GetIsReactRay()const { return m_isReactRay; }

	//�����}�e���A���擾�E�ݒ�
	inline int GetPhysicsMaterial() const{ return m_PhysicsMaterial; }
	inline void SetPhysicsMaterial(int _physicsMaterial)  { m_PhysicsMaterial = _physicsMaterial; }

	//OBB
	inline Vector3 GetNormaDirectionX() const { return m_NormaDirectionX; };// �w�莲�ԍ���X�����x�N�g�����擾
	inline Vector3 GetNormaDirectionY() const { return m_NormaDirectionY; };// �w�莲�ԍ���Y�����x�N�g�����擾
	inline Vector3 GetNormaDirectionZ() const { return m_NormaDirectionZ; };// �w�莲�ԍ���Z�����x�N�g�����擾

	inline float   GetLengthX()        const { return m_LengthX; }// X�������̒������擾
	inline float   GetLengthY()        const { return m_LengthY; }// Y�������̒������擾
	inline float   GetLengthZ()        const { return m_LengthZ; }// Z�������̒������擾

	//�I�[�i�[���W�ݒ�
	void SetOwnerPosition(Vector3 _position);

	//���C�L���X�g
	//_ignoreMySelf�͖����������������g��Actor*�B_ignoreList�͖����������A�N�^�[�^�C�v�Blist�B
	bool Raycast(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//�����}�e���A����Ԃ����C�L���X�g
	int RaycastPM(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//_ignoreMySelf�͖����������������g��Actor*�B_ignoreList�͖����������A�N�^�[�^�C�v�Blist�B
	std::vector<Manager_OBB::s_RaycastHit> RaycastGetHit(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

private:

	class Manager_OBB* m_Manager_OBB;//OBB�}�l�[�W���[�L���b�V���p

	Vector3 m_NormaDirectionX;//X�����x�N�g��
	Vector3 m_NormaDirectionY;//Y�����x�N�g��
	Vector3 m_NormaDirectionZ;//Z�����x�N�g��

	float   m_LengthX;// X�������̒���
	float   m_LengthY;// Y�������̒���
	float   m_LengthZ;// Z�������̒���

	Vector3 m_Position;//���W
	Vector3 m_Rotation;//��]
	Vector3 m_Scale;   //�X�P�[��

	Color   m_Color;//�F

	//�g�p����V�F�[�_�[�̎��
	int  m_ShaderType;

	//���F��
	bool m_Visibility;

	//���C�ɔ������邩
	bool m_isReactRay;

	//�����}�e���A��
	int  m_PhysicsMaterial;

};


