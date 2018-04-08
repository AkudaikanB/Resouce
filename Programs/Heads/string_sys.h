//	ゲーム内文字列（UI）系


//	多重定義防止
#ifndef _STRING_
#define _STRING_


//	ファイル結合
#include "game.h"

//	プロトタイプ宣言
void InitString(LPDIRECT3DDEVICE9 pDevice);	//	初期化
void UninitString(void);					//	最初期化
void DrawString(LPDIRECT3DDEVICE9 pDevice);	//	描画

//void MakeString(int nManager, char* str);	//	文字列を作成


#endif