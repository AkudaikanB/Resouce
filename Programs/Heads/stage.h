//	ステージオブジェクト（=Block:足場/壁/天井,以下ブロック）・ゴール判定

#ifndef STAGE_H	//	多重定義防止
#define STAGE_H

//	ファイル結合
#include "game.h"

//	マクロ/定数定義
#define NUM_MaxBlock	(256)	//	ブロックの数
#define NUM_VtxBlock	(24)	//	ブロックの頂点数（6面x4）
#define NUM_plgBlock	(22)	//	ブロックのポリゴン数(縮退ポリゴン含む)

//	プロトタイプ宣言
void InitStage(LPDIRECT3DDEVICE9 pDevice);	//	初期化
void UninitStage(void);						//	メモリ開放
void UpdateStage(void);						//	更新
void DrawStage(LPDIRECT3DDEVICE9 pDevice);	//	描画

int CreateBlock(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 siz);	//	pos位置にsiz.x*siz.y*siz.zの直方体を生成
void EraseBlock(int Index);
D3DXVECTOR3 Get_PosBlock(int manBlock);	//	指定されたブロックの位置を取得
D3DXVECTOR3 Get_SizBlock(int manBlock);	//	指定されたブロックの大きさを取得
bool Get_flgBlock(int);

void Set_numEraseByGoal(int nStage,int nGoal);	//	足場消去に対応するゴールを設定
void EraseBlockByGoal(int nGoal);				//	nGoal番目のゴールに対応するブロック消去

#endif