//	�S�[��,�ړI�n(=Goal)�̐���

//	���d��`�h�~
#ifndef GOAL_H
#define GOAL_H


//	�t�@�C������
#include "game.h"

//	�}�N����`
#define	NUM_MaxGoal	(10)		//	�ړI�n�̍ő�Ǘ���
#define NUM_ItemToGoal (5)	//	�S�[�������A�C�e���̍ő���i�ėp�j
#define NUM_VtxGoal	(24)	//	�ړI�n�\���i�����́j�̒��_��	
#define NUM_PlgGoal	(22)	//	�ړI�n�\���i�����́j�̃|���S����

//	�v���g�^�C�v�錾
void InitGoal	(LPDIRECT3DDEVICE9 pDevice);	//	������
void UninitGoal	(void);							//	�ŏ�����
void UpdateGoals(void);							//	�X�V
void DrawGoals	(LPDIRECT3DDEVICE9 pDevice);	//	�`��

int CreateGoal(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 siz);	//	�ړI�n�𐶐����A�Ǘ��ԍ���Ԃ�
void EraseGoal		(int Manager);			//	�ړI�n�̖�����
void EraseGoalFLG	(int Manager);			//	�ړI�n�̖������i�T�o�C�o���j
void EraseItemFLG	(int Manager);			//	�A�C�e���̖������i�T�o�C�o���j
void Set_numItemToGoal(int nGoal,int nItem);	//	nGoal�Ԃ̃S�[���̃{�[�i�X�J�E���g�����ɑΉ�����A�C�e���ԍ���ݒ�
void SetGoalCount	(int Manager,bool set);	//	�S�[���������Ԃ̃J�E���^�[��؂�ւ���
void ChangeColorGoal(int nManager,int type);//	�ړI�n�̐F��ύX

int	Get_TouchedGoal(D3DXVECTOR3 pos,D3DXVECTOR3 siz);	//	�ړI�n�Ƃ̐ڐG����
int Get_StartingGoal(int nItem);	//	�A�C�e���Ǘ��ԍ��ɑΉ������S�[���̒l��Ԃ�
int Get_numItemOfGoal(int nManager);//	�S�[�������̃A�C�e�������擾
int Get_numGoal();					//	��������S�[���̐����擾
bool Get_flgGoalCount(int nManager);//	�S�[���̌���������擾
bool Get_toGoal(int nManager);		//	�S�[����������
bool Get_Clear(void);				//	�S�[���̌�������0���ǂ����𔻒�
bool Get_flgGoNext(void);			//	�G���A�i�s�ۂ̎擾


//	�Œ�ʒu(Survival)
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
//	�Œ�T�C�Y(Survival)
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