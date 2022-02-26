//=============================================================================
//
//  FBX���f���R���|�[�l���g [Component_FBX.cpp]
//  Date   : 2021/11/06
//
//=============================================================================

#include "Component_SkeletalMesh.h"
#include "../Actor/3D/Base/Actor3D.h"
#include "../Actor/3D/Camera.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_Model.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Level/Base/Level.h"
#include "../Project/Assimp/Importer.hpp"
#include "../Renderer/Renderer_Polygon.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"

#define OUT_MATRIX_MACRO(type)\
outMatrix.##type##1 = \
matrix[0].##type##1 * deformVertex->BoneWeight[0] +\
matrix[1].##type##1 * deformVertex->BoneWeight[1] +\
matrix[2].##type##1 * deformVertex->BoneWeight[2] +\
matrix[3].##type##1 * deformVertex->BoneWeight[3];\
outMatrix.##type##2 = \
matrix[0].##type##2 * deformVertex->BoneWeight[0] +\
matrix[1].##type##2 * deformVertex->BoneWeight[1] +\
matrix[2].##type##2 * deformVertex->BoneWeight[2] +\
matrix[3].##type##2 * deformVertex->BoneWeight[3];\
outMatrix.##type##3 = \
matrix[0].##type##3 * deformVertex->BoneWeight[0] +\
matrix[1].##type##3 * deformVertex->BoneWeight[1] +\
matrix[2].##type##3 * deformVertex->BoneWeight[2] +\
matrix[3].##type##3 * deformVertex->BoneWeight[3];\
outMatrix.##type##4 = \
matrix[0].##type##4 * deformVertex->BoneWeight[0] +\
matrix[1].##type##4 * deformVertex->BoneWeight[1] +\
matrix[2].##type##4 * deformVertex->BoneWeight[2] +\
matrix[3].##type##4 * deformVertex->BoneWeight[3];


//���f���ǂݍ���
void  Component_SkeletalMesh::LoadSkeletalMesh(const char *FileName)
{
	m_AiScene = {};
	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];

	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<s_DFEFORM_VERTEX>[m_AiScene->mNumMeshes];
	//�ċA�I�Ƀ{�[������
	CreateBone(m_AiScene->mRootNode);
	m_Bone = new s_BONE[m_BoneTable.size()];
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		//���_�f�[�^
		aiMesh* mesh = m_AiScene->mMeshes[m];
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			s_DFEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneIndex[b] = 0;
				deformVertex.BoneWeight[b] = 0.0f;
			}
			m_DeformVertex[m].push_back(deformVertex);
		}
		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[m_BoneTable[bone->mName.C_Str()]].OffsetMatrix = bone->mOffsetMatrix;
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].BoneIndex[num] = m_BoneTable[bone->mName.C_Str()];
				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);//1���_�̍ő�{�[����4
			}
		}
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);

				//���_�J���[���������炻����擾
				if (mesh->HasVertexColors(m))
				{
					vertex[v].Diffuse = Vector4(1.0f - mesh->mColors[AI_MAX_NUMBER_OF_COLOR_SETS]->r,
						1.0f - mesh->mColors[AI_MAX_NUMBER_OF_COLOR_SETS]->g, 1.0f - mesh->mColors[AI_MAX_NUMBER_OF_COLOR_SETS]->b,
						1.0f - mesh->mColors[AI_MAX_NUMBER_OF_COLOR_SETS]->a);
				}
				else
				{
					vertex[v].Diffuse = Vector4(1, 1, 1, 1);
				}
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer_DX11::Instance()->GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;
			{
				unsigned int* index = new unsigned int[mesh->mNumFaces * 3];
				for (unsigned int f = 0; f < mesh->mNumFaces; f++)
				{
					const aiFace* face = &mesh->mFaces[f];
					assert(face->mNumIndices == 3);

					index[f * 3 + 0] = face->mIndices[0];
					index[f * 3 + 1] = face->mIndices[1];
					index[f * 3 + 2] = face->mIndices[2];
				}

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * mesh->mNumFaces * 3;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = index;
				Renderer_DX11::Instance()->GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

				delete[] index;
			}
		}
		for (unsigned int m = 0; m < m_AiScene->mNumMaterials; m++)
		{
			aiString path;
			if (m_AiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				//�e�N�X�`������*����n�܂�Ƃ���FBX�ɖ��ߍ��܂ꂽ�e�N�X�`�������[�h
				if (path.data[0] == '*')
				{
					if (m_Texture[path.data] == NULL)
					{
						ID3D11ShaderResourceView* texture = nullptr;
						int id = atoi(&path.data[1]);
						D3DX11CreateShaderResourceViewFromMemory(
							Renderer_DX11::Instance()->GetDevice(), (const unsigned char*)m_AiScene->mTextures[id]->pcData,
							m_AiScene->mTextures[id]->mWidth,
							NULL, NULL, &texture, NULL);
						m_Texture[path.data] = texture;
					}
				}
				else//�ʏ�̃p�X
				{
					if (m_Texture[path.data] == NULL)
					{
						//�e�N�X�`�����[�h
						if (m_Texture[path.data] = Manager_Texture::Instance()->LoadTexture(path.data, path.data))
						{
							continue;
						}
					}
					//�e�N�X�`�����[�h���s
					m_Texture[path.data] = Manager_Texture::Instance()->GetTexture("Error");
				}
			}
			else//�e�N�X�`������
			{
				m_Texture[path.data] = Manager_Texture::Instance()->GetTexture("Error");
			}
		}
	}
}

//FBX�A�j���[�V�����ǂݍ���
void  Component_SkeletalMesh::LoadAnimation(const char *FileName, const char *AnimationName)
{
	const std::string modelPath(FileName);

	m_Animation[AnimationName].animation = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_Animation[AnimationName].animation);
}

void Component_SkeletalMesh::CreateBone(aiNode* node)
{
	s_BONE bone;
	// ���o�^�̃{�[������������Index�����蓖�Ă�
	if (m_BoneTable.count(node->mName.C_Str()) == 0)
	{
		m_BoneTable[node->mName.C_Str()] = m_BoneTable.size();
	}

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

void  Component_SkeletalMesh::UnloadFBX()//���f���폜
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}
	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;

	for (auto& i : m_Texture)
	{
		i.second->Release();
	}
	aiReleaseImport(m_AiScene);
	m_AiScene = nullptr;

	for (auto Animation : m_Animation)//�A�j���[�V�����폜
	{
		aiReleaseImport(Animation.second.animation);
	}

	delete[] m_Bone;
	//todo:uninit�ƈႤ���ǂ���
}

void Component_SkeletalMesh::SetCurrentAnimationName(std::string _name, unsigned int _frame)
{
	m_CurrentAnimName = _name;
	m_BlendRate = 0.0f;
	//�t���[���ő吔�𒴂��Ȃ��悤�ɂ���
	m_Animation[_name].frame = _frame < m_Animation[_name].animation->mAnimations[0]->mNumChannels ?
		_frame : 0;
}

void Component_SkeletalMesh::AddBlendRate(float _rate)
{
	m_BlendRate += _rate;

	if (m_BlendRate >= 1.0f)
	{
		m_BlendRate = 0.0f;
		//���̃A�j���[�V���������݂̃A�j���[�V�����ɂ���
		m_CurrentAnimName = m_NextAnimName;

		//���̃A�j���[�V�����𖳂��ɐݒ肷��
		m_NextAnimName = "none";
		m_Animation[m_NextAnimName].frame = 0;
	}
}

void Component_SkeletalMesh::SetNextAnimationName(std::string _name, unsigned int _frame)
{
	//���ɃZ�b�g���悤�Ƃ��Ă���A�j���[�V�����������Ȃ�Z�b�g����K�v�͖���
	if (m_CurrentAnimName == _name)
	{
		m_BlendRate = 0.0f;
		return;
	}

	//�Z�b�g�������A�j���[�V�������A���Ɏ��̃A�j���[�V�����ɃZ�b�g����Ă�����u�����h�����Z
	if (m_NextAnimName == _name)
	{
		AddBlendRate(m_UpdateFrameTime);
		return;
	}
	else
	{
		m_BlendRate = 0.0f;
	}

	m_NextAnimName = _name;
	//�t���[���ő吔�𒴂��Ȃ��悤�ɂ���
	m_Animation[_name].frame = _frame < m_Animation[_name].animation->mAnimations[0]->mNumChannels ?
		_frame : 0;
}

void Component_SkeletalMesh::SetAnimation(std::string _start,
	std::string _end, unsigned int _startFrame, unsigned int _endFrame)
{
	m_CurrentAnimName = _start;
	m_Animation[_start].frame = _startFrame < m_Animation[_start].animation->mAnimations[0]->mNumChannels ? _startFrame : 0;
	m_NextAnimName = _end;
	m_Animation[_end].frame = _startFrame < m_Animation[_end].animation->mAnimations[0]->mNumChannels ? _startFrame : 0;
	m_BlendRate = 0.0f;
}

void Component_SkeletalMesh::SetCurrentAnimationFrame(unsigned int _frame)
{
	m_Animation[m_CurrentAnimName].frame = _frame;
	if (m_Animation[m_CurrentAnimName].frame >= m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels)
	{
		m_Animation[m_CurrentAnimName].frame = 0;
	}
}

void Component_SkeletalMesh::SetNextAnimationFrame(unsigned int _frame)
{
	m_Animation[m_NextAnimName].frame = _frame;
	if (m_Animation[m_NextAnimName].frame >= m_Animation[m_NextAnimName].animation->mAnimations[0]->mNumChannels)
	{
		m_Animation[m_NextAnimName].frame = 0;
	}
}

unsigned int Component_SkeletalMesh::GetCurrentAnimationPlayTime()
{
	if (m_CurrentAnimName == "none" ||
		m_Animation.count(m_CurrentAnimName) == 0 ||
		!m_Animation[m_CurrentAnimName].animation->HasAnimations())
	{
		//�A�j���[�V�����������ꍇ
		return 0;
	}

	return  m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels;
}

unsigned int Component_SkeletalMesh::GetNextAnimationPlayTime()
{
	if (m_NextAnimName == "none" ||
		m_Animation.count(m_NextAnimName) == 0 ||
		!m_Animation[m_NextAnimName].animation->HasAnimations())
	{
		return 0;
	}

	return  m_Animation[m_NextAnimName].animation->mAnimations[0]->mNumChannels;
}

bool Component_SkeletalMesh::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_SkeletalMesh>(_fileName, this, sizeof(Component_SkeletalMesh), tempName))
	{
		return false;
	}
	return true;
}

bool Component_SkeletalMesh::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_SkeletalMesh>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}

//������
void Component_SkeletalMesh::Init()
{
	m_ShaderType = Manager_Shader::SHADER_UNLIT;//�f�t�H���g�̓A�����b�g�V�F�[�_�[

	Component::Init();//��ꏉ����

	m_BlendRate = 0.0f;//�u�����h���[�g
	m_ScaleRate = 1.0f;//�T�C�Y��

	m_CurrentAnimName = "none";
	m_Animation[m_CurrentAnimName].frame = 0;
	m_NextAnimName = "none";
	m_Animation[m_NextAnimName].frame = 0;
	m_UpdateFrameTime = 0.05f;

	m_PositionOffset = Vector3_Zero;
	m_RotationOffset = Vector3_Zero;

	m_DeformVertex = nullptr;

	m_ModelColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Manager_ImGui::Instance()->GetMainDebugWindow()->AddColorPicker("characterColor", m_ModelColor);

	if (Manager_Model* manager_FBX = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Model())
	{//�}�l�[�W���[�擾�ł�����o�^
		manager_FBX->AddComponent(this);
	}
}

//�I��
void Component_SkeletalMesh::Uninit()
{
	Component_StaticMesh::Uninit();

	if (m_DeformVertex)delete[] m_DeformVertex;

	for (std::pair<std::string, ID3D11ShaderResourceView*>pair : m_Texture)
	{
		if (pair.second)pair.second->Release();
	}
	for (auto Animation : m_Animation)
	{
		aiReleaseImport(Animation.second.animation);
	}
}
//�X�V
void Component_SkeletalMesh::Update()
{
	//�A�j���[�V�����̌��ݎ��ԍX�V
	m_Animation[m_CurrentAnimName].frame++;
	if (m_Animation[m_CurrentAnimName].frame >= INT_MAX)m_Animation[m_CurrentAnimName].frame = 0;//�I�[�o�[�t���[�΍�
	if (m_NextAnimName != "none")
	{
		m_Animation[m_NextAnimName].frame++;
		if (m_Animation[m_NextAnimName].frame >= INT_MAX)m_Animation[m_NextAnimName].frame = 0;//�I�[�o�[�t���[�΍�
	}

	//int�̍ő�l�𒴂��Ȃ��悤�ɂ���
	//if (m_Frame >= INT_MAX)
	//{
	//	m_Frame = 0;
	//}

	//�A�j���[�V�����̃f�[�^���Ȃ�������RETURN�B
	if (m_CurrentAnimName == "none" ||//���݂�ANIM�����ݒ�
		m_Animation.count(m_CurrentAnimName) == 0 ||//�z��ɐݒ肳�ꂽANIM�L�[������
		!m_Animation[m_CurrentAnimName].animation->HasAnimations())//ANIM�̒��g������
	{
		return;
	}

	//���ɑJ�ڂ���A�j���[�V�������Z�b�g����Ă��邩
	bool tempIsUseNextAnim = false;
	if (!(m_NextAnimName == "none" ||
		m_Animation.count(m_NextAnimName) == 0 ||
		!m_Animation[m_NextAnimName].animation->HasAnimations()))
	{
		tempIsUseNextAnim = true;
		AddBlendRate(m_UpdateFrameTime);//�u�����h���X�V
		if (m_NextAnimName == "none")
		{
			//�u�����h����1�ɂȂ���Next��none�ɂȂ�����u�����h���Ȃ�
			tempIsUseNextAnim = false;
		}
	}

	//�J�ڐ�̃A�j���[�V�������Z�b�g����Ă��鎞�A�u�����h�X�V�����A�j���[�V�������Z�b�g
	if (tempIsUseNextAnim)
	{
		aiAnimation* animation1 = m_Animation[m_CurrentAnimName].animation->mAnimations[0];
		aiAnimation* animation2 = m_Animation[m_NextAnimName].animation->mAnimations[0];

		unsigned int tempMin = m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels < m_Animation[m_NextAnimName].animation->mAnimations[0]->mNumChannels ?
			m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels : m_Animation[m_NextAnimName].animation->mAnimations[0]->mNumChannels;

		for (unsigned int c = 0; c < m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim1 = animation1->mChannels[c];//�J�����g�̃m�[�h��0���珇�ɓo�^
			aiNodeAnim* nodeAnim2 = nullptr;//�J�ڐ�̃A�j���[�V�����̓J�����g�̃m�[�h�ƍ��킹�邽�߂�for�Ō������Ĉ�v����m�[�h��T��

			bool isHit = false;
			for (unsigned int k = 0; k < m_Animation[m_NextAnimName].animation->mAnimations[0]->mNumChannels; k++)
			{
				//�J�����g�̃m�[�h���ƈ�v
				if (animation2->mChannels[k]->mNodeName == nodeAnim1->mNodeName)
				{
					nodeAnim2 = animation2->mChannels[k];
					isHit = true;
					break;
				}
			}
			if (!isHit)continue;

			s_BONE* bone = &m_Bone[m_BoneTable[nodeAnim1->mNodeName.C_Str()]];
			int f;
			f = m_Animation[m_CurrentAnimName].frame % nodeAnim1->mNumRotationKeys;//�ȈՎ���
			aiQuaternion rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = m_Animation[m_CurrentAnimName].frame % nodeAnim1->mNumPositionKeys;//�ȈՎ���
			aiVector3D pos1 = nodeAnim1->mPositionKeys[f].mValue;

			f = m_Animation[m_NextAnimName].frame % nodeAnim2->mNumRotationKeys;//�ȈՎ���
			aiQuaternion rot2 = nodeAnim2->mRotationKeys[f].mValue;

			f = m_Animation[m_NextAnimName].frame % nodeAnim2->mNumPositionKeys;//�ȈՎ���
			aiVector3D pos2 = nodeAnim2->mPositionKeys[f].mValue;

			aiVector3D pos = pos1 * (1.0f - m_BlendRate) + pos2 * m_BlendRate;
			aiQuaternion rot;
			aiQuaternion::Interpolate(rot, rot1, rot2, m_BlendRate);

			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
		}
		UpdateBoneMatrix(m_AiScene->mRootNode, aiMatrix4x4());
	}
	else
	{
		//���̃A�j���[�V�������Z�b�g����Ă��Ȃ��Ƃ�
		//���݂̃A�j���[�V���������[�v����
		aiAnimation* animation1 = m_Animation[m_CurrentAnimName].animation->mAnimations[0];

		for (unsigned int c = 0; c < m_Animation[m_CurrentAnimName].animation->mAnimations[0]->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim1 = animation1->mChannels[c];
			s_BONE* bone = &m_Bone[m_BoneTable[nodeAnim1->mNodeName.C_Str()]];
			int f;
			f = m_Animation[m_CurrentAnimName].frame % nodeAnim1->mNumRotationKeys;//�ȈՎ���
			aiQuaternion rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = m_Animation[m_CurrentAnimName].frame % nodeAnim1->mNumPositionKeys;//�ȈՎ���
			aiVector3D pos1 = nodeAnim1->mPositionKeys[f].mValue;
			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot1, pos1);
		}
		UpdateBoneMatrix(m_AiScene->mRootNode, aiMatrix4x4());
	}

	//���_��񏑂�����
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];
		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer_DX11::Instance()->GetDeviceContext()->Map(m_VertexBuffer[m], 0,
			D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D *vertex = (VERTEX_3D*)ms.pData;

		s_DFEFORM_VERTEX *deformVertex;
		aiMatrix4x4 matrix[4];
		aiMatrix4x4 outMatrix;
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			deformVertex = &m_DeformVertex[m][v];

			matrix[0] = m_Bone[deformVertex->BoneIndex[0]].Matrix;
			matrix[1] = m_Bone[deformVertex->BoneIndex[1]].Matrix;
			matrix[2] = m_Bone[deformVertex->BoneIndex[2]].Matrix;
			matrix[3] = m_Bone[deformVertex->BoneIndex[3]].Matrix;
			{
				OUT_MATRIX_MACRO(a);
				OUT_MATRIX_MACRO(b);
				OUT_MATRIX_MACRO(c);
				OUT_MATRIX_MACRO(d);
			}
			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;

			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;

			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			//vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[v].Diffuse = m_ModelColor;
		}
		Renderer_DX11::Instance()->GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
	}
}

void Component_SkeletalMesh::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	s_BONE* bone = &m_Bone[m_BoneTable[node->mName.C_Str()]];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

//�`��
void Component_SkeletalMesh::Draw()
{
	//�V�F�[�_�[�ݒ�
	Manager_Shader::Instance()->SetShader(m_ShaderType);

	if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3D�ɃL���X�g
	{
		if (!actor3D->GetIsActive() || !m_Active)return;//��A�N�e�B�u�Ȃ�`�悵�Ȃ�

		Vector3 ownerPostion = actor3D->GetPosition();//�I�[�i�[�̍��W�擾
		Vector3 ownerRotation = actor3D->GetRotation();//�I�[�i�[�̉�]�擾
		Vector3 ownerScale = actor3D->GetScale();   //�I�[�i�[�̃T�C�Y�擾

		ownerScale = ownerScale * m_ScaleRate;//�`��T�C�Y����

		if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
		{//�J����������ꍇ
			if (!camera->CheckView(ownerPostion, ownerScale))
			{//������J�����O
				//return;
			}
		}

		// �}�g���N�X�ݒ�
		D3DXMATRIX world, scale, rot, trans, offsetTrans, offsetRot;
		D3DXMatrixScaling(&scale, ownerScale.x, ownerScale.y, ownerScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, ownerRotation.y, ownerRotation.x, ownerRotation.z);
		D3DXMatrixTranslation(&trans, ownerPostion.x, ownerPostion.y, ownerPostion.z);
		D3DXMatrixTranslation(&offsetTrans, m_PositionOffset.x, m_PositionOffset.y, m_PositionOffset.z);
		D3DXMatrixRotationYawPitchRoll(&offsetRot, m_RotationOffset.y, m_RotationOffset.x, m_RotationOffset.z);
		world = scale * offsetTrans * offsetRot * rot * trans;

		Renderer_DX11::Instance()->SetWorldMatrix(&world);

		actor3D->SetMatrix(world);
	}

	Renderer_DX11::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer_DX11::Instance()->SetMaterial(material);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiString path;
		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		//if (m_AiScene->mTextures)
		if (m_Texture[path.data])
		{
			Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[path.data]);
		}
		else
		{
			Renderer_Polygon::Instance()->SetDefaultTexture();
		}

		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer_DX11::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		Renderer_DX11::Instance()->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		Renderer_DX11::Instance()->GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}
