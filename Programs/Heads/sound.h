//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	MUSIC_Title = 0,		// タイトル画面
	MUSIC_CoolTime,			// クールタイム
	MUSIC_Result,			// 結果画面
	MUSIC_Area00,			// エリア00
	MUSIC_Area01,			// エリア01
	MUSIC_Area02,			// エリア02
	MUSIC_Area03,			// エリア03
	MUSIC_Area04,			// エリア04
	MUSIC_Area05,			// エリア05
	MUSIC_Area06,			// エリア06
	MUSIC_Area07,			// エリア07
	MUSIC_Survival,			// サバイバルエリア
	SOUND_Step,			// 接地移動音
	SOUND_Jump,			// ジャンプ時
	SOUND_Slide,		// スライド時
	SOUND_Climb,		// よじ登り
	SOUND_Over,
	SOUND_GetItem,		// アイテム取音
	SOUND_GoalClear,	// ゴール消滅
	SOUND_cngPhase,	// ゴール消滅
	SOUND_cngState,	// ゴール消滅
	SOUND_Starting,	// ゴール消滅
	SOUND_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
