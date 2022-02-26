//=============================================================================
//
//  �|���S�������_���[ [Renderer_Polygon.h]
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once

#include "../System/Main.h"
#include "../Renderer/Renderer_DX11.h"
#include "../System/Singleton.h"

constexpr float MAX_CUT_START_POSTION = 1.0f;//�ő�؂�����W

typedef struct 
{
	int     AnimCount;        //���݂̃t���[����
	int     MaxAnimCount;     //���t���[���ōX�V���邩

	Vector2 OneFrameSize;     //1�R�}�ӂ�̃T�C�Y
	Vector2 CutStartPostion;  //�؂���J�n���W

	bool    Loop;//���[�v�����邩
}s_SpriteAnimation;

class Renderer_Polygon : public Singleton<Renderer_Polygon>
{
private:
	
	friend class Singleton<Renderer_Polygon>;//�V���O���g��
	class Renderer_DX11* m_Renderer_DX11 = nullptr;//DX11�`��N���X�L���b�V��

	ID3D11Buffer* m_CubePolygonVertexBuffer;//�o�[�e�b�N�X�o�b�t�@�[
	ID3D11Buffer* m_LineVertexBuffer;//�o�[�e�b�N�X�o�b�t�@�[
	ID3D11Buffer* m_VertexBuffer3DBox;//�o�[�e�b�N�X�o�b�t�@�[3DBOX
	

	void SetCubePolygonVertexToDefault(VERTEX_3D* _vertex);//���_�����f�t�H���g�ɐݒ�
	void Set3DBoxVertexToDefault(VERTEX_3D* _vertex, Vector3 _magni = Vector3(1.0f,1.0f,1.0f));//3D���_�����f�t�H���g�ɐݒ�

	ID3D11ShaderResourceView* m_TextureWhite;//���e�N�X�`���[
	class MainWindow* cacheMain;

public:

	 void Init();//������
	 void Uninit();//�I��

	 void SetDefaultTexture();
	 inline ID3D11ShaderResourceView* GetDefaultTexture() { return m_TextureWhite; }
	

//=============================================================================
//
//  2D�`��n
//
//=============================================================================

	//�e�N�X�`���Ȃ�2D��`�`��
	 void DrawBox2D(Vector2 _postion, Vector2 _scale,
		 Color _color, bool _reverse = false, float _z = 0.0f);

	 //2D�X�v���C�g�`��
	 void Draw2D(Vector2 _postion, Vector2 _scale, Color _color,
		 bool _reverse = false,float _z = 0.0f);//�f�t�H���g�����͔��]�t���O��z���W

	 //2D�X�v���C�g�A�j���[�V�����`��
	 void Draw2DAnimation(Vector2 _position, Vector2 _scale, Color _color,
		 Vector2 _OneFrameSize, Vector2 _CutStartPostion,
		 bool _reverse = false);//�Ō�̃f�t�H���g�����͔��]�t���O

	 //2D���C��
	 //����ʍ��㒆�S
	 void DrawLine2D(Vector2 _startPos, Vector2 _endPos, Color _color);

//=============================================================================
//
//  3D�`��n
//
//=============================================================================

	 //�r���{�[�h�`��
	 void DrawBillboard(Vector3 _postion, Vector3 _scale, Color _color, 
		 Vector2 _OneFrameSize, Vector2 _CutStartPostion,bool _isUseDefaultTex = false);

	 //�{�b�N�X�`��
	 void DrawBox(Vector3 _postion, Vector3 _Rotation,
		 Vector3 _scale, Color _color, float _texMagni = 1.0f);

	 //���C���`�� todo:�`��o���Ȃ�
	 void DrawLine(Vector3 _start, Vector3 _end, Color _color);

	 void Draw3D(Vector3 _pos, Vector2 _scale, Vector3 _rot, Color _color, bool _isReverse = false);

	 void Draw3DAnimation(Vector3 _pos, Vector2 _scale,
		 Color _color, Vector2 _OneFrameSize, Vector2 _CutStartPostion,
		 bool _isUseDefaultTexture = false, bool _isReverse = false);
};
