#include "RPG.h"

// �֐����FTitle			�^�C�g��
// ���@���F
// �߂�l�F
void CRPG::Title()
{
	// �J�[�\���̔�\��
	m_pCCManager->ExchangeCursorVisible(FALSE);

	// ��ʂ̃N���A
	m_pCCManager->FillAll(L_BLUE, H_YELLOW, m_pCCManager->GetCSBInfo());

	// ���@�p�����[�^�̃��Z�b�g
	m_pPlayer->ResetParameter();

	// ==== �I�[�v�j���O�̕\�� ====
	COORD pos = {0, 0};
	COORD opPos = {20, 4};
	const int opHeight = 16;

	// ��������̑��x
	int charaPopSpeed = 70;

	// �s����𑁂߂�s���̎w��
	int slowViewRowNumber = 7;
	std::string openingText[opHeight] =
	{
		"�Ɋ��̑�n�T���C�h�E�̃T�b�|�R���a���B���̍��̒n���[���ɂ́@",
		"�`�J�z�Ƃ�΂��L��ȃ_���W�������������B�@�@�@�@�@�@�@�@�@",
		"�_���W�����̍ŉ��ɂ͖ڂ�ῂނ悤�ȋ�����󂪖����Ă���Ɓ@�@",
		"�����`�����Ă���E�E�E�E�E�E�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@",
		"�������ɁA���̃_���W�����ɒ������Ƃ����҂���l�B�@�@�@�@�@",
		"�@�@�@�@�@�@�@  �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@",
		"�ނ̖��́E�E�E�E�E�E�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@",
		"�@�@�@�@�@�@�@  �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@",
		"               _�l_�l_�l_�l_�l_�l_�l_�l_                    ",
		"              ���@�C���^�[���̃V�����@ ��                   ",
		"               ^�x^�x^�x^�x^�x^�x^�x^�x^                    ",
		"                                                            ",
		"                                            Press Enter Key "
	};

	// �Ē��펞�̕\���ύX
	if (m_Retry == true)
	{
		charaPopSpeed = 5;
		slowViewRowNumber = opHeight;
		openingText[ 0] = "����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@";
		openingText[ 1] = "�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@ �@�@�@�@";
		openingText[ 2] = "����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@";
		openingText[ 3] = "�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@ �@�@�@�@";
		openingText[ 4] = "����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@";
		openingText[ 5] = "�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@ �@�@�@�@";
		openingText[ 6] = "                                                            ";
		openingText[ 7] = "                  ���̎�͂܂����E�E�E�I                    ";
		openingText[ 8] = "                                                            ";
		openingText[ 9] = "�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@ �@�@�@�@";
		openingText[10] = "����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@";
		openingText[11] = "�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@ �@�@�@�@";
		openingText[12] = "����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@�@";
		openingText[13] = "�@�@�@�@����...�@�@�@�@����...�@�@�@�@����...�@�@�@ �@�@�@�@";
		openingText[14] = "����...�@�@�@�@����...�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@";
		openingText[15] = "                      Press Enter Key  -> Confirm Result    ";
	}

	// ==== �I�[�v�j���O�e�L�X�g�̕\�� ====
	// �N���W�b�g�̃��[���A�b�v���Ԍ���p
	float oneCycleDelay = 2.0f;
	if (m_Retry == true)
	{
		oneCycleDelay = 0.01f;
	}
	DWORD startTime = timeGetTime();
	DWORD nowTime;
	float elapsedTime;

	// �J�E���^
	int count = 0;
	int countEnd = opHeight;
	pos = opPos;
	
	Sleep(200);
	for(;;)
	{
		Sleep(1);
		// �L�[���͌��o
		m_pCCManager->Trigger();
		// �^�C�}�[�J�E���g
		nowTime = timeGetTime();
		elapsedTime = static_cast<float>(nowTime - startTime) / 1000.0f;
		if (elapsedTime >= oneCycleDelay || m_pCCManager->Press(KEY::RETURNKEY))
		{
			// �^�C�}�[���Z�b�g
			startTime = timeGetTime();
			// �\��
			if (count < slowViewRowNumber)
			{
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, openingText[count], NOWAIT, charaPopSpeed);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, openingText[count], NOWAIT, DONOTDELAY);
				oneCycleDelay = 0.5f;
			}
			pos.Y++;
			// �I������
			if (++count >= countEnd)
			{
				break;
			}
		}
	}
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, "", WAIT, DONOTDELAY);

	// ==== �^�C�g���̕\�� ====
	const int tiHeight = 16;
	std::string titleLogo[tiHeight] =
	{
		"�@�@�����������������������@ �@ �@  �����������@�@�@���������������@ �@ ����������",
		"�@�������������������������@ �@ �@�@�����������@�@�@���������������@ �@ ����������",
		"���������@ �@ �����������������������@ �@ �����@�@�@���������������@�����@ �@ ����",
		"�������@ �@ ������������������������ �@ �������@�@�@�������������������� �@ ������",
		"�@�����@�������������������@ �@ �@  �����������@�@�@������������������������������",
		"�@�����@���������@ �@ �����@ �@   �@���������@�@�́@�������������������@���������@ "
	};

	// ���S�\������
	int height = 6;

	// �Ē��펞�Ƀ��S��ύX
	if (m_Retry == true)
	{
		height = tiHeight;
		titleLogo[ 0] = "�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ ";
		titleLogo[ 1] = "�@ �^ �P/�@�\�@ /�@�Q|�Q_�@�Q__|�Q�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@";
		titleLogo[ 2] = "�@�@ �\/�@�@�@ / �@ �b �^ �@�@/|�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@  ";
		titleLogo[ 3] = " �@ �Q/�@�@�Q�^�@ �@ |�@�@�Q�^�b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@";
		titleLogo[ 4] = "�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ /�_�Q�Q�Q�Q_�^�R�@�@�@�@�@�@";
		titleLogo[ 5] = "�@�@ (.�M�R(�M�� �A�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�^''''�@�@�@''''::�_�@�@�@�@�@";
		titleLogo[ 6] = "�@ �@ `'���M�Tr'́_�@�@�@�@�@�@�@�@�@�@�@ �@ �{�@�@|�i���j,�@ ��i���j�:|�@�{�@�@ ";
		titleLogo[ 7] = " �����L�^�L�@ �m, �^��)�@�@�@�@�@�@�@�@ �@ �@ �@�@ |�@,,,�(�_,)�R�,,�@:|�@�@�@�@ ";
		titleLogo[ 8] = "�@��l��L7_�@/�@-�T-')�L�@�@�@�@�@�@�@�@ �@�@�@�@+�@|�@�@�M-=�=- '   :::|�@+ �@�@ ";
		titleLogo[ 9] = "�@�@�@�@�@ �__�@�@�__,.��_�@�@�@�@�@�@�@�@�{�@�@�@�@�_�@�@�M�ƁL�@ ::�^�@�@�@�@+ ";
		titleLogo[10] = "�@�@�@�@�@�@�@(T�Q_�m�@�@ T�R�@�@�@�@, -r'��!�P ` :::7�R�M- ��@�@�@./|�@�@�@�@�@ ";
		titleLogo[11] = "�@�@�@�@�@�@�@�@�R��.�@�@�@/�`�-��<�1�L|�@�R�@| :::::āA�_�@(�@�@./�@�R�@�@�@�@�@";
		titleLogo[12] = "�@�@�@�@�@�@�@�@�@�_l__,.�^�@i�@l �R!�@|�@�@�@| :::::l �R �M�V�.��]'�L|�_-��@�@�@";
		titleLogo[13] = " �Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q  �Q�Q�@�@�@�@�@�@�@�@�@�@";
		titleLogo[14] = "���񄠎O���l���񄠎O���l���񄠎O���l���������������ބ��b�ށb�@�@�@�@�@�@�@�@�@�@ ";
		titleLogo[15] = "���݄��݄��݄��݄��݄��݄��݄��݄��݄��������������݄��b�݁b�@�������Q�[���ł��@ ";
	}

	int startHeight = 5;
	COORD startPos = {m_Width / 2 - titleLogo[0].length() / 2, startHeight};

	std::string _start		= "START";
	std::string _continue	= "CONTINUE";
	std::string _exit		= "EXIT";
	int betMenuMargin = 3;
	COORD sLogoPos = {m_Width / 2 - _start.length() / 2,	height + startPos.Y + betMenuMargin};
	COORD lLogoPos = {m_Width / 2 - _continue.length() / 2,		     sLogoPos.Y + betMenuMargin};
	COORD eLogoPos = {m_Width / 2 - _exit.length() / 2,				 lLogoPos.Y + betMenuMargin};

	COORD savePosition = m_pCCManager->GetCursorPos();

	m_pCCManager->FillAll(L_BLUE, H_YELLOW, m_pCCManager->GetCSBInfo());
	COORD nowPosition = sLogoPos;

	// About View Cursor
	enum MODE : char {_START, _CONTINUE, _EXIT};
	char mode = _START;

	// �^�C�g�����S�̕\��
	for (int i = 0; i < height; i++)
	{
		pos.X = startPos.X;
		pos.Y = startPos.Y + i;
		m_pCCManager->SetCursorPos(pos);
		m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, titleLogo[i], NOWAIT, DONOTDELAY);
	}
	// �ĕ\���t���O
	m_Retry = true;

	// ���ڂ̕\��
	m_pCCManager->SetCursorPos(eLogoPos);
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, eLogoPos, _exit, NOWAIT, DONOTDELAY);
	m_pCCManager->SetCursorPos(lLogoPos);
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, lLogoPos, _continue, NOWAIT, DONOTDELAY);
	m_pCCManager->SetCursorPos(sLogoPos);
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, sLogoPos, _start, NOWAIT, DONOTDELAY);

	SHORT div = -5;
	sLogoPos.X += div;
	lLogoPos.X += div;
	eLogoPos.X += div;
	std::string cursor = "��";
	std::string space = "  ";
	// �����I�����ڂɃJ�[�\����\��
	m_pCCManager->SetCursorPos(sLogoPos);
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, sLogoPos, cursor, NOWAIT, DONOTDELAY);
	m_pCCManager->SetCursorPos(sLogoPos);
	for(;;)
	{
		// �L�[���͌��o
		m_pCCManager->Trigger();
		bool isInput = false;
		if (m_pCCManager->PulseOn(KEY::UPKEY))
		{
			if (mode > 0)
			{
				mode--;
				isInput = true;
			}
		}
		if (m_pCCManager->PulseOn(KEY::DOWNKEY))
		{
			if (mode < 2)
			{
				mode++;
				isInput = true;
			}
		}
		if (isInput == true)
		{
			// �I�����ڂ̃J�[�\���\���Ɣ�I�����ڂ̃J�[�\���Փh��Ԃ�����
			switch (mode)
			{
			case _START:
				m_pCCManager->SetCursorPos(sLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, sLogoPos, cursor, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(lLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, lLogoPos, space, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(eLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, eLogoPos, space, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(sLogoPos);
				break;
			case _CONTINUE:
				m_pCCManager->SetCursorPos(lLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, lLogoPos, cursor, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(sLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, sLogoPos, space, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(eLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, eLogoPos, space, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(lLogoPos);
				break;
			case _EXIT:
				m_pCCManager->SetCursorPos(eLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, eLogoPos, cursor, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(sLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, sLogoPos, space, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(lLogoPos);
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, lLogoPos, space, NOWAIT, DONOTDELAY);
				m_pCCManager->SetCursorPos(eLogoPos);
				break;
			}
		}
		if (m_pCCManager->PulseOn(KEY::RETURNKEY))
		{
			break;
		}
		Sleep(10);
	}
	switch (mode)
	{
	case _START:
		m_GameScene = DUNGEON;
		break;
	case _CONTINUE:
		nowPosition.X = 0;
		nowPosition.Y = 1;
		m_pCCManager->SetCursorPos(nowPosition);
		//m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, "�Z�[�u�f�[�^��T���܂��B", NOWAIT, DONOTDELAY);
		//m_GameScene = CONTINUE;
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, "���ݖ������ł��B�{�҂��͂��߂܂�", WAIT, DONOTDELAY);
		m_GameScene = DUNGEON;
		break;
	case _EXIT:
		nowPosition.X = 0;
		nowPosition.Y = 1;
		m_pCCManager->SetCursorPos(nowPosition);
		m_EndFrag = true;
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, "�Q�[�����I�����܂��BENTERKEY�������Ă��������B", WAIT, DONOTDELAY);
		break;
	}
}
