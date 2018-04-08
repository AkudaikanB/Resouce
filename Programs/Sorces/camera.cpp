//	�J�����ݒ�

//	�t�@�C������
#include "../Heads/camera.h"
#include "../Heads/player.h"

//	�O���[�o���ϐ��錾
camera cameraPlayer;	//	�v���C���[�̖�
D3DXVECTOR3 g_rotCamera;

//	�֐���`
void InitCamera(LPDIRECT3DDEVICE9 pDevice){

	//	�ϐ��錾
	D3DXMATRIXA16	matProj,	//	�ˉe�s��
					matView;	//	���_���W�s��

	//	�ϐ�������
	cameraPlayer = INIT_Camera;	//	�J�����̏����s����쐬


	//	�ˉe�ϊ�
	D3DXMatrixPerspectiveFovLH(	//	�ˉe�s����쐬
		&matProj,			//	�����
		INIT_AngleVision,	//	����p
		INIT_AspectRatio,	//	�A�X�y�N�g��
		INIT_NearClip,		//	�j�A�N���b�v	(�f�����Ȃ�����)
		INIT_FarClip		//	�t�@�[�N���b�v	(�f�����Ȃ��Ȃ鋗��)
	);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);	//	�ˉe�ϊ�

	//	�r���[�ϊ�
	D3DXMatrixLookAtLH(		//	�r���[�s����쐬
		&matView,				//	�����	
		&cameraPlayer.posEye,	//	���_�ʒu
		&cameraPlayer.posView,	//	�����_�ʒu
		&cameraPlayer.vecUp		//	���_�����
	);
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	//	���_�̉�]�x��������
	ZeroMemory(&g_rotCamera,sizeof(D3DXVECTOR3));
}

void UpdateCamera(LPDIRECT3DDEVICE9 pDevice) {

	cameraPlayer.posEye =	Get_posEye();		//	���_�̈ʒu
	cameraPlayer.posView =	Get_vecPlayer();	//	�����_�̈ʒu

	D3DXMATRIXA16 matView;
	D3DXMatrixIdentity(&matView);

	D3DXMatrixLookAtLH(		//	�r���[�s����쐬
		&matView,				//	�����	
		&cameraPlayer.posEye,	//	���_�ʒu
		&cameraPlayer.posView,	//	�����_�ʒu
		&cameraPlayer.vecUp		//	���_�����
	);
	pDevice->SetTransform(D3DTS_VIEW, &matView);
}

//	�J�����̏����擾
camera Get_Camera(void) {
	return cameraPlayer;
}