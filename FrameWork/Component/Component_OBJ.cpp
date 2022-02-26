

#include "../System/Main.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Actor/3D/Base/Actor3D.h"
#include "Component_OBJ.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Drawer/Manager_Model.h"
#include "ModelBase/Component_ModelBase.h"
#include "../Manager/Manager_Shader.h"
#include "../Actor/3D/Base/Actor3D.h"
#include "../Manager/Manager_Texture.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

void Component_OBJ::Init()
{
	if (Manager_Model* manager_model = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Model())
	{//マネージャー取得できたら登録
		manager_model->AddComponent(this);
	}
}

void Component_OBJ::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		m_SubsetArray[i].Material.Texture->Release();
	}

	delete[] m_SubsetArray;

	if (Manager_Model* manager_model = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Model())
	{
		manager_model->RemoveComponent(this);
	}
}

void Component_OBJ::Draw()
{
	//シェーダー設定
	Manager_Shader::Instance()->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

	if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))
	{
		Vector3 ownerPostion = actor3D->GetPosition();//オーナーの座標取得
		Vector3 ownerRotation = actor3D->GetRotation();//オーナーの回転取得
		Vector3 ownerScale = actor3D->GetScale();   //オーナーのサイズ取得

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

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer_DX11::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer_DX11::Instance()->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	Renderer_DX11::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		// マテリアル設定
		//Renderer_DX11::Instance()->SetMaterial(m_SubsetArray[i].Material.Material);

		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Renderer_DX11::Instance()->SetMaterial(material);

		// テクスチャ設定
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_SubsetArray[i].Material.Texture);

		// ポリゴン描画
		Renderer_DX11::Instance()->GetDeviceContext()->DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}
}

void Component_OBJ::Load(const char *_FileName)
{

	sModel model;
	LoadObj(_FileName, &model);

	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		Renderer_DX11::Instance()->GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		Renderer_DX11::Instance()->GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// サブセット設定
	{
		m_SubsetArray = new sSubset[model.SubsetNum];
		m_SubsetNum = model.SubsetNum;

		for (unsigned int i = 0; i < model.SubsetNum; i++)
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = model.SubsetArray[i].Material.Material;

			m_SubsetArray[i].Material.Texture = NULL;

			D3DX11CreateShaderResourceViewFromFile(Renderer_DX11::Instance()->GetDevice(),
				model.SubsetArray[i].Material.TextureName,
				NULL,
				NULL,
				&m_SubsetArray[i].Material.Texture,
				NULL);

			//テクスチャロードエラー
			if (!m_SubsetArray[i].Material.Texture)
			{
				m_SubsetArray[i].Material.Texture = Manager_Texture::Instance()->GetTexture("whiteTexture");
			}

			//assert(m_SubsetArray[i].Material.Texture);

		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;

}

bool Component_OBJ::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_OBJ>(_fileName, this, sizeof(Component_OBJ), tempName))
	{
		return false;
	}
	return true;
}

bool Component_OBJ::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_OBJ>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}

//モデル読込////////////////////////////////////////////
void Component_OBJ::LoadObj(const char *_FileName, sModel *_Model)
{

	char dir[MAX_PATH];
	strcpy(dir, _FileName);
	PathRemoveFileSpec(dir);

	D3DXVECTOR3	*positionArray;
	D3DXVECTOR3	*normalArray;
	D3DXVECTOR2	*texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	sModelMaterial	*materialArray = NULL;
	unsigned int	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen(_FileName, "rt");
	assert(file);

	//要素数カウント
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "v") == 0)
		{
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}

	//メモリ確保
	positionArray = new D3DXVECTOR3[positionNum];
	normalArray = new D3DXVECTOR3[normalNum];
	texcoordArray = new D3DXVECTOR2[texcoordNum];

	_Model->VertexArray = new VERTEX_3D[vertexNum];
	_Model->VertexNum = vertexNum;

	_Model->IndexArray = new unsigned int[indexNum];
	_Model->IndexNum = indexNum;

	_Model->SubsetArray = new sSubset[subsetNum];
	_Model->SubsetNum = subsetNum;

	//要素読込
	D3DXVECTOR3 *position = positionArray;
	D3DXVECTOR3 *normal = normalArray;
	D3DXVECTOR2 *texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "mtllib") == 0)
		{
			//マテリアルファイル
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			//オブジェクト名
			fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			//頂点座標
			fscanf(file, "%f", &position->x);
			fscanf(file, "%f", &position->y);
			fscanf(file, "%f", &position->z);
			position++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			//法線
			fscanf(file, "%f", &normal->x);
			fscanf(file, "%f", &normal->y);
			fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			//テクスチャ座標
			fscanf(file, "%f", &texcoord->x);
			fscanf(file, "%f", &texcoord->y);
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			//マテリアル
			fscanf(file, "%s", str);

			if (sc != 0)
				_Model->SubsetArray[sc - 1].IndexNum = ic - _Model->SubsetArray[sc - 1].StartIndex;

			_Model->SubsetArray[sc].StartIndex = ic;

			for (unsigned int i = 0; i < materialNum; i++)
			{
				if (strcmp(str, materialArray[i].Name) == 0)
				{
					_Model->SubsetArray[sc].Material.Material = materialArray[i].Material;
					strcpy(_Model->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
					strcpy(_Model->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;

		}
		else if (strcmp(str, "f") == 0)
		{
			//面
			in = 0;

			do
			{
				fscanf(file, "%s", str);

				s = strtok(str, "/");
				_Model->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//テクスチャ座標が存在しない場合もある
					s = strtok(NULL, "/");
					_Model->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(NULL, "/");
				_Model->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				_Model->VertexArray[vc].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

				_Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4)
			{
				_Model->IndexArray[ic] = vc - 4;
				ic++;
				_Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}

	if (sc != 0)
		_Model->SubsetArray[sc - 1].IndexNum = ic - _Model->SubsetArray[sc - 1].StartIndex;

	fclose(file);

	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

//マテリアル読み込み///////////////////////////////////////////////////////////////////
void Component_OBJ::LoadMaterial(const char *FileName, sModelMaterial **MaterialArray, unsigned int *MaterialNum)
{
	char dir[MAX_PATH] = {};
	memset(dir, 0, sizeof(dir));
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);

	char str[256];

	FILE *file;
	file = fopen(FileName, "rt");
	assert(file);

	sModelMaterial *materialArray;
	unsigned int materialNum = 0;

	//要素数カウント
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "newmtl") == 0)
		{
			materialNum++;
		}
	}

	//メモリ確保
	materialArray = new sModelMaterial[materialNum];

	//要素読込
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "newmtl") == 0)
		{
			//マテリアル名
			mc++;
			fscanf(file, "%s", materialArray[mc].Name);
			strcpy(materialArray[mc].TextureName, "");

			materialArray[mc].Material.Emission.r = 0.0f;
			materialArray[mc].Material.Emission.g = 0.0f;
			materialArray[mc].Material.Emission.b = 0.0f;
			materialArray[mc].Material.Emission.a = 0.0f;
		}
		else if (strcmp(str, "Ka") == 0)
		{
			//アンビエント
			fscanf(file, "%f", &materialArray[mc].Material.Ambient.r);
			fscanf(file, "%f", &materialArray[mc].Material.Ambient.g);
			fscanf(file, "%f", &materialArray[mc].Material.Ambient.b);
			materialArray[mc].Material.Ambient.a = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//ディフューズ
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.r);
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.g);
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.b);
			materialArray[mc].Material.Diffuse.a = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			//スペキュラ
			fscanf(file, "%f", &materialArray[mc].Material.Specular.r);
			fscanf(file, "%f", &materialArray[mc].Material.Specular.g);
			fscanf(file, "%f", &materialArray[mc].Material.Specular.b);
			materialArray[mc].Material.Specular.a = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//スペキュラ強度
			fscanf(file, "%f", &materialArray[mc].Material.Shininess);
		}
		else if (strcmp(str, "d") == 0)
		{
			//アルファ
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.a);
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//テクスチャ
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			//strcat(path, "\\");
			strcat(path, str);

			strcat(materialArray[mc].TextureName, path);
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}