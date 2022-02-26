#pragma once

#include "../System/Main.h"
#include "../Renderer/Renderer_DX11.h"
#include "ModelBase/Component_ModelBase.h"

// マテリアル構造体
struct sModelMaterial
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;
};

// 描画サブセット構造体
struct sSubset
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	sModelMaterial	Material;
};


// モデル構造体
struct sModel
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	sSubset			*SubsetArray;
	unsigned int	SubsetNum;
};

class Component_OBJ : public Component_ModelBase
{
private:

	ID3D11Buffer*	m_VertexBuffer;
	ID3D11Buffer*	m_IndexBuffer;

	sSubset*	m_SubsetArray;
	unsigned int	m_SubsetNum;

	void LoadObj(const char *_FileName, sModel *_Model);
	void LoadMaterial(const char *_FileName, sModelMaterial **_MaterialArray, unsigned int *_MaterialNum);

public:

	Component_OBJ(class Actor* _owner, int _updatePriority) :
		Component_ModelBase(_owner, _updatePriority) {}

	void Init() override;
	void Update() override {};
	void Uninit() override;
	void Draw();

	void Load(const char *_FileName);

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

};