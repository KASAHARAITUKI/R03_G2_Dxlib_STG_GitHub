/*--+----1----+----2----+----3----+----4----+----5-----+----6----+----7----+----8----+----9----+---*/
//Copyright (c) 2020
//Hamamatsu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.

//図形のソースファイル

//########## ヘッダーファイル読み込み ##########
#include "game.h"
#include "shape.h"
#include "math.h"

//########## グローバル変数 ##########

//########## 関数 ##########

/// <summary>
/// 矩形領域同士の当たり判定をする関数
/// </summary>
/// <param name="a">領域A</param>
/// <param name="b">領域B</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//当たっている
	}

	return FALSE;		//当たっていない
}

/// <summary>
/// 四角と点の当たり判定
/// </summary>
/// <param name="pt">iPOINT型の点</param>
/// <param name="r">矩形領域</param>
/// <returns></returns>
BOOL CheckColliPointToRect(iPOINT pt, RECT r)
{
	if (
		pt.x > r.left && pt.x < r.right		//点のX位置が四角の左右の間にいて
		&& pt.y > r.top && pt.y < r.bottom	//点のY位置が四角の上下の間にいるとき
		)
	{
		return TRUE;
	}
	return FALSE;
}

/// <summary>
/// 円と点の当たり判定
/// </summary>
/// <param name="pt">iPOINT型の点</param>
/// <param name="m">円領域</param>
/// <returns></returns>
BOOL CheckColliPointToMaru(iPOINT pt, MARU m)
{
	//三角関数を使うよ！ c^2 = a^2 + b^2 なので、√c = √a + √b

	//三角形を求めよう！
	float a = pt.x - m.center.x;	//三角形の底辺の長さを求める
	float b = pt.y - m.center.y;	//三角形の高さの長さを求める
	float c = sqrtf(a * a + b * b);	//三角形の斜辺の長さを求める(sq rt = square root = 平方根)

	//斜辺の長さが、円の半径よりも短いならば、円の中にいる！
	if (c <= m.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// 円と円の当たり判定
/// </summary>
/// <param name="maru1">円領域1</param>
/// <param name="maru2">円領域2</param>
/// <returns></returns>
BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2)
{
	//三角関数を使うよ！ c^2 = a^2 + b^2 なので、√c = √a + √b

	//三角形を求めよう
	float a = maru1.center.x - maru2.center.x;	//三角形の底辺の長さを求める
	float b = maru1.center.y - maru2.center.y;	//三角形の高さの長さを求める
	float c = sqrtf(a * a + b * b);				//三角形の斜辺の長さを求める(sq rt = square root = 平方根)

	//斜辺の長さが、円の半径①＋円の半径②よりも短いならば、円の中にいる！
	if (c <= maru1.radius + maru2.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// RECT型を一時的に渡す
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <returns>RECT型</returns>
RECT GetRect(int left, int top, int right, int bottom)
{
	//一時的にRECT型の変数を作って
	RECT rect = { left ,top ,right ,bottom };

	//RECT型を返す
	return rect;
}

/// <summary>
/// RECTを利用して四角を描画
/// </summary>
/// <param name="r">RECT構造体</param>
/// <param name="color">描画する色</param>
/// <param name="b">中を塗りつぶさないならFALSE/塗りつぶすならTRUE</param>
/// <returns></returns>
VOID DrawRect(RECT r, unsigned int color, bool b)
{
	//引数を基に描画
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}

/// <summary>
/// MARUを利用して四角を描画
/// </summary>
/// <param name="c">MARU構造体</param>
/// <param name="color">描画する色</param>
/// <param name="b">中を塗りつぶさないならFALSE/塗りつぶすならTRUE</param>
/// <param name="thick">先の太さ</param>
/// <returns></returns>
VOID DrawMaru(MARU c, unsigned int color, bool b, int thick)
{
	//引数を基に描画
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}