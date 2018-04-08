//	�Q�[���̃��C���̏���

#ifndef GAME_H	//	���d��`�h�~
#define GAME_H

//	�t�@�C������
#include "main.h"	//	DirectX�֘A�̂��̂�����

//	�}�N���E�萔��`
#define	NUM_ColorIndex	(16)
#define NUM_LastArea	(3)	//	�ŏI�G���A�̔ԍ�
enum{
	IDX_phaTitle=0,	//	�^�C�g��
	IDX_phaGaming,	//	�G���A��
	IDX_phaResult	//	���ʉ��
};
enum {
	IDX_steSet=0,		//	������
	IDX_steGaming,		//	�G���A����
	IDX_steEmergency,	//	�ً}���j���[��
	IDX_steCool			//	�I����
};
const	D3DXVECTOR3	INIT_Vector3D = D3DXVECTOR3(0.0f,0.0f,0.0f);	//	�������p3�������
const	int			COL_redBack = 128,	//	�w�i�F:	��
					COL_greBack = 192,	//			��
					COL_bluBack = 255;	//			��
const	D3DXCOLOR	COL_Back	= D3DCOLOR_RGBA(COL_redBack,COL_greBack,COL_bluBack,255);	//	�w�i�F
const	D3DXCOLOR	COL_INDEX[] = {
	D3DCOLOR_RGBA(	255,255,255,64),	//	0:	��
	D3DCOLOR_RGBA(	255,0,	255,128),	//	1:	�}�[���^
	D3DCOLOR_RGBA(	0,	0,	255,128),	//	2:	��
	D3DCOLOR_RGBA(	192,192,0,	128),	//	3:	��
	D3DCOLOR_RGBA(	0,	192,0,	128),	//	4:	��
	D3DCOLOR_RGBA(	128,0,	192,128),	//	5:	��
	D3DCOLOR_RGBA(	64,	32,	0,	192),	//	6:	��
	D3DCOLOR_RGBA(	0,	255,255,128),	//	7:	��
	D3DCOLOR_RGBA(	255,255,128,192),	//	8:	��
	D3DCOLOR_RGBA(	255,128,192,96),	//	9:	��
};
enum{
	IDX_colCommon = 0,//	�ėp�F
	IDX_colIndex,	//	�����F�i���j
	IDX_colDisable ,//	�����F�i�D�j
	IDX_colPositive,//	�m��F�i�΁j
	IDX_colDenial,	//	�ے�F�i�ԁj
	IDX_colShadow,	//	�e�i�D�j
};
const	D3DXCOLOR	COL_String[] = {
	D3DCOLOR_RGBA(	255,255,255,255),	//	�ėp�F
	D3DCOLOR_RGBA(	255,255,128,255),	//	�����F�i���j
	D3DCOLOR_RGBA(	192,192,192,255),	//	�����F�i�D�j
	D3DCOLOR_RGBA(	0,	255,64,	255),	//	�m��F�i�΁j
	D3DCOLOR_RGBA(	255,0,	0,	255),	//	�ے�F�i�ԁj
	D3DCOLOR_RGBA(	64,	64,	64,	128)	//	�e�i�D�j
};

//	�\���́E�N���X��`
class Object {
public:
	D3DXVECTOR3 pos;	//	�ʒu
	D3DXVECTOR3 siz;	//	�傫���i�����̒����Ŏ��j
	D3DXVECTOR2	rot;	//	��]
	D3DXCOLOR	col;	//	�F
};

//	�v���g�^�C�v�錾
void InitGame		(LPDIRECT3DDEVICE9 pDevice);	//	�e�I�u�W�F�N�g�̋N�����ݒ�֐����Ăяo��
void UninitGame		(void);							//	�e�I�u�W�F�N�g�̍ŏ������֐����Ăяo��
void UpdateGame		(LPDIRECT3DDEVICE9 pDevice);	//	�e�I�u�W�F�N�g�̍X�V�֐����Ăяo��
void DrawGame		(LPDIRECT3DDEVICE9 pDevice);	//	�e�I�u�W�F�N�g�̕`��֐����Ăяo��

void InitDrawState	(LPDIRECT3DDEVICE9 pDevice);	//	�`��p�̋��ʒl��ݒ�
void InitVertex2D	(VERTEX_2D* pVtx);				//	2D�`��p���_���̋��ʒl��ݒ�
void Create_recPolygon(VERTEX_2D* pVtx0,RECT rec,D3DXCOLOR col);	//	4�p�`2D�|���S���𐶐�

void IncreaseScore	(int Score);	//	�X�R�A���_
void IncreaseCount	(int Count);	//	�J�E���g����
void Reset_cntGoal	(int Manager);	//	�S�[���̃^�C�}�[�����Z�b�g

void Reset_selMenu(void);	//	���j���[����p�̃J�[�\�������Z�b�g

float Move_10to1		(D3DXVECTOR3*pos,D3DXVECTOR3 dest);	//	������1/10�l�߂�

int Get_numPhase	(void);	//	�Q�[���̏�Ԃ��擾
int Get_numState	(void);	//	�G���A�����̏�Ԃ��擾
int Get_numInArea	(void);	//	���݂̃G���A�ԍ����擾
int Get_numArea0	(void);	//	�J�n�G���A�̔ԍ����擾
bool Get_flgSurvival(void);	//	�Q�[�����[�h(�T�o�C�o��:true)��Ԃ�
bool Get_flgReady	(void);	//	�J�n�O�����m�F�p�ϐ��̎擾

DWORD Get_timAll(void);		//	�J�n�G���A����̍��v�G���A�����Ԃ��擾
DWORD Get_timOfArea(void);	//	���݂̃G���A�����Ԃ��擾
DWORD Get_timOfGoal(int);	//	�S�[���������Ԃ��擾
int Get_numScore(void);		//	�G���A���X�R�A���擾
int Get_numScoreA(void);	//	�J�n�G���A����̍��v�X�R�A���擾

int Get_selMenu(void);		//	�L�[�{�[�h/�W���C�X�e�B�b�N�ɂ�郁�j���[������s���J�[�\���̎擾

bool Get_Floating(D3DXVECTOR3, D3DXVECTOR3);	//	��ڐG����i��ڐG��true��Ԃ��j

void Retry(void);	//	�G���A���̎��Ԃ����Z�b�g

#endif