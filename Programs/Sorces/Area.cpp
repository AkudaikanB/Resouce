//	�G���A����


//	�t�@�C������
#include "../Heads/player.h"	//	�v���C���u���L�����N�^�[
#include "../Heads/sound.h"
#include "../Heads/stage.h"		//	�u���b�N
#include "../Heads/item.h"		//	�A�C�e���I�u�W�F�N�g
#include "../Heads/goal.h"		//	�ړI�n

//
//	�֐���`
//
//	�G���A������
void InitArea(unsigned int Index) {
	//	�ϐ��錾
	int Manager=0;	//	�J��Ԃ�������p�ϐ�
	int nGoal;	//	���������ړI�n�̊Ǘ��ԍ�
	int nStage;	//	��������h�A�̊Ǘ��ԍ�

	//	�����~�߂�
	StopSound();

	//	�S�I�u�W�F�N�g�̏���
	for (Manager = 0; Manager < NUM_MaxBlock; Manager++)EraseBlock(Manager);
	for (Manager = 0; Manager < NUM_MaxItem; Manager++)	EraseItem(Manager);
	for (Manager = 0; Manager < NUM_MaxGoal; Manager++)	EraseGoal(Manager);

	//	�G���A�����Ԃ����Z�b�g
	Retry();

	//	�G���A����
	switch (Index) {	//	�G���A�ԍ�
	case 0:	//	�`���[�g���A���X�e�[�W
		//	�u���b�N
		CreateBlock(D3DXVECTOR3(0.0f,	0.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 450.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	7.5f,	150.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 300.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	22.5f,	300.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	350.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(200.0f, 50.0f, 450.0f));	//	�V��
		CreateBlock(D3DXVECTOR3(0.0f,	292.5f,	-150.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 300.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	277.5f,	-300.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(150.0f,	200.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 300.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(-150.0f,200.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 300.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	200.0f,	450.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 300.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(0.0f,	200.0f,-450.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 300.0f, 0.5f));
	
		//	�ړI�n�ƃA�C�e���̐����i�{�[�i�X�J�E���g���������̓o�^�j
		nGoal =	CreateGoal(D3DXVECTOR3(0.0f, 150.0f, -400.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 150.0f, 300.0f), 10.0f)
		);	//	���������A�C�e���̔ԍ���nGoal�{�[�i�X�J�E���g���������ɓo�^����

		//	BGM�𗬂�
		PlaySound(MUSIC_Area00);

		//	�v���C���[�̈ʒu
		SetPlayer(D3DXVECTOR3(0.0f,150.0f,0.0f),D3DXToRadian(180),morStand);
		break;
	case 1:	
		//	����
		CreateBlock(D3DXVECTOR3(0.0f,	0.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(150.0f, 7.5f, 450.0f));
		CreateBlock(D3DXVECTOR3(150.0f,	300.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 400.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(-150.0f,300.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 400.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	300.0f,	450.0f),D3DXToRadian(0), D3DXVECTOR3(150.0f, 400.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(0.0f,	300.0f,-450.0f),D3DXToRadian(0), D3DXVECTOR3(150.0f, 400.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(0.0f,	450.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(150.0f, 50.0f, 450.0f));	//	�V��
		CreateBlock(D3DXVECTOR3(0.0f,	150.0f, 300.0f),D3DXToRadian(0), D3DXVECTOR3(150.0f, 50.0f, 150.0f));

		//	�ړI�n�ƃA�C�e���̐����i�{�[�i�X�J�E���g���������̓o�^�j
		nGoal = CreateGoal(D3DXVECTOR3(0.0f, 150.0f, -400.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 20.0f, 300.0f), 10.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 320.0f, 300.0f), 10.0f));

		//	BGM�𗬂�
		PlaySound(MUSIC_Area01);

		//	�v���C���[�̈ʒu
		SetPlayer(D3DXVECTOR3(0.0f,150.0f,0.0f),0.0f,morStand);
		break;
	case 2:
			//	�u���b�N 
		CreateBlock(D3DXVECTOR3(0.0f, -100.0f, -100.0f),	D3DXToRadian(0), D3DXVECTOR3(350.0f, 100.0f, 400.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 200.0f, -400.0f),		D3DXToRadian(0), D3DXVECTOR3(100.0f, 200.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(325.0f, 200.0f, -400.0f),	D3DXToRadian(0), D3DXVECTOR3(25.0f, 200.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(-325.0f, 200.0f, -400.0f),	D3DXToRadian(0), D3DXVECTOR3(25.0f, 200.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 500.0f, -400.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 100.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 800.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 50.0f, 500.0f));	//	�V��

		CreateBlock(D3DXVECTOR3(-300.0f, 12.5f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 12.5f, 200.0f));
		CreateBlock(D3DXVECTOR3(-300.0f, 25.0f, 350.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 150.0f));
		CreateBlock(D3DXVECTOR3(-300.0f, 50.0f, 400.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(-300.0f, 75.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(-200.0f, 100.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(-100.0f, 125.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 150.0f, 450.0f),		D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(150.0f, 175.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 25.0f, 50.0f));

		CreateBlock(D3DXVECTOR3(0.0f, 347.5f, 102.5f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 2.5f, 2.5f));
		CreateBlock(D3DXVECTOR3(0.0f, 250.0f, 102.5f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 50.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 347.5f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 2.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(247.5f, 250.0f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 50.0f, 150.0f));
		CreateBlock(D3DXVECTOR3(-247.5f, 347.5f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 2.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(-247.5f, 250.0f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 50.0f, 150.0f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 397.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 200.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(150.0f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(50.0f, 325.0f, 102.5f),		D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-50.0f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-150.0f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 200.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 397.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));

		CreateBlock(D3DXVECTOR3(0.0f,-150.0f, 350.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f,-50.0f, 350.0f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(300.0f,-200.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f,-250.0f, 450.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 100.0f, 250.0f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 100.0f, 150.0f));

		CreateBlock(D3DXVECTOR3(0.0f,	300.0f, -500.0f),	D3DXToRadian(0), D3DXVECTOR3(350.0f, 500.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(350.0f, 300.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(0.5f, 500.0f, 500.0f));
		CreateBlock(D3DXVECTOR3(-350.0f,300.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(0.5f, 500.0f, 500.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	300.0f, 500.0f),	D3DXToRadian(0), D3DXVECTOR3(350.0f, 500.0f, 0.5f));

		//	�ړI�n�ƃA�C�e���̐����i�{�[�i�X�J�E���g���������̓o�^�j
		nGoal = 
			CreateGoal(D3DXVECTOR3(200.0f, 150.0f, -450.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 350.0f, 150.0f), 10.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, -100.0f, 450.0f), 10.0f));
		nGoal = 
			CreateGoal(D3DXVECTOR3(-200.0f, 150.0f, -450.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(300.0f, 100.0f, 450.0f), 10.0f));

			//	BGM�𗬂�
		PlaySound(MUSIC_Area02);

		//	�v���C���[�̈ʒu
		SetPlayer(D3DXVECTOR3(0.0f,150.0f,-250.0f),0.0f,morStand);
		break;
		
	case 3:
		//	����̐ݒu
		CreateBlock(D3DXVECTOR3(0,250.0f,500.0f),	0,	D3DXVECTOR3(650.0f,250.0f,500.0f));
		CreateBlock(D3DXVECTOR3(0,900.0f,250.0f),		0,	D3DXVECTOR3(450.0f,1.0f,250.0f));
		CreateBlock(D3DXVECTOR3(450.0f,	700.0f,250.0f),	0,D3DXVECTOR3(1.0f,200.0f,250.0f));
		CreateBlock(D3DXVECTOR3(-450.0f,700.0f,250.0f),	0,D3DXVECTOR3(1.0f,200.0f,250.0f));
		CreateBlock(D3DXVECTOR3(250.0f,	700.0f,150.0f),	0,D3DXVECTOR3(1.0f,200.0f,150.0f));
		CreateBlock(D3DXVECTOR3(-250.0f,700.0f,150.0f),	0,D3DXVECTOR3(1.0f,200.0f,150.0f));
		CreateBlock(D3DXVECTOR3(0,700.0f,0.0f),		0,	D3DXVECTOR3(450.0f,200.0f,1.0f));
		CreateBlock(D3DXVECTOR3(275,700.0f,500.0f),	0,	D3DXVECTOR3(175.0f,200.0f,1.0f));
		CreateBlock(D3DXVECTOR3(-275,700.0f,500.0f),0,	D3DXVECTOR3(175.0f,200.0f,1.0f));
		CreateBlock(D3DXVECTOR3(150,590.0f,300.0f),	0,	D3DXVECTOR3(100.0f,90.0f,1.0f));
		CreateBlock(D3DXVECTOR3(-150,590.0f,300.0f),0,	D3DXVECTOR3(100.0f,90.0f,1.0f));
		CreateBlock(D3DXVECTOR3(75,690.0f,300.0f),	0,	D3DXVECTOR3(25.0f,10.0f,1.0f));
		CreateBlock(D3DXVECTOR3(-75,690.0f,300.0f),0,	D3DXVECTOR3(25.0f,10.0f,1.0f));
		CreateBlock(D3DXVECTOR3(0,750.0f,300.0f),	0,	D3DXVECTOR3(250.0f,50.0f,1.0f));
		CreateBlock(D3DXVECTOR3(250, 700.0f, 300.0f),	0, D3DXVECTOR3(10.0f, 200.0f, 10.0f));
		CreateBlock(D3DXVECTOR3(-250, 700.0f, 300.0f),	0, D3DXVECTOR3(10.0f, 200.0f, 10.0f));
		CreateBlock(D3DXVECTOR3(0,850.0f,500.0f),	0,	D3DXVECTOR3(100.0f,50.0f,1.0f));

		//	�K�i
		CreateBlock(D3DXVECTOR3(200, 520.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(250, 540.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(300, 560.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(350, 580.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(400, 600.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(450, 620.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 640.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 660.0f, 450.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 680.0f, 400.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 700.0f, 350.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 720.0f, 300.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 740.0f, 250.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 760.0f, 200.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 780.0f, 150.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 800.0f, 100.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 820.0f, 50.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 840.0f, 0.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 860.0f, -50.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0, 880.0f, -50.0f), 0, D3DXVECTOR3(450.0f, 10.0f, 50.0f));

		CreateBlock(D3DXVECTOR3(-250, 900.0f, 750.0f), 0, D3DXVECTOR3(50.0f, 1.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(-250, 900.0f, 1150.0f), 0, D3DXVECTOR3(50.0f, 1.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(200, 550.0f, 50.0f), 0, D3DXVECTOR3(50.0f, 1.0f, 50.0f));

		CreateBlock(D3DXVECTOR3(-150, 350.0f, 1500.0f), 0, D3DXVECTOR3(250.0f, 350.0f, 150.0f));

		//	�S�[��,�A�C�e��,�h�A�̐ݒu
		nGoal =						//	�S�[��
			CreateGoal(D3DXVECTOR3(0.0f, -1000.0f, 500.0f), 0.0f, D3DXVECTOR3(10000.0f, 1.0f, 10000.0f));
		Set_numItemToGoal(nGoal,	//	�A�C�e��
			CreateItem(D3DXVECTOR3(0, 750, 1500), 10));
		Set_numItemToGoal(nGoal,	//	�A�C�e��
			CreateItem(D3DXVECTOR3(400, 910, 400), 10));

		nGoal =						//	�S�[��
			CreateGoal(D3DXVECTOR3(350.0f, 650.0f, 50.0f), 0.0f, D3DXVECTOR3(100.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,	//	�A�C�e��
			CreateItem(D3DXVECTOR3(-350,550,50),50));
		Set_numEraseByGoal(			//	�h�A
			CreateBlock(D3DXVECTOR3(0, 600.0f, 300.0f), 0, D3DXVECTOR3(50.0f, 100.0f, 1.0f)),nGoal);

		nGoal =						//	�S�[��
			CreateGoal(D3DXVECTOR3(-200.0f, 650.0f, 50.0f), 0.0f, D3DXVECTOR3(50.0f, 150.0f, 100.0f));
		Set_numItemToGoal(nGoal,	//	�A�C�e��
			CreateItem(D3DXVECTOR3(200,530,20),10));
		Set_numEraseByGoal(			//	�h�A
			CreateBlock(D3DXVECTOR3(0, 650.0f, 500.0f), 0, D3DXVECTOR3(100.0f, 150.0f, 1.0f)),nGoal);

		CreateItem(D3DXVECTOR3(500, 600, 50), 10);
		CreateItem(D3DXVECTOR3(-500, 600, 50), 10);

		//	�v���C���[�̐ݒu
		SetPlayer(D3DXVECTOR3(350.0f, 500.0f+Get_sizPlayer().y, 400.0f), D3DXToRadian(-90), morStand);

		//	���y�̍Đ�
		PlaySound(MUSIC_Area03);
		break;

	case 4:
		CreateBlock(D3DXVECTOR3(0, 0, 0), 0, D3DXVECTOR3(700.0f, 1.0f, 700.0f));
		//	�S�[���̐ݒu
		CreateGoal(D3DXVECTOR3(-1.0f, 2000.0f, 1000.0f), 0.0f, D3DXVECTOR3(0.5f, 1.0f, 1.0f));

		//	�v���C���[�̐ݒu
		SetPlayer(D3DXVECTOR3(100.0f, 200.0f, 400.0f), 0.0f, morStand);

		//	���y�̍Đ�
		PlaySound(MUSIC_Area04);
		break;

	case 25:	//	�T�o�C�o���G���A
		//	�u���b�N
		CreateBlock(D3DXVECTOR3(0.0f, 0.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(5000.0f, 1.0f, 5000.0f));
		CreateBlock(D3DXVECTOR3(5000.0f, 500.0f, 0.0f), D3DXToRadian(0), D3DXVECTOR3(1.0f, 1000.0f, 5000.0f));
		CreateBlock(D3DXVECTOR3(-5000.0f,500.0f, 0.0f), D3DXToRadian(0), D3DXVECTOR3(1.0f, 1000.0f, 5000.0f));
		CreateBlock(D3DXVECTOR3(0.0f,500.0f, 5000.0f ), D3DXToRadian(0), D3DXVECTOR3(5000.0f, 1000.0f, 0.0f));
		CreateBlock(D3DXVECTOR3(0.0f,500.0f, -5000.0f), D3DXToRadian(0), D3DXVECTOR3(5000.0f, 1000.0f, 0.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 300.0f, 0.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 1.0f, 200.0f));
		CreateBlock(D3DXVECTOR3(-200.0f, 30.0f, 0.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 30.0f, 200.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 10.0f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(-200.0f, 30.0f, 300.0f), D3DXToRadian(0), D3DXVECTOR3(100.0f, 10.0f, 100.0f));

		//	�ړI�n�ƃA�C�e���̐����i�{�[�i�X�J�E���g���������̓o�^�j
		nGoal = CreateGoal(D3DXVECTOR3(-200.0f, 150.0f, -300.0f), D3DXToRadian(0), D3DXVECTOR3(200.0f, 200.0f, 100.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(-200.0f, 150.0f, 0.0f), 10.0f));

		//	BGM�𗬂�
		PlaySound(MUSIC_Survival);

		//	�v���C���[�̈ʒu
		SetPlayer(INIT_posPlayer, 0.0f, morStand);
		break;
	default:	//	���݂��Ȃ��G���A���
		SetPlayer(INIT_posPlayer, 0.0f, morStand);
		break;
	}
}
