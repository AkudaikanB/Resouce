//	����	 //
#ifndef	LIGHT_H
#define LIGHT_H

//	�t�@�C������
#include "main.h"

//	�\���̒�`

//	�}�N��/�萔��`
const D3DXVECTOR3 INIT_vecLightDirectional(2.0f,-1.0f,3.0f);	//	�S�̓I�Ȍ��̏�������

//	�v���g�^�C�v�錾
void InitLight		(LPDIRECT3DDEVICE9 pDevice);	//	�N�����ݒ�
void UpdateLight	(LPDIRECT3DDEVICE9 pDevice);	//	�X�V

void ChangeLight(LPDIRECT3DDEVICE9 pDevice);	//	���C�g��؂�ւ���

#endif