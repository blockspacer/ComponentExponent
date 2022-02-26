//=============================================================================
//
//  ポリゴンレンダラー [Renderer_Polygon.h]
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once

#include "../System/Main.h"
#include "../Renderer/Renderer_DX11.h"
#include "../System/Singleton.h"

constexpr float MAX_CUT_START_POSTION = 1.0f;//最大切り取り座標

typedef struct 
{
	int     AnimCount;        //現在のフレーム数
	int     MaxAnimCount;     //何フレームで更新するか

	Vector2 OneFrameSize;     //1コマ辺りのサイズ
	Vector2 CutStartPostion;  //切り取り開始座標

	bool    Loop;//ループさせるか
}s_SpriteAnimation;

class Renderer_Polygon : public Singleton<Renderer_Polygon>
{
private:
	
	friend class Singleton<Renderer_Polygon>;//シングルトン
	class Renderer_DX11* m_Renderer_DX11 = nullptr;//DX11描画クラスキャッシュ

	ID3D11Buffer* m_CubePolygonVertexBuffer;//バーテックスバッファー
	ID3D11Buffer* m_LineVertexBuffer;//バーテックスバッファー
	ID3D11Buffer* m_VertexBuffer3DBox;//バーテックスバッファー3DBOX
	

	void SetCubePolygonVertexToDefault(VERTEX_3D* _vertex);//頂点情報をデフォルトに設定
	void Set3DBoxVertexToDefault(VERTEX_3D* _vertex, Vector3 _magni = Vector3(1.0f,1.0f,1.0f));//3D頂点情報をデフォルトに設定

	ID3D11ShaderResourceView* m_TextureWhite;//白テクスチャー
	class MainWindow* cacheMain;

public:

	 void Init();//初期化
	 void Uninit();//終了

	 void SetDefaultTexture();
	 inline ID3D11ShaderResourceView* GetDefaultTexture() { return m_TextureWhite; }
	

//=============================================================================
//
//  2D描画系
//
//=============================================================================

	//テクスチャなし2D矩形描画
	 void DrawBox2D(Vector2 _postion, Vector2 _scale,
		 Color _color, bool _reverse = false, float _z = 0.0f);

	 //2Dスプライト描画
	 void Draw2D(Vector2 _postion, Vector2 _scale, Color _color,
		 bool _reverse = false,float _z = 0.0f);//デフォルト引数は反転フラグとz座標

	 //2Dスプライトアニメーション描画
	 void Draw2DAnimation(Vector2 _position, Vector2 _scale, Color _color,
		 Vector2 _OneFrameSize, Vector2 _CutStartPostion,
		 bool _reverse = false);//最後のデフォルト引数は反転フラグ

	 //2Dライン
	 //※画面左上中心
	 void DrawLine2D(Vector2 _startPos, Vector2 _endPos, Color _color);

//=============================================================================
//
//  3D描画系
//
//=============================================================================

	 //ビルボード描画
	 void DrawBillboard(Vector3 _postion, Vector3 _scale, Color _color, 
		 Vector2 _OneFrameSize, Vector2 _CutStartPostion,bool _isUseDefaultTex = false);

	 //ボックス描画
	 void DrawBox(Vector3 _postion, Vector3 _Rotation,
		 Vector3 _scale, Color _color, float _texMagni = 1.0f);

	 //ライン描画 todo:描画出来ない
	 void DrawLine(Vector3 _start, Vector3 _end, Color _color);

	 void Draw3D(Vector3 _pos, Vector2 _scale, Vector3 _rot, Color _color, bool _isReverse = false);

	 void Draw3DAnimation(Vector3 _pos, Vector2 _scale,
		 Color _color, Vector2 _OneFrameSize, Vector2 _CutStartPostion,
		 bool _isUseDefaultTexture = false, bool _isReverse = false);
};
