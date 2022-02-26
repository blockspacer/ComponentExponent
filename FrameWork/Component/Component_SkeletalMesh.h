//=============================================================================
//
//  FBXモデルコンポーネント [Component_SkeletalMesh.h]
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

	//変形後頂点構造体
	struct s_DFEFORM_VERTEX
	{
		aiVector3D Position;
		aiVector3D Normal;
		Color Color;

		int         BoneNum;
		int			BoneIndex[4];
		float       BoneWeight[4];
	};

	//ボーン構造体
	struct s_BONE
	{
		aiMatrix4x4 Matrix;
		aiMatrix4x4 AnimationMatrix;
		aiMatrix4x4 OffsetMatrix;
	};

	float m_BlendRate;//ブレンドレート

	std::string m_CurrentAnimName;
	std::string m_NextAnimName;

	struct s_AnimationSet
	{
		const aiScene* animation;//アニメーション
		unsigned int frame;//現在のフレーム数
		bool isReverse;//逆再生フラグ
	};
	std::unordered_map<std::string, s_AnimationSet> m_Animation;//アニメーション配列

	std::vector<s_DFEFORM_VERTEX>* m_DeformVertex;//変形後頂点データ
	s_BONE* m_Bone;//ボーン
	std::unordered_map<std::string, int> m_BoneTable;
	float m_UpdateFrameTime;//ブレンド更新率

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

public:

	Component_SkeletalMesh(class Actor* owner, int updatePriority) : 
		Component_StaticMesh(owner, updatePriority) {}


	virtual void Init()override;	//初期化
	virtual void Uninit()override;  //終了
	virtual void Update() override;	//更新
	virtual void Draw();//描画

	//ブレンドレート加算
	void AddBlendRate(float rate);

	//モデル読み込み
	void LoadSkeletalMesh(const char *FileName);
	void LoadAnimation(const char *FileName, const char *AnimationName);//アニメーション読み込み
	void UnloadFBX();//削除

	void SetCurrentAnimationName(std::string _name, unsigned int _frame = 0);//現在のアニメーション名をセット
	void SetNextAnimationName(std::string _name, unsigned int _frame = 0);//次のアニメーションをセット
	void SetAnimation(std::string _start, std::string _end,
		unsigned int _startFrame = 0, unsigned int _endFrame = 0);//開始アニメーションと終了アニメーションをセット

	void SetCurrentAnimationFrame(unsigned int _frame);//現在のアニメーションのフレーム数をセット
	void SetNextAnimationFrame(unsigned int _frame);//次のアニメーションのフレーム数をセット

	inline std::string GetCurrentAnimationName()const  { return m_CurrentAnimName; }//現在のアニメーション名取得
	inline std::string GetNextAnimationName()const { return m_NextAnimName; }//次のアニメーション名取得
	inline unsigned int GetCurrentAnimationFrame() { return m_Animation[m_CurrentAnimName].frame; }//現在のアニメーションのアニメーションフレーム取得
	inline unsigned int GetNextAnimationFrame() { return m_Animation[m_NextAnimName].frame; }//次のアニメーションのアニメーションフレーム取得
	inline unsigned int GetCurrentAnimationFrameMax() { return  m_CurrentAnimName != "none" ? m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels : 0; }//アニメーションフレームの最大数取得
	inline unsigned int GetNextAnimationFrameMax() { return  m_NextAnimName != "none" ? m_Animation[m_NextAnimName].animation->mAnimations[0]->mNumChannels : 0; }//アニメーションフレームの最大数取得

	//ブレンドレート取得
	inline float GetBlendLate()const { return m_BlendRate; }
	unsigned int GetCurrentAnimationPlayTime();
	unsigned int GetNextAnimationPlayTime();
	inline void SetUpdateFlameTime(float _set) { m_UpdateFrameTime = _set; }//アニメーション更新速度設定

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;
};
