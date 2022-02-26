//=============================================================================
//
//  静的メッシュコンポーネント [Component_StaticMesh.h]
//  Date   : 2021/12/08
//
//=============================================================================
#pragma once

#pragma comment (lib,"Assimp/assimp.lib")
#include <unordered_map>
#include "ModelBase/Component_ModelBase.h"
#include "../Project/Assimp/cimport.h"
#include "../Project/Assimp/scene.h"
#include "../Project/Assimp/postprocess.h"

class Component_StaticMesh : public Component_ModelBase
{
private:

	//頂点構造体
	struct s_STATICMESH_VERTEX
	{
		Vector3 Position;
		Vector3 Normal;
		Color Color;
	};

protected:

	const aiScene* m_AiScene = NULL;

public:

	Component_StaticMesh(class Actor* _owner, int _updatePriority) : 
		Component_ModelBase(_owner, _updatePriority) {}

	void Init() override;
	void Update()override {};
	void Uninit()override;
	void Draw();


	void LoadStaticMesh(const char *_fileName, std::string _texName = "none");
	void SetColor(Vector4 _color) override;

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

};