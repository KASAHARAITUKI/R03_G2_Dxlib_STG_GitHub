// ヘッダファイルの読み込み
#include "game.h"				// ゲーム全体のヘッダファイル
#include "keyboard.h"			// キーボードの処理
#include "FPS.h"				// FPSの処理

// マクロ定義
#define TAMA_DIV_MAX	4		// 弾の画像の最大数
#define TAMA_MAX		10		// 弾の総数

// 画像の構造体
struct IMAGE
{
	int handle = -1;			// 画像のハンドル(管理番号)
	char path[255];				// 画像の場所(パス)

	int x;						// X位置
	int y;						// Y位置
	int width;					// 幅
	int height;					// 高さ

	BOOL IsDraw = FALSE;		// 画像が描画できる？
};

// キャラクタの構造体
struct CHARACTOR
{
	IMAGE img;					// 画像構造体
	int speed = 1;				// 移動速度
	RECT coll;					// 当たり判定の領域(四角)
};

// 動画の構造体
struct MOVIE
{
	int handle = -1;			// 動画のハンドル
	char path[255];				// 動画のパス

	int x;						// X位置
	int y;						// Y位置
	int width;					// 幅
	int height;					// 高さ

	int Volume = 255;			// ボリューム(最小)0～255(最大)
};

// 音楽の構造体
struct AUDIO
{
	int handle = -1;			// 音楽のハンドル
	char path[255];				// 音楽のパス

	int Volume = -1;			// ボリューム(MIN 0 ～ 255 MAX)
	int playType = -1;		
};

// 弾の構造体
struct TAMA
{
	int handle[TAMA_DIV_MAX];	// 画像のハンドル
	char path[255];				// 画像のパス

	int DivTate;				// 分割数(縦)
	int DivYoko;				// 分割数(横)
	int DivMAX;					// 分割総数

	int AnimeCnt = 0;			// アニメーションカウンタ
	int AnimeCntMAX = 0;		// アニメーションカウンタMAX

	int NowIndex = 0;			// 現在の画像の要素数

	int x;						// X位置
	int y;						// Y位置
	int width;					// 幅
	int height;					// 高さ

	int Speed;					// 速度

	RECT coll;					// 当たり判定(矩形)

	BOOL IsDraw = FALSE;		// 描画できる？
};

// グローバル変数
// シーンを管理する変数
GAME_SCENE GameScene;			// 現在のゲームのシーン
GAME_SCENE OldGameScene;		// 前回のゲームのシーン
GAME_SCENE NextGameScene;		// 次のゲームのシーン

// 画面の切り替え
BOOL IsFadeOut = FALSE;			// フェードアウト
BOOL IsFadeIn = FALSE;			// フェードイン

int fadeTimeMill = 2000;						// 切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;		// ミリ秒をフレーム秒に変換

// フェードアウト
int fadeOutCntInit = 0;							// 初期値
int fadeOutCnt = fadeOutCntInit;				// フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;				// フェードアウトのカウンタMAX

// フェードイン
int fadeInCntInit = fadeTimeMax;				// 初期値
int fadeInCnt = fadeInCntInit;					// フェードインのカウンタ
int fadeInCntMax = fadeTimeMax;					// フェードインのカウンタMAX

// 弾の構造体
struct TAMA tama_moto;		// 元
struct TAMA tama[TAMA_MAX];	// 実際に使う

// 弾の発射カウンタ
int tamaShotCnt = 0;
int tamaShotCntMAX = 5;

// プロトタイプ宣言
VOID Title(VOID);			// タイトル画面
VOID TitleProc(VOID);		// タイトル画面(処理)
VOID TitleDraw(VOID);		// タイトル画面(描画)

VOID Play(VOID);			// プレイ画面
VOID PlayProc(VOID);		// プレイ画面(処理)
VOID PlayDraw(VOID);		// プレイ画面(描画)

VOID End(VOID);				// エンド画面
VOID EndProc(VOID);			// エンド画面(処理)
VOID EndDraw(VOID);			// エンド画面(描画)

VOID Change(VOID);			// 切り替え画面
VOID ChangeProc(VOID);		// 切り替え画面(処理)
VOID ChangeDraw(VOID);		// 切り替え画面(描画)

VOID ChangeScene(GAME_SCENE scene);				// シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);		// 当たり判定の領域を更新
VOID CollUpdate(CHARACTOR* chara);				// 当たり判定
VOID CollUpdateTama(TAMA* tama);				// 弾の当たり判定の更新

BOOL OnCollRect(RECT a, RECT b);				// 矩形と矩形の当たり判定

BOOL GameLoad(VOID);							// ゲームのデータの読み込み

BOOL LoadImageMem(IMAGE* image, const char* path);										// ゲームの画像を読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);				// ゲームの音楽を読み込み
BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate);	// ゲームの画像の分割読み込み

VOID GameInit(VOID);																	// ゲームのデータの初期化

VOID DrawTama(TAMA* tama);																// 弾の描画

// プログラムは WinMain から始まります
// windowsのプログラミング方法 = (WinAPI)で動いている！
// DxLibは、DirectXという、ゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				// Log.txtを出力しない
	ChangeWindowMode(TRUE);								// ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						// ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	// ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				// ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					// デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						// アイコンファイルを読み込み
	SetWindowStyleMode(GAME_WINDOW_BAR);				// ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								// ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								// ウィンドウをずっとアクティブにする


	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}


	// ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	// 最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	// ゲーム全体の初期化

	// ゲーム読み込み
	if (!GameLoad())
	{
		// データの読み込みに失敗したとき
		DxLib_End();		// DxLib終了
		return -1;			// 異常終了
	}

	// ゲームの初期化
	GameInit();

	// 無限ループ
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	// メッセージを受け取り続ける	
		if (ClearDrawScreen() != 0) { break; }	// 画面を消去する

		// キーボード入力の更新
		AllKeyUpdate();

		// FPS値の更新
		FPSUpdate();

		// ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		// 以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		// シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();
			break;
		case GAME_SCENE_PLAY:
			Play();
			break;
		case GAME_SCENE_END:
			End();
			break;
		case GAME_SCENE_CHANGE:
			Change();
			break;
		default:
			break;
		}

		// シーンを切り替える
		if (OldGameScene != GameScene)
		{
			// 現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		// 次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	// 画面切り替えシーンになる
			}
		}

		// FPS値を描画
		FPSDraw();

		// FPS値を待つ
		FPSWait();

		ScreenFlip();				// ダブルバッファリング下画面を描画
		
	}

	// 読み込んだ画像を開放
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(tama_moto.handle[i]); }

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0; 	// ソフトの終了
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE / 読み込めなかったらFALSE</returns>
BOOL GameLoad(VOID)
{
	// 弾の分割数を設定
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	// 弾のパス
	strcpyDx(tama_moto.path, ".\\image\\dia_green.png");

	// 画像を分割して読み込み
	if (LoadImageDivMem(&tama_moto.handle[0], tama_moto.path, tama_moto.DivYoko, tama_moto.DivTate) == FALSE) { return FALSE; }

	// 幅と高さを取得
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);

	// 位置を設定
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;		// 中央揃え
	tama_moto.y = GAME_HEIGHT - tama_moto.height;			// 画面下

	// 速度
	tama_moto.Speed = 1;

	// アニメを変える速度
	tama_moto.AnimeCntMAX = 10;

	// 当たり判定の更新
	CollUpdateTama(&tama_moto);

	// 画像を表示しない
	tama_moto.IsDraw = FALSE;

	// 全てのたまに情報をコピー
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	return TRUE;											// 全て読み込み成功
}

/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="bunkatuYoko">分割するときの横の数</param>
/// <param name="bunkatuTate">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate)
{
	// 弾の読み込み
	int IsTamaLoad = -1;									// 画像が読み込めたか

	// 一時的に画像のハンドルを用意する
	int TamaHandle = LoadGraph(path);

	// 読み込みエラー
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),							// ウィンドウハンドル
			path,											// 本文
			"画像読み込みエラー",							// タイトル
			MB_OK											// ボタン
		);

		return FALSE;										// 読み込み失敗
	}

	// 画像の幅と高さを取得
	int TamaWidth = -1;										// 幅
	int TamaHeight = -1;									// 高さ
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	// 分割して読み込み
	IsTamaLoad = LoadDivGraph(
		path,												// 画像のパス
		TAMA_DIV_MAX,										// 分割総数
		bunkatuYoko, bunkatuTate,												// 横の分割,縦の分割
		TamaWidth / bunkatuYoko, TamaHeight / bunkatuTate,	// 画像一つ分の幅,高さ
		handle												// 連続で管理する配列の先頭アドレス
	);

	// 分割エラー
	if (IsTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),							// ウィンドウハンドル
			path,											// 本文
			"画像分割エラー",								// タイトル
			MB_OK											// ボタン
		);

		

		return FALSE;										// 読み込み失敗
	}

	// 一時的に読み込んだハンドルを開放
	DeleteGraph(TamaHandle);

	return TRUE;
}

/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="image">画像構造体のアドレス</param>
/// <param name="path">画像のパス</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	// 画像の読み込み
	strcpyDx(image->path, path);	// パスのコピー
	image->handle = LoadGraph(image->path);			// 画像の読み込み

	// 画像が読み込めなかった時は、エラー(-1)が入る
	if (image->handle == -1)
	{
		MessageBox
		(GetMainWindowHandle(),			// メインのウィンドウハンドル
			image->path,					// メッセージ本文
			"画像読み込みエラー！",		// メッセージタイトル
			MB_OK						// ボタン
		);

		return FALSE;					// 読み込み失敗
	}

	// 画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	// 読み込めた
	return TRUE;
}

/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="audio">Audio構造体変数のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType) 
{
	// 音楽の読み込み
	strcpyDx(audio->path, path);	// パスのコピー
	audio->handle = LoadSoundMem(audio->path);			// 音楽の読み込み

	// 音楽が読み込めなかった時は、エラー(-1)が入る
	if (audio->handle == -1)
	{
		MessageBox
		(GetMainWindowHandle(),			// メインのウィンドウハンドル
			audio->path,					// メッセージ本文
			"音楽読み込みエラー！",		// メッセージタイトル
			MB_OK						// ボタン
		);

		return FALSE;					// 読み込み失敗
	}

	// その他の設定
	audio->Volume = volume;
	audio->playType= playType;

	// 読み込めた
	return TRUE;
}

VOID GameInit(VOID)
{
	
}

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;		// シーンを切り替え
	IsFadeIn = FALSE;		// フェードインしない
	IsFadeOut = TRUE;		// フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();		// 処理
	TitleDraw();		// 描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		// シーン切り替え
		// 次のシーンの初期化をここで行うと楽

		// ゲームの初期化
		GameInit();

		// プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 弾の描画
/// </summary>
/// <param name="tama">弾の構造体</param>
VOID DrawTama(TAMA* tama)
{
	// 弾の描画ができるとき
	if (tama->IsDraw == TRUE)
	{
		// 弾の描画
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		// 画像を変えるタイミング
		if (tama->AnimeCnt < tama->AnimeCntMAX)
		{
			tama->AnimeCnt++;
		}
		else
		{
			// 弾の添え字が弾の分割数の最大よりも小さいとき
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;
			}
			else
			{
				tama->NowIndex = 0;
			}

			tama->AnimeCnt = 0;
		}
	}

}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();		// 処理
	PlayDraw();		// 描画
	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	if (GAME_DEBUG == TRUE)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			// エンド画面に切り替え
			ChangeScene(GAME_SCENE_END);

			return;
		}
	}

	// スペースキーを押しているとき
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	{
		if (tamaShotCnt == 0)
		{
			// 弾を発射する(弾を描画する)
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					// 弾を発射する
					tama[i].IsDraw = TRUE;

					tama[i].x = GAME_WIDTH / 2 - tama[i].width / 2;
					tama[i].y = GAME_HEIGHT / 2 - tama[i].height / 2;

					// 弾の当たり判定の更新
					CollUpdateTama(&tama[i]);

					// 弾を1発出したらループを抜ける
					break;
				}
			}
		}
		
		// 弾の発射待ち
		if (tamaShotCnt < tamaShotCntMAX)
		{
			tamaShotCnt++;
		}
		else
		{
			tamaShotCnt = 0;
		}
	}

	// 弾を飛ばす
	for (int i = 0; i < TAMA_MAX; i++)
	{
		// 描画されているとき
		if (tama[i].IsDraw == TRUE)
		{
			// tama[i].x;
			tama[i].y -= tama[i].Speed;

			// 画面外に出たら描画しない
			if (tama[i].y + tama[i].height < 0 ||
				tama[i].y>GAME_HEIGHT ||
				tama[i].x + tama[i].width<0 ||
				tama[i].x>GAME_WIDTH)
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}
	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	// 弾の描画
	for (int i = 0; i < TAMA_MAX; i++)
	{
		// 描画されているとき
		if (tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();		// 処理
	EndDraw();		// 描画
	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		// シーン切り替え
		// 次のシーンの初期化をここで行うと楽

		// タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		// 処理
	ChangeDraw();		// 描画
	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	// フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	// カウンタを減らす
		}
		else
		{
			// フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;		// カウンタ初期化
			IsFadeIn = FALSE;				// フェードイン処理完了
		}
	}

	// フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	// カウンタを増やす
		}
		else
		{
			// フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;		// カウンタ初期化
			IsFadeOut = FALSE;				// フェードアウト処理完了
		}
	}

	// 切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		// フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;		// 次のシーンに切り替え
		OldGameScene = GameScene;		// 以前のゲームシーン更新
	}
	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	// 以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		// タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			// プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();			// エンド画面の描画
		break;
	default:
		break;
	}

	// フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	// フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	// 四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0),TRUE);

	// 半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 16, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新(プレイヤー)
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新(弾)
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;
	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形A</param>
/// <param name="b">矩形B</param>
/// <returns>あたったらTRUE/あたらないならFALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		a.left < b.right &&
		a.right > b.left &&
		a.top < b.bottom &&
		a.bottom > b.top
		)
	{
		// 当たっているとき
		return TRUE;
	}
	else
	{
		// 当たっていないとき
		return FALSE;
	}
}
