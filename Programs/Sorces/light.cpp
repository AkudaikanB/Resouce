//	����

//	�t�@�C������
#include "../Heads/light.h"
#include "../Heads/player.h"
#include "../Heads/input.h"

//	�O���[�o���ϐ��錾
D3DLIGHT9	Light_Directional00 = { D3DLIGHT_DIRECTIONAL };	//	�S�̓I�Ȍ���
D3DLIGHT9	Light_Directional01 = { D3DLIGHT_DIRECTIONAL };	//	����B
bool		flgLight01 = false;

D3DLIGHT9	Light_Player = { D3DLIGHT_POINT };	//	���Ȕ���


//	�֐���`

void InitLight(LPDIRECT3DDEVICE9 pDevice) {
	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);		//	���̐ݒ��L���ɂ���
	pDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);	//	���ʔ��˂�L���ɂ���

	//	�S�̓I�Ȍ����i���z�I�ȁj�̐ݒ�
	Light_Directional00.Diffuse		= D3DXCOLOR(0.8f,1.0f,1.0f,1.0f);	//	���ːF���ő�ɐݒ�
	Light_Directional00.Specular	= D3DXCOLOR(0.8f,1.0f,1.0f,1.0f);	//	���ʔ��˗��𖳂��ɐݒ�
	Light_Directional00.Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);	//	�������Œ�ɐݒ�
	Light_Directional01.Diffuse		= D3DXCOLOR(1.0f,0.8f,1.0f,1.0f);	//	���ːF���ő�ɐݒ�
	Light_Directional01.Specular	= D3DXCOLOR(1.0f,0.8f,1.0f,1.0f);	//	���ʔ��˗��𖳂��ɐݒ�
	Light_Directional01.Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);	//	�������Œ�ɐݒ�

	D3DXVECTOR3 vecLight01 = INIT_vecLightDirectional*-1.0f;
	
	//	���Ȕ����̌�
	Light_Player.Diffuse	=	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f);
	Light_Player.Specular	=	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f);
	Light_Player.Ambient	=	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f);
	Light_Player.Position	=	INIT_Vector3D;
	Light_Player.Range		=	300.0f;

	//	���̊p�x��P�ʉ�
	D3DXVec3Normalize((D3DXVECTOR3*)&Light_Directional00.Direction,&INIT_vecLightDirectional);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light_Directional01.Direction, &vecLight01);

	pDevice->SetLight(0,&Light_Directional00);	//	���̐ݒ�l��0�Ԃɐݒ�
	pDevice->SetLight(1,&Light_Directional01);	//	���̐ݒ�l��1�Ԃɐݒ�
	pDevice->LightEnable(0, TRUE);				//	0�Ԃ̌���L����

	pDevice->SetLight(2,&Light_Player);		//	�v���C���[�����̐ݒ�l��0�Ԃɐݒ�
	pDevice->LightEnable(2, TRUE);			//	0�Ԃ̌���L����
}

void UpdateLight(LPDIRECT3DDEVICE9 pDevice) {
	//	�v���C���[�̎��Ȕ������v���C���[�ɒǏ]������
	Light_Player.Position = Get_posEye();

	pDevice->SetLight(2, &Light_Player);		//	�v���C���[�����̐ݒ�l���X�V
}

//	���C�g�؂�ւ��ilight = 0,1�j
void ChangeLight(LPDIRECT3DDEVICE9 pDevice) {
	flgLight01 = !flgLight01;

	if (flgLight01) {
		pDevice->LightEnable(1, TRUE);
	}
	else {
		pDevice->LightEnable(1, FALSE);
	}
}