//	環境光	 //
#ifndef	LIGHT_H
#define LIGHT_H

//	ファイル結合
#include "main.h"

//	構造体定義

//	マクロ/定数定義
const D3DXVECTOR3 INIT_vecLightDirectional(2.0f,-1.0f,3.0f);	//	全体的な光の初期方向

//	プロトタイプ宣言
void InitLight		(LPDIRECT3DDEVICE9 pDevice);	//	起動時設定
void UpdateLight	(LPDIRECT3DDEVICE9 pDevice);	//	更新

void ChangeLight(LPDIRECT3DDEVICE9 pDevice);	//	ライトを切り替える

#endif