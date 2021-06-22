#pragma once

// ヘッダファイルを読み込み
#include "game.h"

// キーボードのヘッダファイル

// マクロ定義
#define KEY_KIND_MAX	256		// 取得するキーの種類

// キーボード構造体
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];		// 入力状態を管理
	int AllKeyState[KEY_KIND_MAX];			// 全てのキーの入力時間を管理
	int OldAllKeyState[KEY_KIND_MAX];		// 以前の全てのキーの入力時間を管理
};

// 外部のグローバル変数
extern KEYBOARD keyboard;

// プロトタイプ宣言
extern VOID AllKeyUpdate(VOID);								// 全てのキーの状態を取得する
extern BOOL KeyDown(int KEY_INPUT_);						// 特定のキーを押しているか?
extern BOOL KeyUp(int KEY_INPUT_);							// 特定のキーをあげているか?
extern BOOL KeyClick(int KEY_INPUT_);						// 特定のキーをクリックしたか?
extern BOOL KeyDownKeep(int KEY_INPUT_,int MillSec);		// 特定のキーを指定のミリ秒分押し続けているか?