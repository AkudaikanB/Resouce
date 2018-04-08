//	ゴール,目的地(=Goal)の制御

//	多重定義防止
#ifndef GOAL_H
#define GOAL_H


//	ファイル結合
#include "game.h"

//	マクロ定義
#define	NUM_MaxGoal	(10)		//	目的地の最大管理数
#define NUM_ItemToGoal (5)	//	ゴール条件アイテムの最大個数（汎用）
#define NUM_VtxGoal	(24)	//	目的地表示（直方体）の頂点数	
#define NUM_PlgGoal	(22)	//	目的地表示（直方体）のポリゴン数

//	プロトタイプ宣言
void InitGoal	(LPDIRECT3DDEVICE9 pDevice);	//	初期化
void UninitGoal	(void);							//	最初期化
void UpdateGoals(void);							//	更新
void DrawGoals	(LPDIRECT3DDEVICE9 pDevice);	//	描画

int CreateGoal(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 siz);	//	目的地を生成し、管理番号を返す
void EraseGoal		(int Manager);			//	目的地の無効化
void EraseGoalFLG	(int Manager);			//	目的地の無効化（サバイバル）
void EraseItemFLG	(int Manager);			//	アイテムの無効化（サバイバル）
void Set_numItemToGoal(int nGoal,int nItem);	//	nGoal番のゴールのボーナスカウント発生に対応するアイテム番号を設定
void SetGoalCount	(int Manager,bool set);	//	ゴール発生時間のカウンターを切り替える
void ChangeColorGoal(int nManager,int type);//	目的地の色を変更

int	Get_TouchedGoal(D3DXVECTOR3 pos,D3DXVECTOR3 siz);	//	目的地との接触判定
int Get_StartingGoal(int nItem);	//	アイテム管理番号に対応したゴールの値を返す
int Get_numItemOfGoal(int nManager);//	ゴール条件のアイテム数を取得
int Get_numGoal();					//	現存するゴールの数を取得
bool Get_flgGoalCount(int nManager);//	ゴールの現存判定を取得
bool Get_toGoal(int nManager);		//	ゴール条件判定
bool Get_Clear(void);				//	ゴールの現存数が0かどうかを判定
bool Get_flgGoNext(void);			//	エリア進行可否の取得


//	固定位置(Survival)
const D3DXVECTOR3 POS_SurvivalGoals[NUM_MaxGoal] = {
	D3DXVECTOR3(00.0f,100.0f,1000.0f),
	D3DXVECTOR3(1000.0f,100.0f,00.0f),
	D3DXVECTOR3(-1000.0f,100.0f,00.0f),
	D3DXVECTOR3(00.0f,100.0f,-1000.0f),
	D3DXVECTOR3(1000.0f,100.0f,1000.0f),
	D3DXVECTOR3(-1000.0f,100.0f,1000.0f),
	D3DXVECTOR3(1000.0f,100.0f,-1000.0f),
	D3DXVECTOR3(-1000.0f,100.0f,-1000.0f),
	D3DXVECTOR3(250.0f,500.0f,250.0f),
	D3DXVECTOR3(-250.0f,500.0f,-250.0f)
};
//	固定サイズ(Survival)
const D3DXVECTOR3 Siz_SurvivalGoals[NUM_MaxGoal] = {
	D3DXVECTOR3(250.0f,	100.0f,050.0f),
	D3DXVECTOR3(050.0f,	100.0f,250.0f),
	D3DXVECTOR3(050.0f,	100.0f,250.0f),
	D3DXVECTOR3(250.0f,	100.0f,050.0f),
	D3DXVECTOR3(250.0f,	100.0f,250.0f),
	D3DXVECTOR3(250.0f,	100.0f,250.0f),
	D3DXVECTOR3(250.0f,	100.0f,250.0f),
	D3DXVECTOR3(250.0f,	100.0f,250.0f),
	D3DXVECTOR3(250.0f,	100.0f,250.0f),
	D3DXVECTOR3(250.0f,	100.0f,250.0f)
};


#endif