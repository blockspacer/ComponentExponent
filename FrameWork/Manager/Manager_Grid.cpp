

//******************************************************************************
// インクルード
//******************************************************************************
#include "../System/Main.h"
#include "../System/MainWindow.h"
#include "../Renderer/Renderer_Polygon.h"
#include "Manager_Grid.h"

//******************************************************************************
// 初期化
//******************************************************************************
void Manager_Grid2D::Init()
{
	color = Color(1, 1, 1, 1);
	centerColor = Color(1, 0, 0, 1);

	cacheMain = MainWindow::Instance();
}

//******************************************************************************
// 描画
//******************************************************************************
void Manager_Grid2D::Draw()
{
	//デバッグ時のみ機能
#ifndef _DEBUG
	return;
#endif

	//横の線
	Color tempColor = color;
	for (int i = 0; i < lineNum.y; i++)
	{
		if (lineNum.y % 2 == 0)
		{
			//偶数なら真ん中が二本
			tempColor = (lineNum.y / 2 == (i + 1) || lineNum.y / 2 + 1 == (i + 1)) ? centerColor : color;
		}
		else
		{
			//奇数なら真ん中が取れる
			tempColor = lineNum.y / 2 == i ? centerColor : color;
		}

		Renderer_Polygon::Instance()->DrawLine2D({ 0.0f,scale.y * i + scale.y }, 
			{ static_cast<float>(cacheMain->GetWindowSize().x),scale.y * i + scale.y }, tempColor);
	}

	//縦の線
	for (int i = 0; i < lineNum.x; i++)
	{
		if (lineNum.x % 2 == 0)
		{
			//偶数なら真ん中が二本
			tempColor = (lineNum.x / 2 == (i + 1) || lineNum.x / 2 + 1 == (i + 1)) ? centerColor : color;
		}
		else
		{
			//奇数なら真ん中が取れる
			tempColor = lineNum.x / 2 == i ? centerColor : color;
		}

		Renderer_Polygon::Instance()->DrawLine2D({ scale.x * i + scale.x,0.0f }, 
			{ scale.x * i + scale.x,static_cast<float>(cacheMain->GetWindowSize().y) }, tempColor);
	}
}

//******************************************************************************
// 幅から線の本数とスケールを計算
//******************************************************************************
void Manager_Grid2D::SetLength(float _len)
{
	//先の本数を計算
	lineNum.x = cacheMain->GetWindowSize().x / static_cast<int>(_len);
	lineNum.y = cacheMain->GetWindowSize().y / static_cast<int>(_len);

	//スケールセット
	scale.x = scale.y = _len;
}

//******************************************************************************
// 幅から線の数を計算
//******************************************************************************
void Manager_Grid2D::SetScale(Vector2 _scale)
{
	//先の本数を計算
	lineNum.x = cacheMain->GetWindowSize().x / static_cast<int>(_scale.x);
	lineNum.y = cacheMain->GetWindowSize().y / static_cast<int>(_scale.y);

	//スケールセット
	scale = _scale;
}

//******************************************************************************
// 縦と横で指定された分割数に合わせたスケールと線の本数に計算
//******************************************************************************
void Manager_Grid2D::SetCutNum(Int2 _cut)
{
	scale.x = static_cast<float>(cacheMain->GetWindowSize().x / (_cut.x <= 0 ? 1 : _cut.x));
	scale.y = static_cast<float>(cacheMain->GetWindowSize().y / (_cut.y <= 0 ? 1 : _cut.y));

	lineNum.x = cacheMain->GetWindowSize().x / static_cast<int>(scale.x) - 1;
	lineNum.y = cacheMain->GetWindowSize().y / static_cast<int>(scale.y) - 1;
}
