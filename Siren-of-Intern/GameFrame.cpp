#include "RPG.h"

// �R���X�g���N�^
CRPG::CRPG()
{
	Initialize();
	Create();
}

// �f�X�g���N�^
CRPG::~CRPG()
{
	Release();
}

// �֐����FInitialize			������
// ���@���F
// �߂�l�F
void CRPG::Initialize()
{
	m_GameScene				= TITLE;
	m_EndFrag				= false;
	// �}�b�v
	m_NowMapNumber			= 0;
	m_pMapFileName			= nullptr;
	m_pppMapData			= nullptr;
	m_TerrainSymbol[NONE]	= " ";
	m_TerrainSymbol[WALL]	= "*";
	m_TerrainSymbol[GROUND]	= "-";
	m_TerrainSymbol[STAIR]	= "S";
	// ���@
	m_pPlayer				= nullptr;
	// ���O�̃T�C�Y(���c��35�A�w���v�\��1�s�A4���ő�)
	m_WindowLogSize			= 3;
	// ���g���C�t���O
	m_Retry					= false;
	// �Q�[���w�i�F
	m_GameBGColor			= L_BLACK;
	m_GameTXTColor			= L_YELLOW;
	m_LogTXTColor			= H_GREEN;
	// �w���v�\��(��)
	m_Help[0]				= "<ACTION> MOVE:Q,W,E,A,D,Z,X,C WAIT:S ->QUICKLY:+LSHIFT  PICKUPITEM:RETURN <LOGSCROLL> UP,DOWN";
	m_Help[1]				= "<MENU> OPEN/CLOSE:SPACE USE/EQUIP:RETURN [SYMBOL] P:PLAYER S:STAIRtoNEXTFLOOR I:ITEM OTHER:ENEMY";
	// �A�C�e��
	m_ItemBGColor			= L_YELLOW;
	m_ItemTXTColor			= H_GREEN;
	m_ItemSymbol			= "I";
	m_pItemList				= nullptr;

	// �G�f�[�^
	m_NumEnemyType			= 0;
	m_pEnemyBookList		= nullptr;


}

// �֐����FCreate				����
// ���@���F
// �߂�l�F
void CRPG::Create()
{
	// ���@�N���X
	m_pPlayer = new CPlayer(25, 100, 15, 3, 1, 1, 30); // �ő�HP�A�����x�A�����o���l�ő�l�A�U�A�h�A�s�A�J�E���g
	// ���@�̏�����
	m_pPlayer->Initialize();
	// �R���\�[���}�l�[�W���N���X
	m_pCCManager = new CConsoleManager();
	// ��ʂ̃N���A
	m_pCCManager->FillAll(L_PURPLE, H_WHITE, m_pCCManager->GetCSBInfo());
}

// �֐����FCreateGameData		�Q�[���f�[�^���\��
// ���@���F
// �߂�l�F
bool CRPG::CreateGameData()
{
	// �J�[�\�����\��
	m_pCCManager->ExchangeCursorVisible(FALSE);

	int x = 0, y = 0;
	m_pCCManager->SetCursorPos(x, y);

	// ==== �}�b�v�f�[�^���̓ǂݍ��� ====
	if (false == CLoadData::GetLoadingInfo("Floor\\FloorStructure.txt", &m_NumMap, &m_pMapFileName, &y,&m_pCCManager, L_PURPLE, H_WHITE))
	{
		m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "�t���A�\���t�@�C���̓W�J�Ɏ��s�B�f�[�^�̍\�����������Ă��������BENTER�L�[�ŏI�����܂��B", WAIT, DONOTDELAY);
		return false;
	}
	m_pCCManager->SetCursorPos(x,++y);

	SMALL_RECT rect = m_pCCManager->GetScreenSize();

	// �}�b�v�T�C�Y�̌���
	m_Width = rect.Right;
	m_Height = rect.Bottom - m_WindowLogSize - 2 - 1; // �E�B���h�E����35 - ���O3�s - �w���v2�s - �}�b�v���K�w�\��1�s
	// �K�w���̕\���ʒu
	m_MapNamePosition = MAKECOORD(0, m_Height);
	// ���O�\���ʒu
	m_LogPosition = MAKECOORD(0, m_Height + 1);

	// �}�b�v�f�[�^�i�[�z��̐���
	if (m_NumMap == 1)
	{
		m_pppMapData = new char**();
	}
	else
	{
		m_pppMapData = new char**[m_NumMap];
	}
	for (int i = 0; i < m_NumMap; i++)
	{
		m_pppMapData[i] = new char*[m_Height];
		for (int j = 0; j < m_Height; j++)
		{
			m_pppMapData[i][j] = new char[m_Width];
		}
	}

	// ==== �e�}�b�v�f�[�^�̓ǂݍ��� ====
	for (int i= 0; i < m_NumMap; i++)
	{
		if ( false == CLoadData::LoadMapData(m_pMapFileName[i], &m_pppMapData[i], m_Width, m_Height, &y, &m_pCCManager, L_PURPLE, H_WHITE))
		{
			m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "�}�b�v�f�[�^�̓ǂݍ��݂Ɏ��s�B�t�@�C���f�[�^�̍\�����������Ă��������BENTER�L�[�ŏI�����܂��B", WAIT, DONOTDELAY);
			return false;
		}
		m_pCCManager->SetCursorPos(x,y);

		// �g���K�[���o
		m_pCCManager->Trigger();

		if ( ! m_pCCManager->Press(KEY::RETURNKEY))
		{
			Sleep(200);
		}
	}

	std::string str = "�}�b�v�f�[�^�̓ǂݍ��݊����B";
	m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), str, NOWAIT, DONOTDELAY);

	// �g���K�[���o
	m_pCCManager->Trigger();
	if ( ! m_pCCManager->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// ==== �G�f�[�^�̓ǂݍ��� ====
	if (false == CLoadData::LoadEnemyData("Data\\Enemy.txt", &m_NumEnemyType, &m_pEnemyBookList, &(++y), &m_pCCManager, L_PURPLE, H_WHITE))
	{
		m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "�G�f�[�^�t�@�C���̓W�J�Ɏ��s�B�f�[�^�̍\�����������Ă��������BENTER�L�[�ŏI�����܂��B", WAIT, DONOTDELAY);
		return false;
	}
	// �g���K�[���o
	m_pCCManager->Trigger();
	if ( ! m_pCCManager->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// ==== �A�C�e���f�[�^�̓ǂݍ��� ====
	if (false == CLoadData::LoadItemData("Data\\Item.txt", &m_NumAllItem, &m_pItemList, &y, &m_pCCManager, L_PURPLE, H_WHITE))
	{
		m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "�A�C�e���f�[�^�t�@�C���̓W�J�Ɏ��s�B�f�[�^�̍\�����������Ă��������BENTER�L�[�ŏI�����܂��B", WAIT, DONOTDELAY);
		return false;
	}

	// ���@�ɐ}�Ӄf�[�^�̃A�h���X���Z�b�g
	m_pPlayer->SetItemBook(&m_pItemList);

	// �J�[�\����\��
	m_pCCManager->ExchangeCursorVisible(TRUE);
	m_pCCManager->SetCursorPos(x + str.size(), y);
	m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "", WAIT, DONOTDELAY);

	return true;
}

// �֐����FRelease		���\�[�X�̊J��
// ���@���F
// �߂�l�F
void CRPG::Release()
{
	// �R���\�[���N���X
	if (m_pCCManager != nullptr)
	{
		delete m_pCCManager;
		m_pCCManager = nullptr;
	}
	// �}�b�v�f�[�^
	if (m_pppMapData != nullptr)
	{
		for (int i = 0; i < m_NumMap; i++)
		{
			for (int j = 0; j < m_Height; j++)
			{
				delete [] m_pppMapData[i][j];
			}
			delete [] m_pppMapData[i];
		}
		if (m_NumMap == 1)
		{
			delete m_pppMapData;
		}
		else
		{
			delete [] m_pppMapData;
			m_pppMapData = nullptr;
		}
	}
	// ���@
	if (m_pPlayer != nullptr)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	// �A�C�e���}�Ӄf�[�^
	if (m_pItemList != nullptr)
	{
		if (m_NumAllItem == 1)
		{
			delete m_pPlayer;
			m_pItemList = nullptr;
		}
		else
		{
			delete [] m_pPlayer;
			m_pItemList = nullptr;
		}
	}

	// �G�f�[�^
	if (m_pEnemyBookList != nullptr)
	{
		if (m_NumEnemyType == 1)
		{
			delete m_pEnemyBookList;
		}
		else
		{
			delete [] m_pEnemyBookList;
		}
		m_pEnemyBookList = nullptr;
	}
}

// �֐����FGameLoop		�Q�[�����[�v����
// ���@���F
// �߂�l�F
void CRPG::GameLoop()
{
	//enum GAMESCENE : char { LOAD, SAVE, TITLE, CONTINUE, DUNGEON, CITY, DEATH, RESULT, CREDIT, };
	for (;;)
	{
		switch (m_GameScene)
		{
		case TITLE:
			Title();
			break;
		case CONTINUE:
			m_GameScene = TITLE;
			break;
		case DUNGEON:
 			Dungeon();
			break;
		case CITY:
			break;
		case DEATH:
			GameOver();
			break;
		case RESULT:
			break;
		case CREDIT:
			Sleep(100);
			CreditRoll();
			break;
		case SAVE:
			break;
		}

		if (m_EndFrag == true)
		{
			// �I��
			break;
		}
	}
}
