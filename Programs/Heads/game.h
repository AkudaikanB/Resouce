//	ゲームのメインの処理

#ifndef GAME_H	//	多重定義防止
#define GAME_H

//	ファイル結合
#include "main.h"	//	DirectX関連のものを結合

//	マクロ・定数定義
#define	NUM_ColorIndex	(16)
#define NUM_LastArea	(3)	//	最終エリアの番号
enum{
	IDX_phaTitle=0,	//	タイトル
	IDX_phaGaming,	//	エリア内
	IDX_phaResult	//	結果画面
};
enum {
	IDX_steSet=0,		//	準備中
	IDX_steGaming,		//	エリア内時
	IDX_steEmergency,	//	緊急メニュー時
	IDX_steCool			//	終了時
};
const	D3DXVECTOR3	INIT_Vector3D = D3DXVECTOR3(0.0f,0.0f,0.0f);	//	初期化用3次元情報
const	int			COL_redBack = 128,	//	背景色:	赤
					COL_greBack = 192,	//			緑
					COL_bluBack = 255;	//			青
const	D3DXCOLOR	COL_Back	= D3DCOLOR_RGBA(COL_redBack,COL_greBack,COL_bluBack,255);	//	背景色
const	D3DXCOLOR	COL_INDEX[] = {
	D3DCOLOR_RGBA(	255,255,255,64),	//	0:	白
	D3DCOLOR_RGBA(	255,0,	255,128),	//	1:	マゼンタ
	D3DCOLOR_RGBA(	0,	0,	255,128),	//	2:	青
	D3DCOLOR_RGBA(	192,192,0,	128),	//	3:	黄
	D3DCOLOR_RGBA(	0,	192,0,	128),	//	4:	緑
	D3DCOLOR_RGBA(	128,0,	192,128),	//	5:	紫
	D3DCOLOR_RGBA(	64,	32,	0,	192),	//	6:	茶
	D3DCOLOR_RGBA(	0,	255,255,128),	//	7:	空
	D3DCOLOR_RGBA(	255,255,128,192),	//	8:	金
	D3DCOLOR_RGBA(	255,128,192,96),	//	9:	桃
};
enum{
	IDX_colCommon = 0,//	汎用色
	IDX_colIndex,	//	強調色（黄）
	IDX_colDisable ,//	無効色（灰）
	IDX_colPositive,//	肯定色（緑）
	IDX_colDenial,	//	否定色（赤）
	IDX_colShadow,	//	影（灰）
};
const	D3DXCOLOR	COL_String[] = {
	D3DCOLOR_RGBA(	255,255,255,255),	//	汎用色
	D3DCOLOR_RGBA(	255,255,128,255),	//	強調色（黄）
	D3DCOLOR_RGBA(	192,192,192,255),	//	無効色（灰）
	D3DCOLOR_RGBA(	0,	255,64,	255),	//	肯定色（緑）
	D3DCOLOR_RGBA(	255,0,	0,	255),	//	否定色（赤）
	D3DCOLOR_RGBA(	64,	64,	64,	128)	//	影（灰）
};

//	構造体・クラス定義
class Object {
public:
	D3DXVECTOR3 pos;	//	位置
	D3DXVECTOR3 siz;	//	大きさ（半分の長さで持つ）
	D3DXVECTOR2	rot;	//	回転
	D3DXCOLOR	col;	//	色
};

//	プロトタイプ宣言
void InitGame		(LPDIRECT3DDEVICE9 pDevice);	//	各オブジェクトの起動時設定関数を呼び出し
void UninitGame		(void);							//	各オブジェクトの最初期化関数を呼び出し
void UpdateGame		(LPDIRECT3DDEVICE9 pDevice);	//	各オブジェクトの更新関数を呼び出し
void DrawGame		(LPDIRECT3DDEVICE9 pDevice);	//	各オブジェクトの描画関数を呼び出し

void InitDrawState	(LPDIRECT3DDEVICE9 pDevice);	//	描画用の共通値を設定
void InitVertex2D	(VERTEX_2D* pVtx);				//	2D描画用頂点情報の共通値を設定
void Create_recPolygon(VERTEX_2D* pVtx0,RECT rec,D3DXCOLOR col);	//	4角形2Dポリゴンを生成

void IncreaseScore	(int Score);	//	スコア加点
void IncreaseCount	(int Count);	//	カウント増加
void Reset_cntGoal	(int Manager);	//	ゴールのタイマーをリセット

void Reset_selMenu(void);	//	メニュー操作用のカーソルをリセット

float Move_10to1		(D3DXVECTOR3*pos,D3DXVECTOR3 dest);	//	距離を1/10詰める

int Get_numPhase	(void);	//	ゲームの状態を取得
int Get_numState	(void);	//	エリア内時の状態を取得
int Get_numInArea	(void);	//	現在のエリア番号を取得
int Get_numArea0	(void);	//	開始エリアの番号を取得
bool Get_flgSurvival(void);	//	ゲームモード(サバイバル:true)を返す
bool Get_flgReady	(void);	//	開始前準備確認用変数の取得

DWORD Get_timAll(void);		//	開始エリアからの合計エリア内時間を取得
DWORD Get_timOfArea(void);	//	現在のエリア内時間を取得
DWORD Get_timOfGoal(int);	//	ゴール発生時間を取得
int Get_numScore(void);		//	エリア内スコアを取得
int Get_numScoreA(void);	//	開始エリアからの合計スコアを取得

int Get_selMenu(void);		//	キーボード/ジョイスティックによるメニュー操作を行うカーソルの取得

bool Get_Floating(D3DXVECTOR3, D3DXVECTOR3);	//	非接触判定（非接触時trueを返す）

void Retry(void);	//	エリア内の時間をリセット

#endif