#include "RPG.h"

// �֐����FCreditRoll			�N���W�b�g�V�[��
// ���@���F
// �߂�l�F
void CRPG::CreditRoll()
{
	// �o�b�N�O���E���h�F�w��
	COLOR bBack = L_BLACK;
	COLOR bText = H_WHITE;
	// �N���W�b�g�w�i�A�����F�w��
	COLOR cBack = L_BLACK;
	COLOR cText = H_GREEN;
	// �X�L�b�v�\���w�i�A�����F�w��
	COLOR sBack = L_BLACK;
	COLOR sText = H_YELLOW;

	// �J�[�\���̔�\��
	m_pCCManager->ExchangeCursorVisible(FALSE);

	// ��ʂ̃N���A
	m_pCCManager->FillAll(bBack, bText, m_pCCManager->GetCSBInfo());

	// �N���W�b�g�̃��[���A�b�v���Ԍ���p
	float oneCycleDelay = 0.5f;
	DWORD startTime = timeGetTime();
	DWORD nowTime;
	float elapsedTime;

	// �s�\���Ԃ̒x��
	int onRowDelay = 100;

	// ��������̑��x
	int charaPopSpeed = 70;

	// �J�E���^
	int count = 0;
	int rollHeight = m_Height;
	int countEnd = rollHeight;
	const int numRows = 12;

	// �N���W�b�g
	std::string endingCredit[numRows] =
	{
		"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
		"@ GAME : No-Named RogueLike Dungeon RPG GAME (Windows 7 / Console / C++)       @",
		"@                                                         Date: 2016.01.07-22  @",
		"@ This Game was Created by A.I. (Maybe, He is Human Race)                      @",
		"@ Good Superviser : <removed>       F    riendly Adviser  : <removed>          @",
		"@ Kind Neighbor   : <removed>                                                  @",
		"@                                                                              @",
		"@  Thank you for playing. If you like, Please try to modify dungeon data       @",
		"@   as your original one,  XD                                                  @",
		"@                   Let's meet again Sometime and Somewhere !!                 @",
		"@                                                                              @",
		"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	};
	std::string cleaner = "";
	for (unsigned int i = 0; i < endingCredit[0].size(); i++)
	{
		cleaner += " ";
	}

	COORD startPos = {m_Width / 2 - endingCredit[0].length() / 2, rollHeight};
	COORD pos = startPos;
	std::string sceneNameView = "<CREDIT ROLL>";
	std::string menu = "Press Enter Key to Title Back";
	int margin = 3;
	COORD menuPos = {m_Width / 2 - menu.length() / 2, numRows + margin};

	// �����I�����ڂɃJ�[�\����\��
	m_pCCManager->SetCursorPos(startPos);
	bool isComplete = false;
	for(;;)
	{
		Sleep(1);
		// �L�[���͌��o
		m_pCCManager->Trigger();
		// ���[���X�L�b�v
		if (m_pCCManager->Press(KEY::RETURNKEY))
		{
			if (isComplete == false)
			{
				count = countEnd - 1;
			}
		}

		if (isComplete == false)
		{
			// �^�C�}�[�J�E���g
			nowTime = timeGetTime();
			elapsedTime = static_cast<float>(nowTime - startTime) / 1000.0f;
			if (elapsedTime >= oneCycleDelay)
			{
				// �^�C�}�[���Z�b�g
				startTime = timeGetTime();

				// �Ō�̈�񂾂���ʂ��t���b�V��
				if (count >= countEnd - 1)
				{
					m_pCCManager->FillAll(bBack, bText, m_pCCManager->GetCSBInfo());
				}

				// �w��͈͂ɃN���W�b�g�̕\��
				for (int i = 0; i < numRows; i++)
				{
					pos.Y = startPos.Y + i - count;
					if (pos.Y <= 0)
					{
						continue;
					}
					if (pos.Y >= rollHeight)
					{
						break;
					}
					m_pCCManager->SetCursorPos(pos);
					// �N���W�b�g(1��)
					if (count < countEnd - 1)
					{
						m_pCCManager->WriteTextCharacter(cBack, cText, pos, endingCredit[i], NOWAIT, DONOTDELAY);
					}
					else
					{
						m_pCCManager->WriteTextCharacter(cBack, cText, pos, endingCredit[i], NOWAIT, charaPopSpeed);
						Sleep(onRowDelay);
					}
				}
				// �N���W�b�g�̂��݂��t���b�V��
				for (int i = numRows; i < numRows + 1; i++)
				{
					pos.Y = startPos.Y + i - count;
					if (pos.Y <= 0)
					{
						continue;
					}
					if (pos.Y >= rollHeight)
					{
						break;
					}
					m_pCCManager->SetCursorPos(pos);
					m_pCCManager->WriteTextCharacter(bBack, bText, pos, cleaner, NOWAIT, DONOTDELAY);
				}
				// �J�E���g�A�b�v
				count++;
				// �I������
				if (count >= countEnd)
				{
					m_GameScene = TITLE;
					isComplete = true;;
				}
			}
			// �X�L�b�v�\��
			if(count < countEnd)
			{
				COORD p = {0, 0};
				m_pCCManager->SetCursorPos(p);
				m_pCCManager->WriteTextCharacter(sBack, sText, p, sceneNameView + " " + menu, NOWAIT, DONOTDELAY);
			}
			else
			{
				m_pCCManager->SetCursorPos(menuPos);
				m_pCCManager->WriteTextCharacter(sBack, sText, menuPos, menu, NOWAIT, charaPopSpeed);
			}

		}
		// �I�� (�^�C�g���o�b�N)
		if (m_pCCManager->PulseOn(KEY::RETURNKEY))
		{
			m_GameScene = TITLE;
			break;
		}
	}
}
