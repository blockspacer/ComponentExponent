#pragma once

//******************************************************************************
// INCLUDE
//******************************************************************************
#include "../System/Singleton.h"

//******************************************************************************
// クラス定義
//******************************************************************************
class Manager_Grid2D : public Singleton<Manager_Grid2D>
{
private:

	friend Singleton<Manager_Grid2D>;

	Vector2	scale;//1マスのスケール
	Color color;//通常カラー
	Color centerColor;//真ん中の線のカラー
	Int2 lineNum;//線の数
	class MainWindow* cacheMain;//メインウィンドウキャッシュ

public:

	void Init();//初期化
	void Draw();//描画

	void SetLength(float _len);//正四角形のサイズ
	void SetScale(Vector2 _scale);//一つの四角形のサイズ
	void SetCutNum(Int2 _cut);//画面を何分割するか
	inline void SetColor(Color _color) { color = _color; }//カラーセット
	inline void SetCenterColor(Color _color) { centerColor = _color; }//カラーセット

};