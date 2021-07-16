#pragma once
/*--+----1----+----2----+----3----+----4----+----5-----+----6----+----7----+----8----+----9----+---*/
//Copyright (c) 2020
//Hamamatsu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.
#pragma once

//図形のヘッダファイル

//########## ヘッダーファイル読み込み ##########

//ゲームのメインヘッダファイル
#include "game.h"

//########## マクロ定義 ##########



//########## 構造体 ##########

//int型のPOINT構造体
struct iPOINT
{
	int x = -1;	//座標を初期化
	int y = -1;	//座標を初期化
};

//円の構造体
struct MARU
{
	iPOINT center;		//中心点
	int radius;		//半径
};

//########## 外部のグローバル変数 ##########


//########## プロトタイプ宣言 ##########
//※extern は、外部に関数がありますよ！という目印でつけています。

extern BOOL CheckCollRectToRect(RECT a, RECT b);					//四角領域の当たり判定をチェックする
extern BOOL CheckColliPointToRect(iPOINT pt, RECT r);				//点と四角の当たり判定
extern BOOL CheckColliPointToMaru(iPOINT pt, MARU m);				//点と円の当たり判定
extern BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2);			//円と円の当たり判定

extern RECT GetRect(int left, int top, int right, int bottom);		//RECT型を一時的に渡す
extern void DrawRect(RECT r, unsigned int color, bool b);			//RECTを利用して四角を描画
extern VOID DrawMaru(MARU c, unsigned int color, bool b, int thick = 1);	//MARUを利用して円を描画