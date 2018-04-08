
//-					-//--------------------------------------------------	
//	��b�\�z�w�b�_	 //
//-					-//--------------------------------------------------

#ifndef	MAIN_H	//	���d��`�h�~
#define	MAIN_H

#define USE_SOUND	// �T�E���h�̎g�p�錾

#include <windows.h>
#include <stdlib.h>		//	����
#include "d3dx9.h"		//	DirectX9 �Ŏg�p����w�b�_�[���C���N���[�h

#define DIRECTINPUT_VERSION	(0x0800)
#include "dinput.h"

#ifdef USE_SOUND
#include "xaudio2.h"	//  XAudio2�֘A�̃w�b�_�[
#endif

//	DirectX9 �Ŏg�p���郉�C�u�������C���N���[�h
#pragma comment (lib,	"d3d9.lib")
#pragma comment (lib,	"d3dx9.lib")
#pragma comment (lib,	"dxguid.lib")
#pragma comment (lib,	"dinput8.lib")
#pragma comment (lib,	"winmm.lib")


//	�}�N����`
#define	SIZE_ScreenWidth	(640)					//	��ʂ̉���
#define SIZE_ScreenHeight	(480)					//	��ʂ̏c��
#define POS_ScreenCenter_X	(SIZE_ScreenWidth/2)	//	��ʂ̒��S�ʒu(X��)
#define POS_ScreenCenter_Y	(SIZE_ScreenHeight/2)	//	��ʂ̒��S�ʒu(Y��)
#define NUM_BackBuffer		(1)

#define	NUM_VertexPolygon2D	(4)	//	2D�`�悳���I�u�W�F�N�g�̒��_��(�b��)
#define NUM_Polygon2D		(2)	//	2D�`�悳���I�u�W�F�N�g�̃|���S����(�b��)

//	-	���������[�N�̈��S�ȊJ���}�N��
#define SafeRelease(ptr)	{if(ptr){ptr->Release();ptr=NULL;}}

//	-	���_�t�H�[�}�b�g
#define FVF_VERTEX_2D	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )				//	���_���W/ ���_�J���[/ �e�N�X�`�����W
#define FVF_VERTEX_3D	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	//	���_���W/ �@���x�N�g�� / ���ˌ� / �e�N�X�`�����W

//	�\���̒�`
typedef struct {	//	2D���_�t�H�[�}�b�g�ɍ��킹���\����
	D3DXVECTOR3	pos;	//	��ʏ�̍��W
	float		rhw;	//	�������W�̔{��(?)
	D3DCOLOR	col;	//	���_�̐F
	D3DXVECTOR2	tex;	//	�e�N�X�`����̍��W
}VERTEX_2D;

typedef struct {	//	3D���_�t�H�[�}�b�g�ɍ��킹���\����
	D3DXVECTOR3 pos;	//	���z��ԏ�̍��W
	D3DXVECTOR3 vec;	//	�@���x�N�g��
	D3DCOLOR	def;	//	���ˌ�
	D3DXVECTOR2	tex;	//	�e�N�X�`����̍��W
}VERTEX_3D;

//	�v���g�^�C�v�錾
void QuitGame(void);
void SetMouseMode(bool);		//	�}�E�X�̈ړ�����
void SetScreenMode(void);		//	�X�N���[�����[�h�؂�ւ�
D3DXVECTOR2 Get_posMouse(void);	//	�}�E�X�́i��ʏ�́j�ʒu�擾�֐�
DWORD		Get_timLast(void);	//	�ŏI�X�V�������擾
void		IsolateMouse(void);		//	�}�E�X�̈ʒu�𗣂ꂽ�ʒu�ɐݒ�

//-					-//--------------------------------------------------
//	�\�[�X�I���ʒu	 //
//-					-//--------------------------------------------------

#endif