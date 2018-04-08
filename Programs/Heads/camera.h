//	�J�����ݒ�
#ifndef	CAMERA_H	//	���d��`�h�~
#define CAMERA_H

//	�t�@�C������
#include "main.h"

//	�\���̒�`
struct camera {			//	�J����
	D3DXVECTOR3	posEye;		//	���_�ʒu
	D3DXVECTOR3 posView;	//	�����_�ʒu
	D3DXVECTOR3	vecUp;		//	���_�����
};

//	�}�N��,�萔��`
const camera	INIT_Camera{	//	�J�����̏����ݒ�
		D3DXVECTOR3(0.0f,1.8f,0.0f),
		D3DXVECTOR3(0.0f,1.8f,1.0f),
		D3DXVECTOR3(0.0f,1.0f,0.0f)
	};
const float	INIT_AngleVision	= D3DXToRadian(80);
const float	INIT_AspectRatio	= (float)SIZE_ScreenWidth / SIZE_ScreenHeight;	//	�����A�X�y�N�g��
const float INIT_NearClip		= 1.0f;											//	�����j�A�N���b�v
const float INIT_FarClip		= 5000.0f;										//	�����t�@�[�N���b�v

void InitCamera		(LPDIRECT3DDEVICE9 pDevice);	//	������
void UninitCamera	(void);							//	�������J��
void UpdateCamera	(LPDIRECT3DDEVICE9 pDevice);	//	�X�V

camera Get_Camera(void);	//	�J�����̏����擾����֐�

#endif