#include "RPG.h"

// �֐����FDungeon			�_���W�����V�[��
// ���@���F
// �߂�l�F
void CRPG::Dungeon()
{
	// �������� �_���W�����K�w���[�v ��������
	for (;;)
	{
		// ==== ��ʂ̃N���A ====
		m_pCCManager->FillAll(m_GameBGColor, m_GameTXTColor, m_pCCManager->GetCSBInfo());

		// ==== �J�[�\���̔�\�� ====
		m_pCCManager->ExchangeCursorVisible(FALSE);

		// ==== �ړ��\�n�`�̃T���v�����O ====
		std::multimap<char, char> ground = ViewMapAndGetGround();
		// �ۑ��p�ɕ���
		std::multimap<char, char> saveAllGround = ground;
		// �����p�C�e���[�^
		std::multimap<char, char>::iterator gIter;

		COORD pos = {0, 0};
		std::string symbol;

		// ==== �A�C�e���̏�����(��) ====
		ResetItem();
		// ==== �A�C�e���̔z�u ====
		ItemPlace(ground);
		// �n�`�f�[�^�̕���
		ground = saveAllGround;

		// ==== ���@�̔z�u ====
		PlayerPlace(&ground);

		// ==== �G�̏����� ====
		ResetEnemy();

		// ���� ���E��ނ̓}�l�[�W��������ĊǗ������� ��������������������
		// �����G���̌���
		m_NumStartMaxEnemy = ground.size() / 25; // 4%
		unsigned int numEnemy = rand() % m_NumStartMaxEnemy + (m_NowMapNumber + 1) * 2; // + ��
		// �����̓G���͔z�u�ł��鐔�Ɏ��߂�
		if (numEnemy >= ground.size() - 1)
		{
			numEnemy = ground.size() - 1;
		}
		// �G��ނ̌���
		for (unsigned int i = 0; i < numEnemy; i++)
		{
			int randomIndex;
			// 5�K�w���Ƃ�5��ނ���
			randomIndex = ((rand() % 5) + (5 * (m_NowMapNumber / 5))) % m_NumEnemyType;
			// �}�Ӄf�[�^�̎擾
			SetEnemyData(randomIndex);
			// �G�̓o�^
			m_Enemy.push_back(m_Sample);
		}
		numEnemy = m_Enemy.size();
		// ������������������������������������������������������������

		// ==== �G�@�̔z�u ====
		std::vector<CEnemy>::iterator eIt;
		int enemyIndex = 0;
		EnemyPlace(&ground);

		// �n�`�f�[�^�̕���
		ground = std::move(saveAllGround);

		// �C���t�H�\���p�̊J�n�ʒu
		COORD infoPos  = {0, 0};
		COORD helpPos1 = {0, 0};
		COORD helpPos2 = {0, 0};
		helpPos1.Y = m_pCCManager->GetScreenSize().Bottom - 1;
		helpPos2.Y = helpPos1.Y + 1;
		infoPos.Y = helpPos1.Y - 1;

		// �A�C�e���̕\��
		std::vector<FALLITEMLIST>::iterator itemIter;
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_ItemTXTColor, MAKECOORD((*itemIter).X, (*itemIter).Y), m_ItemSymbol, NOWAIT, DONOTDELAY);
		}

		// ==== ���@���̕\�� ====
		m_pPlayer->ViewInfo(infoPos, &m_pCCManager);

		// ==== �K�w�ԍ��E�w���v�̕\�� ====
		// �K�w�ԍ�
		char buf[256];
		::_itoa_s(m_NowMapNumber + 1, buf, sizeof(char) * 256, 10);
		std::string mapNumberStr = "�_���W�����F�`�J�z";
		mapNumberStr += buf;
		mapNumberStr += "�K";
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, m_MapNamePosition, mapNumberStr, NOWAIT, DONOTDELAY);
		// �w���v
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos1, m_Help[0], NOWAIT, DONOTDELAY);
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos2, m_Help[1], NOWAIT, DONOTDELAY);

		// �A�C�e���𓥂�ł��邩�̃t���O
		bool isOverRap = false;

		// ���@�̕\��
		isOverRap = false;
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
			// �A�C�e���𓥂�ł�����A�C�e���̔w�i�F��\��
			if (EQUAL(itemPos, m_pPlayer->GetPosition()))
			{
				isOverRap = true;
				break;
			}
		}
		if (isOverRap == true)
		{
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_pPlayer->GetSymbolTXTColor(), m_pPlayer->GetPosition(), m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}
		else
		{
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_pPlayer->GetSymbolTXTColor(), m_pPlayer->GetPosition(), m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}

		// ==== �G�@�̕\�� ====
		for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
		{
			isOverRap = false;
			for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
			{
				COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
				// �A�C�e���𓥂�ł�����A�C�e���̔w�i�F��\��
				if (EQUAL(itemPos, (*eIt).GetPosition()))
				{
					isOverRap = true;
					break;
				}
			}
			if (isOverRap == true)
			{
				m_pCCManager->WriteTextCharacter(m_ItemBGColor, (*eIt).GetTXTColor(), (*eIt).GetPosition(), (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(m_GameBGColor, (*eIt).GetTXTColor(), (*eIt).GetPosition(), (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
			}
		}

		// �s�����O�X�N���[���\���p
		int scrollCount = 0;
		int numScroll = 0;

		// �s�����O�̏���
		m_pCCManager->CleanLog();
		m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

		// ���j���[�\���p
		bool isOpenMenu = false;
		int menuScrollCount = 0;

		// �������� �_���W�������s�����[�v ��������
		for(;;)
		{
			// �L�[���͌��o
			m_pCCManager->Trigger();

			// �G�Ǘ����ǉ������肵����
			// �ǉ��Ŕz�u�ł���ꏊ�𑖍����A���̈ʒu�ɓG��z�u����

			// �G�C���f�b�N�X�̍X�V
			RefreshEnemyIndex();

			// ==== ���́F���@�s�� ====
			COORD nowPosition = m_pPlayer->GetPosition();
			COORD target = nowPosition;
			// �t���O
			bool isInput = false;	// ���@�s���p�̓��̗͂L��
			bool useItem = false;
			//enum STATE : int { _ACTION = 0x0001, _FOOT = 0x0002, _ATTACK = 0x0004, _PICKUP = 0x0008, _USEITEM = 0x0010, };
			int playerState = 0;
			// �U���\�ȑ���̃C���f�b�N�X�ԍ�
			int targetNumber = 0;
			// �g�p����A�C�e���̔ԍ�
			int selectItemIndex = 0;
			ItemList& IList = m_pPlayer->GetItem();
			ItemList::iterator uiIter = IList.begin();

			// ==== ���j���[�\�� ====
			if (true == Menu(&isOpenMenu, &useItem, &menuScrollCount))
			{
				continue;
			}

			// �A�C�e���g�p�̗v�����������ꍇ�A�ʏ푀������D�悷��
			if (useItem == true)
			{
				isInput = true;
				playerState |= _USEITEM;
				playerState |= _ACTION;
			}
			else
			{
				// ==== ���́F���O�X�N���[�� ====
				if (true == LogScroll(&scrollCount, &numScroll))
				{
					continue;
				}

				// ���@�s���p�̓��͊m�F
				PlayerActionInput(nowPosition, &target, &isInput, &playerState);

				// ���͎���PLAYER���ړ��\���𔻒�
				ConfirmPlayerMove(isInput, &playerState, &targetNumber, target);

				// �A�C�e�����E���邩���m�F
				itemIter = ConfirmPlayerPickUp(isInput, &playerState);
			}


			// ==== ���͊m�莞�̍s������ (�ƃ��O�\��) ====
			if (playerState & _ACTION)
			{
				// �s�����m�肵���̂ŕێ����Ă������O����������
				m_pCCManager->CleanLog();
				scrollCount = 0;
				numScroll = 0;

				// �������� ���@�̍s�� ��������
				PlayerAction(&playerState, targetNumber, &itemIter, nowPosition, target);

				// ==== ���@���S����(��) ====
				if (true == m_pPlayer->IsDead())
				{
					break;
				}

				// ==== ���B���� ====
				nowPosition = m_pPlayer->GetPosition();
				if (STAIR == m_pppMapData[m_NowMapNumber][nowPosition.Y][nowPosition.X])
				{
					break;
				}

				// �������� �G�̍s�� ��������
				// ==== �G���������Ƀ\�[�g ====
				EnemyDistanceSort();
			
				// ==== ���S�����G�𑖍����Čo���l�v�Z�ƃ��O��\�� ====
				EnemyDeadCheck();

				// ==== �������Ă���G�̍s�� ====
				EnemyAction();

				// ���@���S����(�G����̍U��)
				if (true == m_pPlayer->IsDead())
				{
					break;
				}

				// �������� �A�C�e���̕\�� ��������
				ViewItem();

				// ���@���̕\��
				m_pPlayer->ViewInfo(infoPos, &m_pCCManager);
				// �w���v�̕\��
				m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos1, m_Help[0], NOWAIT, DONOTDELAY);
				// �w���v�̕\��
				m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos2, m_Help[1], NOWAIT, DONOTDELAY);

				// ��񃍃O�̕\��
				m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);
				m_pCCManager->ViewLog(m_LogPosition, m_GameBGColor, m_LogTXTColor, 0, m_WindowLogSize);

			}// end if (canAction == true)
			
			// �ҋ@���Ԃ̐ݒ�
			Sleep(10);
		}

		// ==== �_���W�����p�[�g�̏I������ (�ƕ\��) ====
		//enum DUNGEONENDMODE : int { PLAYERDEAD, HIERARCHYCLEAR, DUNGEONCLEAR, CONTINUATION, };
		int endMode = ConfirmDungeonState();
		if (endMode == PLAYERDEAD || endMode == DUNGEONCLEAR)
		{
			break;
		}
	}
}