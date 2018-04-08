//	�X�e�[�W�I�u�W�F�N�g�i=Block:����/��/�V��,�ȉ��u���b�N�j�E�S�[������

#ifndef STAGE_H	//	���d��`�h�~
#define STAGE_H

//	�t�@�C������
#include "game.h"

//	�}�N��/�萔��`
#define NUM_MaxBlock	(256)	//	�u���b�N�̐�
#define NUM_VtxBlock	(24)	//	�u���b�N�̒��_���i6��x4�j
#define NUM_plgBlock	(22)	//	�u���b�N�̃|���S����(�k�ރ|���S���܂�)

//	�v���g�^�C�v�錾
void InitStage(LPDIRECT3DDEVICE9 pDevice);	//	������
void UninitStage(void);						//	�������J��
void UpdateStage(void);						//	�X�V
void DrawStage(LPDIRECT3DDEVICE9 pDevice);	//	�`��

int CreateBlock(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 siz);	//	pos�ʒu��siz.x*siz.y*siz.z�̒����̂𐶐�
void EraseBlock(int Index);
D3DXVECTOR3 Get_PosBlock(int manBlock);	//	�w�肳�ꂽ�u���b�N�̈ʒu���擾
D3DXVECTOR3 Get_SizBlock(int manBlock);	//	�w�肳�ꂽ�u���b�N�̑傫�����擾
bool Get_flgBlock(int);

void Set_numEraseByGoal(int nStage,int nGoal);	//	��������ɑΉ�����S�[����ݒ�
void EraseBlockByGoal(int nGoal);				//	nGoal�Ԗڂ̃S�[���ɑΉ�����u���b�N����

#endif