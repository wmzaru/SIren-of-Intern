#include "RPG.h"

// �֐����FGetGroundData	�}�b�v�f�[�^����n�ʃf�[�^�𒊏o
// ���@���F
// �߂�l�F
std::multimap<char, char> CRPG::ViewMapAndGetGround()
{
	COORD pos = {0, 0};
	std::multimap<char, char> ground;
	std::string symbol;
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			pos = MAKECOORD(j, i);
			m_pCCManager->SetCursorPos(pos);
			char str = m_pppMapData[m_NowMapNumber][i][j];
			//enum MAP : char { NONE, WALL, GROUND, STAIR, };
			switch (str)
			{
			case NONE:
				symbol = m_TerrainSymbol[NONE];
				break;
			case WALL:
				symbol = m_TerrainSymbol[WALL];
				break;
			case GROUND:
				symbol = m_TerrainSymbol[GROUND];
				ground.insert(std::make_pair(j, i));
				break;
			case STAIR:
				symbol = m_TerrainSymbol[STAIR];
				break;
			}
			if (symbol == m_TerrainSymbol[STAIR])
			{
				m_pCCManager->WriteTextCharacter(H_WHITE, L_BLACK, pos, symbol, NOWAIT, DONOTDELAY);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, pos, symbol, NOWAIT, DONOTDELAY);
			}
		}
	}
	return ground;
}

// �֐����FSetEnemyData		�C���f�b�N�X�ԍ��ŌĂяo�����G�f�[�^���Q��
// ���@���Findex			�C���f�b�N�X�ԍ�
// �߂�l�F
void CRPG::SetEnemyData(int index)
{
	// �}�ӂ���G�T���v���փf�[�^��o�^
	m_Sample.SetBookData(m_pEnemyBookList[index]);

	// �T���v���̏����� (�̗͂̌��ݒl�Ȃ�)
	m_Sample.Initialize();

	// �����ŏ����A�C�e��������(��)
	int seed = rand() % 20;
	bool flag = false;
	if (seed == 0)
	{
		flag = true;
	}

	int dropItemIndex;// �h���b�v�A�C�e���ԍ�
	dropItemIndex = rand() % m_NumAllItem;

	m_Sample.SetItemHaving(flag, dropItemIndex);
}

// �֐����FResetEnemy			�o�^�����G�̏���(�R���e�i�̑S�v�f�폜->������)
// ���@���F
// �߂�l�F
void CRPG::ResetEnemy()
{
	m_Enemy.clear();
}

// �֐����FSetItemData		�C���f�b�N�X�ԍ��ŌĂяo�����A�C�e���f�[�^���Q��
// ���@���Findex			�C���f�b�N�X�ԍ�
// �߂�l�F
void CRPG::SetItemData(int index)
{

}

// �֐����FResetItem			�o�^�����A�C�e���̏���(�R���e�i�̑S�v�f�폜->������)
// ���@���F
// �߂�l�F
void CRPG::ResetItem()
{
	m_FallItemList.clear();
}

// �֐����FLogScroll		���O�X�N���[��
// ���@���FpScrollCount		�X�N���[���ʂւ̃A�h���X
// �@�@�@�FpNumScroll		�ő�X�N���[���\�ʂւ̃A�h���X
// �߂�l�F
bool CRPG::LogScroll(int* pScrollCount, int* pNumScroll)
{
	int logSize = m_pCCManager->GetLogSize();
	if (m_pCCManager->GetLogSize() <= m_WindowLogSize)
	{
		return false;
	}

	// �ő�X�N���[���ʂ̍X�V
	(*pNumScroll) = logSize - m_WindowLogSize;

	// �t���O
	bool isRefresh = false;

	// ���O����ɃX�N���[��
	if (m_pCCManager->PulseOn(KEY::UPKEY))
	{
		if ((*pScrollCount) > 0)
		{
			isRefresh = true;
			(*pScrollCount)--;
		}
	}
	// ���O�����ɃX�N���[��
	if (m_pCCManager->PulseOn(KEY::DOWNKEY))
	{
		if ((*pScrollCount) < (*pNumScroll))
		{
			isRefresh = true;
			(*pScrollCount)++;
		}
	}
	if (isRefresh == true)
	{
		// �X�N���[��������񃍃O�̍ĕ\��
		m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);
		m_pCCManager->ViewLog(m_LogPosition, m_GameBGColor, m_LogTXTColor,(*pScrollCount), m_WindowLogSize);
		Sleep(10);
	}
	return isRefresh;
}

// �֐����FMenu				���j���[�\��
// ���@���FpIsOpen			���j���[���J���Ă��邩�ǂ���
// �@�@�@�FpScrollCount		�X�N���[���ʂւ̃A�h���X
// �߂�l�F
bool CRPG::Menu(bool* pIsOpen, bool* pItemUse, int* pScrollCount)
{
	const int viewHeight = 3; // �s�����O�̍���
	char buffer[256];
	std::string selectCursor = "�� [USE:ENTER / BACK:SPACE]";
	// �A�C�e���f�[�^
	ItemList& iList = m_pPlayer->GetItem();
	ItemList::iterator itemIter;

	// ��\���̎���SPACE�{�^���ŊJ��
	if (m_pCCManager->PulseOn(KEY::SPACEKEY))
	{
		// �t���O�������Ă��Ȃ����̓��j���[���[�h�ɓ���
		if ((*pIsOpen) == false)
		{
			(*pIsOpen) = true;

			// �\���̈�̃N���A
			m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

			COORD pos = m_LogPosition;
			// �X�N���[���������j���[�̍ĕ\��
			int count = 0;
			for (itemIter = iList.begin(); itemIter != iList.end(); itemIter++)
			{
				std::string name = m_pItemList[(*itemIter).first].name + ":";
				::_itoa_s((*itemIter).second, buffer, sizeof(char) * 256, 10);
				name += buffer;
				if (count == 0)
				{
					name += selectCursor;
				}
				// �A�C�e���ꗗ�̕\��
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_LogTXTColor, pos, name, NOWAIT, DONOTDELAY);
				pos.Y++;
				if (++count == viewHeight)
				{
					break;
				}
			}
			// �A�C�e���������Ȃ��Ƃ�
			if (iList.size() == 0)
			{
				std::string str = "���������Ă��Ȃ��B [BACK:SPACE]";
				// �A�C�e���ꗗ�̕\��
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_LogTXTColor, pos, str, NOWAIT, DONOTDELAY);
			}
		}
		// �t���O�������Ă���Ƃ��̓��j���[���[�h���甲����
		else
		{
			(*pIsOpen) = false;

			// �X�N���[���ʂ̃��Z�b�g
			(*pScrollCount) = 0;

			// �\���̈�̃N���A
			m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

			return false;
		}
	}


	// �t���O�������͂��̂܂ܕԋp
	if ((*pIsOpen) == false)
	{
		return false;
	}

	// �I�����Ă���A�C�e�����g�p���邽�߂̃t���O�؂�ւ�
	if (m_pCCManager->PulseOn(KEY::RETURNKEY))
	{
		// �����A�C�e����0�̂Ƃ��͖���
		if (iList.size() != 0)
		{
			// �t���O�؂�ւ�
			(*pItemUse) = true;
			(*pIsOpen) = false;
		}
	}

	if ((*pItemUse) == false)
	{
		// ���j���[�\���̍X�V�t���O
		bool isRefresh = false;
		// �A�C�e���ꗗ�̍s��
		int listSize = m_pPlayer->GetItem().size();

		// ���O����ɃX�N���[��
		if (m_pCCManager->PulseOn(KEY::UPKEY))
		{
			if ((*pScrollCount) > 0)
			{
				isRefresh = true;
				(*pScrollCount)--;
			}
		}
		// ���O�����ɃX�N���[��
		if (m_pCCManager->PulseOn(KEY::DOWNKEY))
		{
			if ((*pScrollCount) < listSize - 1)
			{
				isRefresh = true;
				(*pScrollCount)++;
			}
		}

		if (isRefresh == true)
		{
			// �\���̈�̃N���A
			m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

			COORD pos = m_LogPosition;
			int viewCount = 0;
			int passCount = 0;
			// �X�N���[���������j���[�̍ĕ\��
			itemIter = iList.begin();

			// �A�C�e���̖��O��o�^����
			std::vector<std::string> nameList;
			for (itemIter = iList.begin(); itemIter != iList.end(); itemIter++)
			{
				// �A�C�e���̖��̂��擾
				std::string name = m_pItemList[(*itemIter).first].name + ":";
				::_itoa_s((*itemIter).second, buffer, sizeof(char) * 256, 10);
				name += buffer;

				// ���ݎw�肵�Ă���A�C�e���̖��̕����ɃJ�[�\��������
				if (viewCount++ == (*pScrollCount))
				{
					name += selectCursor;
				}
				nameList.push_back(name);
			}

			// �o�^�������O��\������
			viewCount = 0;
			std::vector<std::string>::iterator nameIter;
			for (nameIter = nameList.begin(); nameIter != nameList.end(); nameIter++)
			{
				if (passCount++ < (*pScrollCount) - 2)
				{
					continue;
				}

				// �A�C�e���ꗗ�̕\��
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_LogTXTColor, pos, (*nameIter), NOWAIT, DONOTDELAY);
				pos.Y++;
				if (++viewCount == viewHeight)
				{
					break;
				}
			}
			Sleep(10);
		}
	}
	else
	{
		// �\���̈�̃N���A
		m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

		// ==== ���݂̃X�N���[���ʂŎw�肳��Ă���A�C�e���̃C���f�b�N�X�ԍ����擾���� ====
		COORD pos = m_LogPosition;
		int passCount = 0;
		for (itemIter = iList.begin(); itemIter != iList.end(); itemIter++)
		{
			if (passCount++ != (*pScrollCount))
			{
				continue;
			}

			// �g�p����A�C�e���̃C���f�b�N�X�ԍ����v���C���Ɏw�肷��
			m_pPlayer->SetUseItemIndex((*itemIter).first);
			break;
		}

		// �A�C�e���g�p�����܂��ă��j���[�����̂ŃX�N���[���ʂ����Z�b�g����
		(*pScrollCount) = 0;

		return false;
	}

	return true;
}

// �֐����FPlayerActionInput	���@�s���p���͊m�F
// ���@���FpNowPosition			���݂̍��W
// �@�@�@�FpTarget				�ړ���̍��W�ւ̃A�h���X
// �@�@�@�FpIsInput				���͂����������̃t���O�ւ̃A�h���X
// �@�@�@�FpPlayerState			���@�̏�Ԃւ̃A�h���X
// �߂�l�F
void CRPG::PlayerActionInput(COORD nowPosition, COORD* pTarget, bool* pIsInput, int* pPlayerState)
{
	// �A�C�e���̎擾
	if (m_pCCManager->PulseOn(KEY::RETURNKEY))
	{
		(*pIsInput)		= true;
		(*pPlayerState)	|= _PICKUP;
		return;
	}

	// ������
	if ((m_pCCManager->PulseOn(KEY::SKEY))
	 || (m_pCCManager->Press(KEY::SKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		(*pIsInput)		= true;
		(*pPlayerState)	|= _FOOT;
		(*pPlayerState)	|= _ACTION;
		return;
	}

	// ��ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::WKEY))
	 || (m_pCCManager->Press(KEY::WKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.Y > 0)
		{
			(*pTarget).Y--;
			(*pIsInput) = true;
			return;
		}
	}
	// ���ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::XKEY))
	 || (m_pCCManager->Press(KEY::XKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.Y < m_Height - 1)
		{
			(*pTarget).Y++;
			(*pIsInput) = true;
			return;
		}
	}
	// ���ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::AKEY))
	 || (m_pCCManager->Press(KEY::AKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X > 0)
		{
			(*pTarget).X--;
			(*pIsInput) = true;
			return;
		}
	}
	// �E�ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::DKEY))
	 || (m_pCCManager->Press(KEY::DKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X < m_Width - 1)
		{
			(*pTarget).X++;
			(*pIsInput) = true;
			return;
		}
	}
	// ����ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::QKEY))
	 || (m_pCCManager->Press(KEY::QKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X > 0 && nowPosition.Y > 0)
		{
			(*pTarget).X--;
			(*pTarget).Y--;
			(*pIsInput) = true;
			return;
		}
	}
	// �E��ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::EKEY))
	 || (m_pCCManager->Press(KEY::EKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X < m_Width - 1 && nowPosition.Y > 0)
		{
			(*pTarget).X++;
			(*pTarget).Y--;
			(*pIsInput) = true;
			return;
		}
	}
	// �����ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::ZKEY))
	 || (m_pCCManager->Press(KEY::ZKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X > 0 && nowPosition.Y < m_Height - 1)
		{
			(*pTarget).X--;
			(*pTarget).Y++;
			(*pIsInput) = true;
			return;
		}
	}
	// �E���ւ̈ړ�
	if ((m_pCCManager->PulseOn(KEY::CKEY))
	 || (m_pCCManager->Press(KEY::CKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X < m_Width - 1 && nowPosition.Y < m_Height - 1)
		{
			(*pTarget).X++;
			(*pTarget).Y++;
			(*pIsInput) = true;
			return;
		}
	}
}

// �֐����FItemPlace			�A�C�e���̔z�u
// ���@���Fground				�n�`�f�[�^
// �߂�l�F
void CRPG::ItemPlace(std::multimap<char, char> ground)
{
	// �ϐ�
	COORD startPos;
	int size = ground.size();
	int number = rand() % size;

	// �}�b�v�J�n���ɗ����Ă���A�C�e���̐�
	int numItem = rand() % (m_NowMapNumber + 3) + 2;

	// �����p�C�e���[�^
	std::multimap<char, char>::iterator gIter = ground.begin();

	for (int i = 0; i < numItem;  /**/ )
	{
		// �z�u�ꏊ�������_���w��
		size = ground.size();
		number = rand() % size;
		gIter = ground.begin();
		for (int j = 0; j < number; j++)
		{
			gIter++;
		}
		startPos.X = gIter->first;
		startPos.Y = gIter->second;

		// �A�C�e���ԍ��̃����_���w��
		int index = rand() % m_NumAllItem;

		// ������������������������������������
		// ����̏o����4�K�ȍ~�ɋ����w��
		if (m_NowMapNumber < 3)
		{
			int type = m_pItemList[index].type;
			if (type == ARMOR || type == WEAPON || type == SHIELD)
			{
				continue;
			}
		}
		// ������������������������������������


		// �A�C�e���T���v���Ƀf�[�^��ݒ�
		m_ItemBookSample.X = startPos.X;
		m_ItemBookSample.Y = startPos.Y;
		m_ItemBookSample.index = index;

		// �A�C�e����ݒ�
		m_FallItemList.push_back(m_ItemBookSample);

		// �A�C�e�����m���d�����Ă��܂��̂ŗv�f���폜
		ground.erase(gIter);

		i++;
	}

}

// �֐����FPlayerPlace			�A�C�e���̔z�u
// ���@���Fground				�n�`�f�[�^
// �߂�l�F
void CRPG::PlayerPlace(std::multimap<char, char>* pGround)
{
	// �ϐ�
	COORD startPos;
	int size = (*pGround).size();
	int number = rand() % size;

	// �n�`���X�g�̃C�e���[�^
	std::multimap<char, char>::iterator gIter = (*pGround).begin();

	for (int j = 0; j < number; j++)
	{
		gIter++;
	}
	startPos.X = gIter->first;
	startPos.Y = gIter->second;

	// �v�f���폜
	(*pGround).erase(gIter);

	// ���W�̐ݒ�
	m_pPlayer->SetPosition(startPos);
}

// �֐����FEnemyPlace			�G�̔z�u
// ���@���FpGround				�n�`�f�[�^�R���e�i�ւ̃A�h���X
// �߂�l�F
void CRPG::EnemyPlace(std::multimap<char, char>* pGround)
{
	// �ϐ�
	int cIndex = 0;
	COORD startPos;
	int size;
	int number;

	// �G���X�g�̃C�e���[�^
	std::vector<CEnemy>::iterator eIt;

	// �n�`���X�g�̃C�e���[�^
	std::multimap<char, char>::iterator gIter;

	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		// �n�`�������_���w��
		size = (*pGround).size();
		number = rand() % size;
		gIter = (*pGround).begin();
		for (int j = 0; j < number; j++)
		{
			gIter++;
		}
		startPos.X = gIter->first;
		startPos.Y = gIter->second;

		// �v�f�̍폜
		(*pGround).erase(gIter);

		// �G�ɍ��W��ݒ肷��
		(*eIt).SetPosition(startPos);
		(*eIt).SetIndex(cIndex++);
	}
}

// �֐����FRefreshEnemyIndex	�G�C���f�b�N�X�̍X�V
// ���@���F
// �߂�l�F
void CRPG::RefreshEnemyIndex()
{
	int enemyIndex = 0;
	std::vector<CEnemy>::iterator eIt;
	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		(*eIt).SetIndex(enemyIndex++);
	}
}

// �֐����FConfirmPlayerMove	�ړ��̊m�F
// ���@���FisInput				���̗͂L��
// �@�@�@�FpPlayerState			���@�̏�Ԃւ̃A�h���X
// �@�@�@�Ftarget				�ړ���̍��W
// �߂�l�F
void CRPG::ConfirmPlayerMove(bool isInput, int* pPlayerState, int* pTargetNumber, COORD target)
{
	if (isInput == false)
	{
		return;
	}

	if ( ! (   ( ! ((*pPlayerState) & _FOOT))
			&& ( ! ((*pPlayerState) & _PICKUP))
			&& ( ! ((*pPlayerState) & _USEITEM))))
	{
		return;
	}

	// �G�����邩�ǂ���
	bool enemyExist = false;
	std::vector<CEnemy>::iterator eIt;
	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		if ((*eIt).GetDeadFlag())
		{
			continue;
		}
		COORD enemy = (*eIt).GetPosition();
		if (EQUAL(target, enemy))
		{
			enemyExist = true;
			(*pTargetNumber) = (*eIt).GetIndex();
			break;
		}
	}
	if (enemyExist == true)
	{
		(*pPlayerState) |= _ACTION;
		(*pPlayerState) |= _ATTACK;
	}
	else
	{
		char after = m_pppMapData[m_NowMapNumber][target.Y][target.X];
		// enum MAP : char { NONE, WALL, GROUND, STAIR, };
		if (after == GROUND || after == STAIR)
		{
			(*pPlayerState) |= _ACTION;
		}
	}
}

// �֐����FConfirmPlayerPickUp	�A�C�e���E���̊m�F
// ���@���FisInput				���̗͂L��
// �@�@�@�FpPlayerState			���@�̏�Ԃւ̃A�h���X
// �߂�l�F
std::vector<FALLITEMLIST>::iterator CRPG::ConfirmPlayerPickUp(bool isInput, int* pPlayerState)
{
	if (isInput == false)
	{
		return m_FallItemList.end();
	}

	if ( ! (   ( ! ((*pPlayerState) & _FOOT))
			&& (   ((*pPlayerState) & _PICKUP))
			&& ( ! ((*pPlayerState) & _USEITEM))))
	{
		return m_FallItemList.end();
	}

	// ���̏ꏊ�ɃA�C�e��������Ύ擾
	std::vector<FALLITEMLIST>::iterator itemIter;
	for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
	{
		COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
		// ���@�Əd�Ȃ��Ă��邩
		if (EQUAL(itemPos, m_pPlayer->GetPosition()))
		{
			// �A�C�e���擾���m�肷��̂�
			// �s���t���O�𗧂Ă�
			(*pPlayerState) |= _ACTION;
			return itemIter;
		}
	}
	return m_FallItemList.end();
}

// �֐����FEnemyDistanceSort	�G�̋������\�[�g
// ���@���F
// �߂�l�F
void CRPG::EnemyDistanceSort()
{
	//
	// �G�̌Ăяo���������R���e�i�ւ̓o�^�����玩�@����̋������߂����ւƕύX����
	//

	// �}���`�}�b�v�ꎞ�ϐ�(�}�����\�[�g�����)
	std::multimap<int, std::vector<CEnemy>::iterator> dII;

	// �C�e���[�^
	std::vector<CEnemy>::iterator eIt;

	// �����ƃC�e���[�^���Z�b�g�ɂ��ă}���`�}�b�v�Ɋi�[
	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		int distance = DISTANCE(m_pPlayer->GetPosition(), (*eIt).GetPosition());
		dII.insert(std::pair<int, std::vector<CEnemy>::iterator>(distance, eIt));
	}

	// 
	// �\�[�g�������Ԃōč\������m_E�ɖ߂�
	//

	// �x�N�^�ꎞ�ϐ�
	std::vector<CEnemy> tmpEnemy;

	// �C�e���[�^
	std::multimap<int, std::vector<CEnemy>::iterator>::iterator dIIiter;
	// �}�����Ƀ\�[�g���ς�ł���̂ŁA�C�e���[�^�ŏ����Ăяo���Ċi�[
	for (dIIiter = dII.begin(); dIIiter != dII.end(); dIIiter++)
	{
		tmpEnemy.push_back((*(dIIiter->second)));
	}

	// �N���X�ϐ��ɑ�����Ė߂�
	m_Enemy = tmpEnemy;
}

// �֐����FEnemyDeadCheck		�G�̎��S�m�F (�ƃ��O�̕\��)
// ���@���F
// �߂�l�F
void CRPG::EnemyDeadCheck()
{
	// �n�`�L�� (��)
	std::string symbol = "-";

	// �C�e���[�^
	std::vector<CEnemy>::iterator eIt;

	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); /* */ )
	{
		if ((*eIt).GetDeadFlag())
		{
			COORD enemyPos = (*eIt).GetPosition();

			// ���S���O
			std::string str;
			str = (*eIt).GetName();
			str += "��|�����B";
			// �o���l�v�Z�ƃ��O�\��
			m_pPlayer->GetEXP((*eIt).GetEXPValue(), str, &m_pCCManager);
			// ���ݒn�̓h��Ԃ�
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, enemyPos, symbol, NOWAIT, DONOTDELAY);

			// �A�C�e�������t���O�������Ă���΃A�C�e�����h���b�v
			if ((*eIt).CanDropItem() == true)
			{
				m_ItemBookSample.index = (*eIt).GetItemIndex();
				m_ItemBookSample.X = enemyPos.X;
				m_ItemBookSample.Y = enemyPos.Y;
				m_FallItemList.push_back(m_ItemBookSample);
				str = (*eIt).GetName() + "��";
				str += m_pItemList[(*eIt).GetItemIndex()].name + "�𗎂Ƃ����B";
				m_pCCManager->AddLog(str);
			}

			// �v�f������
			eIt = m_Enemy.erase(eIt);
			continue;
		}
		// �C�e���[�^��i�߂�
		++eIt;
	}
}

// �֐����FPlayerAction			���@�̍s��
// ���@���FpPlayerState			���@�̏�Ԃւ̃A�h���X
// �@�@�@�FtargetNumber			�U���ΏۂƂȂ�G�̃C���f�b�N�X�ԍ�
// �@�@�@�FpItemIter			�E���ΏۂƂȂ�A�C�e�����w���C�e���[�^�ւ̃A�h���X
// �@�@�@�FnowPosition			���ݒn
// �@�@�@�Ftarget				�ړ���̏ꏊ
// �߂�l�F
void CRPG::PlayerAction(int* pPlayerState, int targetNumber,
	std::vector<FALLITEMLIST>::iterator* pItemIter, COORD nowPosition, COORD target)
{
	// �G�R���e�i�̃C�e���[�^
	std::vector<CEnemy>::iterator eIt;

	// �E���\�A�C�e���R���e�i�̃C�e���[�^
	std::vector<FALLITEMLIST>::iterator itemIter = (*pItemIter);

	// ���@�̃A�C�e�����X�g
	ItemList& IList = m_pPlayer->GetItem();
	ItemList::iterator uiIter;

	if ((*pPlayerState) & _ATTACK)
	{
		// ==== �U�� ====
		int strength = m_pPlayer->GetATK();
		int damage;
		eIt = m_Enemy.begin();
		for (int i = 0; i < targetNumber; i++)
		{
			eIt++;
		}
		damage = (*eIt).ReceiveAttack(strength);

		// ���@�̍s���J�E���^�����߂�
		m_pPlayer->Foot(false);

		// ���O��\��
		std::string cleaner(m_pCCManager->GetScreenSize().Right, ' ');
		std::string str;
		char buffer[256];
		str = m_pPlayer->GetName() + "��[" + (*eIt).GetSymbol() + "]" + (*eIt).GetName() + "���U���I";
		if (damage > 0)
		{
			::_itoa_s(damage, buffer,256 * sizeof(char),10);
			str += buffer;
			str += "�̃_���[�W��^�����I";
		}
		else
		{
			str += "�_���[�W���ʂ�Ȃ������I";
		}
		m_pCCManager->AddLog(str);
	}
	else if (((*pPlayerState) & _FOOT) || ((*pPlayerState) & _PICKUP))
	{
		// ==== �ҋ@/�A�C�e���̏E�� ====
		if ((*pPlayerState) & _PICKUP && itemIter != m_FallItemList.end())
		{
			// ���͊m�F���ɑ������ăq�b�g�����A�C�e�������̂܂ܓo�^����
			// �����A�C�e���ɓo�^
			m_pPlayer->AddItem((*itemIter).index);
			// �擾���O�̍쐬�Ɛݒ�
			std::string str = m_pPlayer->GetName() + "��" + m_pItemList[(*itemIter).index].name + "����ɓ��ꂽ�B";
			m_pCCManager->AddLog(str);
			// �v�f�̍폜
			m_FallItemList.erase(itemIter);
			// �A�C�e�����Ȃ��Ȃ�̂Ō��w�i�F�œh��Ԃ��čĕ\��
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_pPlayer->GetSymbolTXTColor(), target, m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}
		m_pPlayer->Foot(true);
	}
	else if ((*pPlayerState) & _USEITEM)
	{
		// ==== �A�C�e���̎g�p ====
		ITEM nowUseItem = m_pItemList[m_pPlayer->GetUseItemIndex()];
		int oldMaxValue;
		int newMaxValue;
		int healValue;
		char buffer[256];
		std::string str;
		

		switch (nowUseItem.type)
		{
		case MEDICINE:
			// �g�p���O�̍쐬�Ɛݒ�
			str = m_pPlayer->GetName() + "��" + nowUseItem.name + "��H�ׂ��B";
			m_pCCManager->AddLog(str);
			oldMaxValue = m_pPlayer->GetMaxHP();
			healValue = m_pPlayer->Heal(nowUseItem.value);
			newMaxValue = m_pPlayer->GetMaxHP();
			// ���ʂ̕\��
			if (nowUseItem.value == healValue)
			{
				::_itoa_s(nowUseItem.value, buffer, 256 * sizeof(char), 10);
				str  = "�̗͂�";
				str += buffer;
				str += "�񕜂����B";
			}
			else
			{
				str = "�̗͂��S�����Č��C���o���B";
			}
			m_pCCManager->AddLog(str);
			// �ő�l����������Βǉ��\��
			if (oldMaxValue < newMaxValue)
			{
				str = "�̗͂̍ő�l��";
				::_itoa_s(newMaxValue, buffer, 256 * sizeof(char), 10);
				str += buffer;
				str += "�ɂȂ����B";
				m_pCCManager->AddLog(str);
			}
			break;
		case FOOD:
			// �g�p���O�̍쐬�Ɛݒ�
			str = m_pPlayer->GetName() + "��" + nowUseItem.name + "��H�ׂ��B";
			m_pCCManager->AddLog(str);
			oldMaxValue = m_pPlayer->GetHungryLimit();
			healValue = m_pPlayer->EatFood(nowUseItem.value);
			newMaxValue = m_pPlayer->GetHungryLimit();
			// ���ʂ̕\��
			if (nowUseItem.value == healValue)
			{
				::_itoa_s(nowUseItem.value, buffer, 256 * sizeof(char), 10);
				str = "�����x��";
				str += buffer;
				str += "�񕜂����B";
			}
			else
			{
				str = "���Ȃ�����t�ɂȂ��ĂƂĂ��������ꂽ�B";
			}
			m_pCCManager->AddLog(str);
			// �ő�l����������Βǉ��\��
			if (oldMaxValue < newMaxValue)
			{
				str = "�����x�̍ő�l��";
				::_itoa_s(newMaxValue, buffer, 256 * sizeof(char), 10);
				str += buffer;
				str += "�ɂȂ����B";
				m_pCCManager->AddLog(str);
			}
			break;
		case WEAPON:
			// �������O�̍쐬�Ɛݒ�
			str = m_pPlayer->GetName() + "��" + nowUseItem.name + "�𑕔������B";
			m_pCCManager->AddLog(str);
			// �A�C�e���̔ԍ���ݒ肷��
			m_pPlayer->EquippedWeapon(nowUseItem.itemIndex);
			break;
		case SHIELD:
			// �������O�̍쐬�Ɛݒ�
			str = m_pPlayer->GetName() + "��" + nowUseItem.name + "�𑕔������B";
			m_pCCManager->AddLog(str);
			// �A�C�e���̔ԍ���ݒ肷��
			m_pPlayer->EquippedShield(nowUseItem.itemIndex);
			break;
		case ARMOR:
			// �������O�̍쐬�Ɛݒ�
			str = m_pPlayer->GetName() + "��" + nowUseItem.name + "�𑕔������B";
			m_pCCManager->AddLog(str);
			// �A�C�e���̔ԍ���ݒ肷��
			m_pPlayer->EquippedArmor(nowUseItem.itemIndex);
			break;
		default:
			assert(false);
			break;
		}

		if (nowUseItem.type == MEDICINE || nowUseItem.type == FOOD)
		{
			// �A�C�e�����̍팸�ƃ��X�g����̍폜
			uiIter = IList.find(nowUseItem.itemIndex);
			uiIter->second--;
			if (uiIter->second == 0)
			{
				IList.erase(uiIter);
			}
			m_pPlayer->SetItemList(std::move(IList));
		}
	}
	else
	{
		// ==== �ړ� ====
		// �ړ��O�̈ʒu��n�`�ŏ�h��
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, m_TerrainSymbol[GROUND], NOWAIT, DONOTDELAY);
		nowPosition = target;

		// �A�C�e���𓥂ނ����m�F
		bool isOverRap = false;
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
			if (EQUAL(itemPos, nowPosition))
			{
				isOverRap = true;
				break;
			}
		}

		// �ړ���̈ʒu�Ɏ��@��\��
		if (isOverRap == true)
		{
			// �A�C�e���𓥂�ł�����A�C�e���̔w�i�F��\��
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_pPlayer->GetSymbolTXTColor(), target, m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}
		else
		{
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_pPlayer->GetSymbolTXTColor(), target, m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}

		m_pPlayer->Move(nowPosition);
	}
}

// �֐����FEnemyAction			�������Ă���G�̍s��
// ���@���F
// �߂�l�F
void CRPG::EnemyAction()
{
	COORD oldPos;
	COORD newPos;

	// �G�R���e�i�̃C�e���[�^
	std::vector<CEnemy>::iterator eIt;

	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); /**/ )
	{
		for (int j = 0; j < (*eIt).GetNumAction(); j++)
		{
			oldPos = (*eIt).GetPosition();
			(*eIt).DecideAction(&m_pPlayer, &m_Enemy, m_Enemy.size(), &m_pppMapData[m_NowMapNumber], m_Width, m_Height, &m_pCCManager);
			newPos = (*eIt).GetPosition();

			// �ړ����������ꍇ���̏ꏊ�Ɍ��̒n�`��\������
			if ( false == EQUAL(oldPos, newPos))
			{
				// ���ݒn�̓h��Ԃ�
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, oldPos, m_TerrainSymbol[GROUND], NOWAIT, DONOTDELAY);
			}
		}

		// ���W�̍X�V
		newPos = (*eIt).GetPosition();

		// �A�C�e���𓥂�ł��邩�̃t���O
		bool isOverRap = false;

		// �E���\�A�C�e���R���e�i�̃C�e���[�^
		std::vector<FALLITEMLIST>::iterator itemIter;

		// �������Ă���G�̏ꏊ�ɓG�}�[�N��\��
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
			// �A�C�e���𓥂�ł�����A�C�e���̔w�i�F��\��
			if (EQUAL(itemPos, newPos))
			{
				isOverRap = true;
				break;
			}
		}
		if (isOverRap == true)
		{
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, (*eIt).GetTXTColor(), newPos, (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
		}
		else
		{
			m_pCCManager->WriteTextCharacter(m_GameBGColor, (*eIt).GetTXTColor(), newPos, (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
		}

		// �C�e���[�^��i�߂�
		++eIt;
	}
}

// �֐����FViewItem				�A�C�e���̕\��
// ���@���F
// �߂�l�F
void CRPG::ViewItem()
{
	// �C�e���[�^
	std::vector<FALLITEMLIST>::iterator itemIter;

	// �A�C�e���R���e�i�̗v�f�S��
	for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
	{
		// ���W
		COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);

		// ==== ���@�Əd�Ȃ��Ă��邩 ====
		if (EQUAL(itemPos, m_pPlayer->GetPosition()))
		{
			continue;
		}

		// ==== �G�Əd�Ȃ��Ă��邩 ====
		bool canView = true;
		std::vector<CEnemy>::iterator eIt;
		for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++ )
		{
			if (false == (*eIt).GetDeadFlag() && EQUAL(itemPos, (*eIt).GetPosition()))
				{
					canView = false;
					break;
				}
		}
		if (canView == false)
		{
			continue;
		}

		// �\��
		m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_ItemTXTColor, itemPos, m_ItemSymbol, NOWAIT, DONOTDELAY);
	}
}

// �֐����FConfirmDungeonState	�K�w�N���A�A���S�A�S�[�����̃��O�\���ƑJ��
// ���@���Fpos					�\���ʒu
// �߂�l�F
int CRPG::ConfirmDungeonState()
{
	//enum DUNGEONENDMODE : int { PLAYERDEAD, HIERARCHYCLEAR, DUNGEONCLEAR, CONTINUATION, };

	// ��ʂ̃N���A
	m_pCCManager->FillAll(m_GameBGColor, m_GameTXTColor, m_pCCManager->GetCSBInfo());

	// �\���ʒu (���S)
	COORD center = {m_Width / 2, m_Height / 2};

	// ���@�̎��S
	if (true == m_pPlayer->IsDead())
	{
		m_NowMapNumber = 0;
		m_GameScene = DEATH;
		std::string view  = "## YOU DIED. OH MY GODDESS X( ##";
		std::string clean = "                                                ";
		center.X -= view.size() / 2;
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_RED, center, view, WAIT, DONOTDELAY);
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, center, clean, NOWAIT, DONOTDELAY);
		return PLAYERDEAD;
	}
	else
	{
		// �K�w�̃N���A
		if (++m_NowMapNumber != m_NumMap)
		{
			std::string view  = "## GOAL!! GO TO THE NEXT FLOOR. PRESS ENTER KEY.##";
			std::string clean = "                                                  ";
			center.X -= view.size() / 2;
			m_pCCManager->WriteTextCharacter(m_GameBGColor, H_RED, center, view, WAIT, DONOTDELAY);
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, center, clean, NOWAIT, DONOTDELAY);
			return HIERARCHYCLEAR;
		}
		// �_���W�����S�̂̃N���A
		else
		{
			std::string view  = "## DUNGEON CLEAR!! PRESS ENTER KEY TO FINISH.##";
			std::string clean = "                                               ";
			center.X -= view.size() / 2;
			m_pCCManager->WriteTextCharacter(m_GameBGColor, H_RED, center, view, WAIT, DONOTDELAY);
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, center, clean, NOWAIT, DONOTDELAY);
			
			// ������������������������������������������������
			// �X�ɋA�邩�A�N���A�ɂȂ邩
			m_GameScene = CREDIT;
			// ������������������������������������������������

			m_NowMapNumber = 0;
			return DUNGEONCLEAR;
		}
	}
	return CONTINUATION;
}