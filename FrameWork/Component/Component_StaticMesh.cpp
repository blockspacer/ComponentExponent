//=============================================================================
//
//  静的メッシュコンポーネント [Component_StaticMesh.cpp]
//  Date   : 2021/12/08
//
//=============================================================================


#include "../System/Main.h"
#include "Component_StaticMesh.h"
#include "../Actor/3D/Base/Actor3D.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Renderer/Renderer_Polygon.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Shader.h"
#include "../Actor/3D/Camera.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Drawer/Manager_Model.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"

void Component_StaticMesh::Init()
{
	m_ShaderType = Manager_Shader::SHADER_TYPE::SHADER_UNLIT;

	m_PositionOffset = Vector3_Zero;
	m_RotationOffset = Vector3_Zero;

	m_ScaleRate = 1.0f;

	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;

	m_ModelColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	/*Manager_ImGui::Instance()->GetMainDebugWindow()->AddColorPicker("static_Test", m_ModelColor);*/

	if (Manager_Model* manager_model = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Model())
	{//マネージャー取得できたら登録
		manager_model->AddComponent(this);
	}
}

void Component_StaticMesh::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}
	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;

	aiReleaseImport(m_AiScene);
	m_AiScene = nullptr;

	if (Manager_Model* manager_model = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Model())
	{//マネージャー取得できたら削除
		manager_model->RemoveComponent(this);
	}
}

void Component_StaticMesh::Draw()
{
	//シェーダー設定
	Manager_Shader::Instance()->SetShader(m_ShaderType);

	if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3Dにキャスト
	{
		if (!actor3D->GetIsActive() || !m_Active)return;//非アクティブなら描画しない

		Vector3 ownerPostion = actor3D->GetPosition();//オーナーの座標取得
		Vector3 ownerRotation = actor3D->GetRotation();//オーナーの回転取得
		Vector3 ownerScale = actor3D->GetScale();   //オーナーのサイズ取得

		ownerScale = ownerScale * m_ScaleRate;//描画サイズ調整

		if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
		{//カメラがある場合
			if (!camera->CheckView(ownerPostion, ownerScale))
			{//視錐台カリング
				//return;
			}
		}
		//this->SetColor(this->m_ModelColor);

		// マトリクス設定
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

		aiString path = {};
		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		//if (m_AiScene->mTextures)
		if (m_Texture[path.data])
		{
			Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[path.data]);
		}
		else
		{
			//todo:適当なテクスチャを当てないと見えない。

			//static ID3D11ShaderResourceView* tempTexture = nullptr;
			//if (!tempTexture)
			//{
			//	D3DX11CreateShaderResourceViewFromFile(Renderer_DX11::Instance()->GetDevice(),
			//		"../ROM/2D/Error/T_Error.png",
			//		NULL,
			//		NULL,
			//		&tempTexture,
			//		NULL);
			//}
			//Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &tempTexture);

			Renderer_Polygon::Instance()->SetDefaultTexture();
		}

		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer_DX11::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		Renderer_DX11::Instance()->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		Renderer_DX11::Instance()->GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void Component_StaticMesh::LoadStaticMesh(const char * _fileName, std::string _texName)
{
	if (m_AiScene)
	{
		aiReleaseImport(m_AiScene);
		m_AiScene = nullptr;
	}

	m_AiScene = aiImportFile(_fileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	if (m_AiScene == nullptr)
	{
		char temp[512] = {};
		sprintf(temp, "%s ファイルがロード出来ませんでした。", _fileName);
		MessageBox(NULL, "FBXが存在しないか、ロードエラー", "ERROR", MB_OK);
		return;
	}

	m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd = {};
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

				D3D11_BUFFER_DESC bd = {};
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * mesh->mNumFaces * 3;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd = {};
				//ZeroMemory(&sd, sizeof(sd));
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
				//テクスチャのパスが引数にあったらそれをロード
				if (_texName != "none")
				{
					//テクスチャロード
					if (m_Texture[path.data] = Manager_Texture::Instance()->LoadTexture(_texName.c_str(), _texName.c_str()))
					{
						continue;
					}
				}
				else//テクスチャが指定されてないとき
				{
					ID3D11ShaderResourceView* texture;
					if (path.data[0] == '*')//テクスチャの名前の先頭文字が*なら（なにかのソフトの書き出しの決まりみたいな感じ？）
					{
						if (m_Texture[path.data] == NULL)
						{
							int id = atoi(&path.data[1]);
							D3DX11CreateShaderResourceViewFromMemory(Renderer_DX11::Instance()->GetDevice(), (const unsigned char*)m_AiScene->mTextures[id]->pcData,
								m_AiScene->mTextures[id]->mWidth,
								NULL, NULL, &texture, NULL);
							m_Texture[path.data] = texture;
							Manager_Texture::Instance()->AddTexture(texture, path.data);
						}
					}
					else//テクスチャパスの引数が無く、fbxから取得したパスの先頭が*ではないとき
					{
						texture = nullptr;
						//ROMの文字が出るまでファイルの名前を調べていく
						bool isAdd = false;
						bool isFindROM = false;//テクスチャのパスがロム外かどうか
						std::string tempName = "../";
						for (unsigned int i = 0; i < path.length; i++)
						{
							if (path.data[i] == 'R' && path.data[i + 1] == 'O' && path.data[i + 2] == 'M')
							{
								isAdd = true;
								isFindROM = true;
							}

							if (isAdd)
							{
								tempName += path.data[i];
							}
						}
						if (isFindROM)//ROM内にテクスチャがあったらそれをロード
						{
							m_Texture[path.data] = Manager_Texture::Instance()->LoadTexture(tempName.c_str(), tempName.c_str());
							continue;
						}
						else//パスがROM外になっていたら、決め打ちで2D内にあるとしてロードを試みる
						{
							texture = nullptr;
							//パスからテクスチャ名だけを抜き出して2Dからロード
							tempName = "../ROM/2D/Test/";
							std::string tempFileName = "";
							for (int i = path.length; i > 0; i--)
							{
								if (path.data[i] == '/' && path.data[i - 1] == '/')
								{
									break;
								}
								else
								{
									tempFileName += path.data[i];
								}
							}
							for (int i = (int)tempFileName.length(); i > 0; i--)
							{
								tempName += tempFileName[i];
							}
							m_Texture[path.data] = Manager_Texture::Instance()->LoadTexture(tempName.c_str(), tempName.c_str());
							continue;
						}

						texture = nullptr;
						Manager_Texture::Instance()->GetTexture("Error");
						if (texture)
						{
							m_Texture[path.data] = texture;
							continue;
						}
						//テクスチャをロード出来なかった
						m_Texture[path.data] = Manager_Texture::Instance()->GetTexture("Error");
					}
				}
			}
			else//テクスチャ無し
			{
				int a = 0;
			}
		}
	}
}

void Component_StaticMesh::SetColor(Vector4 _color)
{
	m_ModelColor = _color;

	//色のために頂点書き換え
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer_DX11::Instance()->GetDeviceContext()->Map(m_VertexBuffer[m], 0,
			D3D11_MAP_WRITE_DISCARD, 0, &ms);
		VERTEX_3D *vertex = (VERTEX_3D*)ms.pData;

		aiMesh* mesh = m_AiScene->mMeshes[m];
		{
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = m_ModelColor;
			}
		}
		Renderer_DX11::Instance()->GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
	}
}

bool Component_StaticMesh::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_StaticMesh>(_fileName, this, sizeof(Component_StaticMesh), tempName))
	{
		return false;
	}
	return true;
}

bool Component_StaticMesh::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, sizeof(tempName), "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_StaticMesh>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}
