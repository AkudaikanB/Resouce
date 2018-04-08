//	�v���C���u���L�����N�^�[�̐���

#ifndef	PLAYER_H	//	���d��`�h�~
#define	PLAYER_H

//	�t�@�C������
#include "game.h"
#include "item.h"

//	�\���́E�N���X��`
class Character :public Object {	//	�v���C���u���L�����N�^�[�̏��
public:
	int		mortion;	//	�s���i�̐��j
	D3DXVECTOR3 vec;	//	�����_�ւ̃x�N�g���i�P�ʃx�N�g���j
	float	posEye;		//	�ڂ̈ʒu�i�����j
	float	higStand;	//	�������g��
	float	higLay;		//	����������

	D3DXVECTOR3 vecFlow;	//	�ړ��x�N�g��
	float		spdJump;	//	�W�����v��
	float		spdFall;	//	�������x
	bool		flgRun;		//	����/�삯���X�C�b�`
	float		spdStep;	//	�ڒn�ړ��̑��x

	bool		Item[NUM_MaxItem];	//	�����A�C�e����
};

//	�}�N���E�萔�E�񋓌^��`
const	D3DXVECTOR3	INIT_posPlayer	= D3DXVECTOR3(0.0f, 200.0f, 0.0f);
const	D3DXVECTOR3	INIT_vecPlayer	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
const	D3DXVECTOR3	INIT_sizPlayer	= D3DXVECTOR3(30.0f, 90.0f, 30.0f);
const	float		INIT_talEye		= 80.0f;
const	D3DXVECTOR2	INIT_rotPlayer	= D3DXVECTOR2(0.0f, 0.0f);
const	D3DXVECTOR3 INIT_vecFlow	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

enum{	//	�s���i�̐��j
	morStand = 0,	//	����
	morLay,			//	�����i�قӂ��j
	morClimb,		//	�悶�o��
	morJump,		//	��ђ���
	morStagger,		//	�ӂ���i�u���[�L���Ȃǁj
	morSlide		//	���荞��
};

//	�v���g�^�C�v�錾
void InitPlayer(LPDIRECT3DDEVICE9 pDevice);	//	������
void UninitPlayer(void);					//	�ŏ�����
void UpdatePlayer(void);					//	�X�V

void SetPlayer(D3DXVECTOR3 pos,	//	�v���C���u���L�����N�^�[�̃G���A�J�n���ݒ�
	float rot,int mortion
);

bool Get_flgItem(int nManager);	//	�A�C�e���̏�������
int Get_numItem(void);			//	�A�C�e���̏��������擾
bool Get_flgFallout(void);		//	�����I��������擾
D3DXVECTOR3 Get_posEye(void);
D3DXVECTOR3 Get_vecPlayer(void);
D3DXVECTOR2 Get_rotPlayer(void);
D3DXVECTOR3 Get_sizPlayer(void);

#endif