//=============================================================================
//
//  FBX���f���R���|�[�l���g [Component_SkeletalMesh.h]
//  Date   : 2021/11/06
//
//=============================================================================
#pragma once

#include <unordered_map>
#include <string>
#include "Component_StaticMesh.h"
#include "../Project/Assimp/matrix4x4.h"

class Component_SkeletalMesh : public Component_StaticMesh
{
private:

	//�ό`�㒸�_�\����
	struct s_DFEFORM_VERTEX
	{
		aiVector3D Position;
		aiVector3D Normal;
		Color Color;

		int         BoneNum;
		int			BoneIndex[4];
		float       BoneWeight[4];
	};

	//�{�[���\����
	struct s_BONE
	{
		aiMatrix4x4 Matrix;
		aiMatrix4x4 AnimationMatrix;
		aiMatrix4x4 OffsetMatrix;
	};

	float m_BlendRate;//�u�����h���[�g

	std::string m_CurrentAnimName;
	std::string m_NextAnimName;

	struct s_AnimationSet
	{
		const aiScene* animation;//�A�j���[�V����
		unsigned int frame;//���݂̃t���[����
		bool isReverse;//�t�Đ��t���O
	};
	std::unordered_map<std::string, s_AnimationSet> m_Animation;//�A�j���[�V�����z��

	std::vector<s_DFEFORM_VERTEX>* m_DeformVertex;//�ό`�㒸�_�f�[�^
	s_BONE* m_Bone;//�{�[��
	std::unordered_map<std::string, int> m_BoneTable;
	float m_UpdateFrameTime;//�u�����h�X�V��

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

public:

	Component_SkeletalMesh(class Actor* owner, int updatePriority) : 
		Component_StaticMesh(owner, updatePriority) {}


	virtual void Init()override;	//������
	virtual void Uninit()override;  //�I��
	virtual void Update() override;	//�X�V
	virtual void Draw();//�`��

	//�u�����h���[�g���Z
	void AddBlendRate(float rate);

	//���f���ǂݍ���
	void LoadSkeletalMesh(const char *FileName);
	void LoadAnimation(const char *FileName, const char *AnimationName);//�A�j���[�V�����ǂݍ���
	void UnloadFBX();//�폜

	void SetCurrentAnimationName(std::string _name, unsigned int _frame = 0);//���݂̃A�j���[�V���������Z�b�g
	void SetNextAnimationName(std::string _name, unsigned int _frame = 0);//���̃A�j���[�V�������Z�b�g
	void SetAnimation(std::string _start, std::string _end,
		unsigned int _startFrame = 0, unsigned int _endFrame = 0);//�J�n�A�j���[�V�����ƏI���A�j���[�V�������Z�b�g

	void SetCurrentAnimationFrame(unsigned int _frame);//���݂̃A�j���[�V�����̃t���[�������Z�b�g
	void SetNextAnimationFrame(unsigned int _frame);//���̃A�j���[�V�����̃t���[�������Z�b�g

	inline std::string GetCurrentAnimationName()const  { return m_CurrentAnimName; }//���݂̃A�j���[�V�������擾
	inline std::string GetNextAnimationName()const { return m_NextAnimName; }//���̃A�j���[�V�������擾
	inline unsigned int GetCurrentAnimationFrame() { return m_Animation[m_CurrentAnimName].frame; }//���݂̃A�j���[�V�����̃A�j���[�V�����t���[���擾
	inline unsigned int GetNextAnimationFrame() { return m_Animation[m_NextAnimName].frame; }//���̃A�j���[�V�����̃A�j���[�V�����t���[���擾
	inline unsigned int GetCurrentAnimationFrameMax() { return  m_CurrentAnimName != "none" ? m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels : 0; }//�A�j���[�V�����t���[���̍ő吔�擾
	inline unsigned int GetNextAnimationFrameMax() { return  m_NextAnimName != "none" ? m_Animation[m_NextAnimName].animation->mAnimations[0]->mNumChannels : 0; }//�A�j���[�V�����t���[���̍ő吔�擾

	//�u�����h���[�g�擾
	inline float GetBlendLate()const { return m_BlendRate; }
	unsigned int GetCurrentAnimationPlayTime();
	unsigned int GetNextAnimationPlayTime();
	inline void SetUpdateFlameTime(float _set) { m_UpdateFrameTime = _set; }//�A�j���[�V�����X�V���x�ݒ�

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;
};
