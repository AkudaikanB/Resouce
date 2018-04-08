//	プレイヤブルキャラクターの制御

#ifndef	PLAYER_H	//	多重定義防止
#define	PLAYER_H

//	ファイル結合
#include "game.h"
#include "item.h"

//	構造体・クラス定義
class Character :public Object {	//	プレイヤブルキャラクターの情報
public:
	int		mortion;	//	行動（体勢）
	D3DXVECTOR3 vec;	//	注視点へのベクトル（単位ベクトル）
	float	posEye;		//	目の位置（高さ）
	float	higStand;	//	立ち時身長
	float	higLay;		//	伏せ時高さ

	D3DXVECTOR3 vecFlow;	//	移動ベクトル
	float		spdJump;	//	ジャンプ力
	float		spdFall;	//	落下速度
	bool		flgRun;		//	歩き/駆け足スイッチ
	float		spdStep;	//	接地移動の速度

	bool		Item[NUM_MaxItem];	//	所持アイテム数
};

//	マクロ・定数・列挙型定義
const	D3DXVECTOR3	INIT_posPlayer	= D3DXVECTOR3(0.0f, 200.0f, 0.0f);
const	D3DXVECTOR3	INIT_vecPlayer	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
const	D3DXVECTOR3	INIT_sizPlayer	= D3DXVECTOR3(30.0f, 90.0f, 30.0f);
const	float		INIT_talEye		= 80.0f;
const	D3DXVECTOR2	INIT_rotPlayer	= D3DXVECTOR2(0.0f, 0.0f);
const	D3DXVECTOR3 INIT_vecFlow	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

enum{	//	行動（体勢）
	morStand = 0,	//	立ち
	morLay,			//	伏せ（ほふく）
	morClimb,		//	よじ登り
	morJump,		//	飛び跳ね
	morStagger,		//	ふらつき（ブレーキ時など）
	morSlide		//	滑り込み
};

//	プロトタイプ宣言
void InitPlayer(LPDIRECT3DDEVICE9 pDevice);	//	初期化
void UninitPlayer(void);					//	最初期化
void UpdatePlayer(void);					//	更新

void SetPlayer(D3DXVECTOR3 pos,	//	プレイヤブルキャラクターのエリア開始時設定
	float rot,int mortion
);

bool Get_flgItem(int nManager);	//	アイテムの所持判定
int Get_numItem(void);			//	アイテムの所持数を取得
bool Get_flgFallout(void);		//	落下終了判定を取得
D3DXVECTOR3 Get_posEye(void);
D3DXVECTOR3 Get_vecPlayer(void);
D3DXVECTOR2 Get_rotPlayer(void);
D3DXVECTOR3 Get_sizPlayer(void);

#endif