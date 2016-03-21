#ifndef __TREASUREHUNT_H__
#define __TREASUREHUNT_H__
#pragma once

#include <Windows.h>
#include <tuple>
#include <map>
#include <iterator>
#include "GlobalInfo.h"
#include "LoadData.h"
#include "ConsoleManager.h"
#include "Player.h"
#include "Enemy.h"

// ---- �N���X�錾 ----

class CRPG
{
private:
	CConsoleManager*		m_pCCManager;		// �R���\�[���o�͐���N���X
	char					m_GameScene;		// �Q�[���V�[��

	// �Q�[���p�t���O
	bool					m_Retry;			// �Ē���t���O
	bool					m_EndFrag;			// �I���t���O

	// �}�b�v�f�[�^
	int						m_NumMap;			// �}�b�v��
	int						m_NowMapNumber;		// ���݂̃}�b�v�ԍ�
	std::string*			m_pMapFileName;		// �}�b�v�f�[�^�̃t�@�C�����i�[�p�|�C���^
	char***					m_pppMapData;		// �}�b�v�f�[�^�i�[�p�|�C���^
	int						m_Width;			// �}�b�v�����̗v�f��
	int						m_Height;			// �}�b�v�c���̗v�f��
	std::string				m_TerrainSymbol[NUMTERRAIN]; // �n�`�̎��
	

	// �\�����O
	COORD					m_MapNamePosition;	// �}�b�v���̕\���ʒu
	int						m_WindowLogSize;	// �Q�[�����O�̏c��
	COORD					m_LogPosition;		// ���O�̕\���ʒu
	COLOR					m_GameBGColor;		// �w�i�F
	COLOR					m_GameTXTColor;		// �����F
	COLOR					m_LogTXTColor;		// ���O�����F
	std::string				m_Help[2];			// �w���v�\��

	// ���@
	CPlayer*				m_pPlayer;			// �v���C���N���X

	// �G
	int						m_NumEnemyType;		// �G�̎�ސ�
	ENEMYLIST*				m_pEnemyBookList;	// �G�}�Ӄf�[�^
	int						m_NumStartMaxEnemy; // �J�n���̓G�̍ő吔
	CEnemy					m_Sample;			// �}�ӎQ�Ɨp�G�N���X
	std::vector<CEnemy>		m_Enemy;			// ���ݏo�����Ă���G

	// �A�C�e��
	COLOR					m_ItemBGColor;		// �A�C�e���̔w�i�F
	COLOR					m_ItemTXTColor;		// �A�C�e���̕����F
	std::string				m_ItemSymbol;		// �A�C�e���̕\���L��
	int						m_numItem;			// �t�B�[���h��̃A�C�e���̐�
	int						m_NumAllItem;		// �A�C�e������ސ�
	ITEM*					m_pItemList;		// �A�C�e���}��
	FALLITEMLIST			m_ItemBookSample;	// �A�C�e���W�{
	std::vector<FALLITEMLIST> m_FallItemList;	// �����Ă���A�C�e��

	// �N���X������
/*	typedef struct _tagEnemyManager_
	{
		// �o������o����ށA�ő吔�Ȃǂ����߂���}�b�v�f�[�^����擾����
		int						m_NumEnemy;			// �G�̐�
		int*					m_NumEnemyType;		// �o������G�̎�ސ�
		std::vector<CEnemy>		m_Enemy;			// �G�N���X
	}EMANAGER;
	*/

public:
	CRPG();
	~CRPG();

	bool CreateGameData();		// �f�[�^�̍쐬
	void Initialize();			// ������
	void Create();				// ����
	void Release();				// ���\�[�X�̊J��
	void GameLoop();			// �Q�[�����[�v
	void Title();				// �^�C�g���V�[��
	void Dungeon();				// �_���W�����V�[��
	void GameOver();			// �Q�[���I�[�o�[�V�[��
	void CreditRoll();			// �N���W�b�g�V�[��

	// �_���W�����V�[����������
	std::multimap<char, char> ViewMapAndGetGround(); // �}�b�v�f�[�^����n�ʂ𒊏o����
	void SetEnemyData(int);		// �C���f�b�N�X�ԍ��ŌĂяo�����G�f�[�^���Q��
	void ResetEnemy();			// �o�^�����G�̏���
	void SetItemData(int);		// �C���f�b�N�X�ԍ��ŌĂяo�����A�C�e���f�[�^���Q��
	void ResetItem();			// �o�^�����G�̏���
	bool Menu(bool*, bool*, int*);	// ���j���[�\��
	bool LogScroll(int*, int*);	// ���O�X�N���[��
	void PlayerActionInput(COORD, COORD*, bool*, int*);	// ���@�s���p���͊m�F
	void ItemPlace(std::multimap<char, char>);		// �A�C�e���̍Ĕz�u
	void PlayerPlace(std::multimap<char, char>*);	// ���@�̍Ĕz�u
	void EnemyPlace(std::multimap<char, char>*);	// �G�̍Ĕz�u
	void RefreshEnemyIndex();	// �G�C���f�b�N�X�̍X�V
	void ConfirmPlayerMove(bool, int*, int*, COORD);					// �ړ��\�����m�F
	std::vector<FALLITEMLIST>::iterator ConfirmPlayerPickUp(bool, int*);// �A�C�e���擾�\�����m�F
	void EnemyDistanceSort();	// �G�̋������\�[�g
	void EnemyDeadCheck();		// �G�̎��S�m�F (�ƃ��O�̕\��)
	// ���@�̍s��
	void PlayerAction(int*, int, std::vector<FALLITEMLIST>::iterator*, COORD, COORD);
	void EnemyAction();			// �������Ă���G�̍s��
	void ViewItem();			// �A�C�e���̕\��
	int ConfirmDungeonState();	// �K�w�N���A�A���S�A�S�[�����̃��O�\���ƑJ��
};
#endif