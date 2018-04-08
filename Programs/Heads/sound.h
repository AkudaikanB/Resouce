//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	MUSIC_Title = 0,		// �^�C�g�����
	MUSIC_CoolTime,			// �N�[���^�C��
	MUSIC_Result,			// ���ʉ��
	MUSIC_Area00,			// �G���A00
	MUSIC_Area01,			// �G���A01
	MUSIC_Area02,			// �G���A02
	MUSIC_Area03,			// �G���A03
	MUSIC_Area04,			// �G���A04
	MUSIC_Area05,			// �G���A05
	MUSIC_Area06,			// �G���A06
	MUSIC_Area07,			// �G���A07
	MUSIC_Survival,			// �T�o�C�o���G���A
	SOUND_Step,			// �ڒn�ړ���
	SOUND_Jump,			// �W�����v��
	SOUND_Slide,		// �X���C�h��
	SOUND_Climb,		// �悶�o��
	SOUND_Over,
	SOUND_GetItem,		// �A�C�e���批
	SOUND_GoalClear,	// �S�[������
	SOUND_cngPhase,	// �S�[������
	SOUND_cngState,	// �S�[������
	SOUND_Starting,	// �S�[������
	SOUND_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
