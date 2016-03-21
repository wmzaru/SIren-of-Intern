#include "ConsoleManager.h"

// �R���X�g���N�^
CConsoleManager::CConsoleManager() : m_Wcs(nullptr)
{
	Initialize();
}

// �f�X�g���N�^
CConsoleManager::~CConsoleManager()
{
	CloseHandle(m_Handle);
	if (m_Wcs != nullptr)
	{
		delete [] m_Wcs;
	}
}

// �֐����FInitialize		������
// ���@���F
// �߂�l�F
void CConsoleManager::Initialize()
{
	// �p�b�h�f�[�^�̏�����
	m_Pad.normal			= 0;
	m_Pad.alltrig			= 0;
	m_Pad.endtrig			= 0;
	m_Pad.old				= 0;
	m_Pad.trig				= 0;
	m_CsDefault.dwSize		= 100; // 1 �` 100
	m_CsDefault.bVisible	= TRUE;
	m_CsInfo.dwSize			= 100;
	m_CsInfo.bVisible		= FALSE;
	m_MaxBufferSize.X		= 101;
	m_MaxBufferSize.Y		= 36;
	m_Position.X			= 0;
	m_Position.Y			= 0;
	m_RctWindowRect.Left	= 0;
	m_RctWindowRect.Top		= 0;
	m_RctWindowRect.Right	= m_MaxBufferSize.X - 1;
	m_RctWindowRect.Bottom	= m_MaxBufferSize.Y - 1;

	m_Handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

	// �F�̐ݒ�
	SetColor(L_PURPLE, H_WHITE);

	// �t�H���g�T�C�Y�̐ݒ�
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	::GetCurrentConsoleFontEx(m_Handle, 0, &info);
	// �� default = (8, 18), max = (12, 27), min = (1, 1)
	info.dwFontSize.X = 12;
	info.dwFontSize.Y = 27;
	::SetCurrentConsoleFontEx(m_Handle, 0, &info);

	// �o�b�t�@�T�C�Y�̐ݒ�
	SetScreenSize(m_MaxBufferSize, m_RctWindowRect);

	Sleep(0);

	// �^�C�g���̐ݒ�
	::SetConsoleTitle("20160107 - 0122 Intern RPG ROGUELIKE GAME made by A.I.");

	// �J�[�\�����̐ݒ�
	::SetConsoleCursorInfo(m_Handle, &m_CsDefault);

}

// �֐����FTrigger			�g���K�[���o
// ���@���F
// �߂�l�F
__forceinline void CConsoleManager::Trigger() const
{
	Sleep(0);

	m_Pad.old = m_Pad.normal;	// �ۑ�

	// �I��
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		exit(0);
	}

	// ----- �L�[���͏��� -----
	int KeyData = 0;
	if (GetKeyState(VK_UP) & 0x8000)
	{
		KeyData |= KEY::UPKEY;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		KeyData |= KEY::DOWNKEY;
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		KeyData |= KEY::LEFTKEY;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		KeyData |= KEY::RIGHTKEY;
	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		KeyData |= KEY::SPACEKEY;
	}
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		KeyData |= KEY::RETURNKEY;
	}
	if (GetKeyState('Q') & 0x8000)
	{
		KeyData |= KEY::QKEY;
	}
	if (GetKeyState('W') & 0x8000)
	{
		KeyData |= KEY::WKEY;
	}
	if (GetKeyState('E') & 0x8000)
	{
		KeyData |= KEY::EKEY;
	}
	if (GetKeyState('A') & 0x8000)
	{
		KeyData |= KEY::AKEY;
	}
	if (GetKeyState('S') & 0x8000)
	{
		KeyData |= KEY::SKEY;
	}
	if (GetKeyState('D') & 0x8000)
	{
		KeyData |= KEY::DKEY;
	}
	if (GetKeyState('Z') & 0x8000)
	{
		KeyData |= KEY::ZKEY;
	}
	if (GetKeyState('X') & 0x8000)
	{
		KeyData |= KEY::XKEY;
	}
	if (GetKeyState('C') & 0x8000)
	{
		KeyData |= KEY::CKEY;
	}
	if (GetKeyState(VK_RSHIFT) & 0x8000)
	{
		KeyData |= KEY::RSHIFTKEY;
	}
	if (GetKeyState('M') & 0x8000)
	{
		KeyData |= KEY::MKEY;
	}
	if (GetKeyState(VK_LSHIFT) & 0x8000)
	{
		KeyData |= KEY::LSHIFTKEY;
	}

	// ----- �g���K�[���� -----
	m_Pad.normal = KeyData;
	m_Pad.alltrig = m_Pad.old ^ m_Pad.normal;
	m_Pad.trig = m_Pad.alltrig & m_Pad.normal;
	m_Pad.endtrig = m_Pad.alltrig & m_Pad.old;

}

// �֐����FPulseOn			�����オ�茟�o
// ���@���Fkey				�L�[�ԍ�
// �߂�l�F					����/���s
__forceinline bool CConsoleManager::PulseOn(const ULONGLONG key)const
{
	return (m_Pad.trig & key) ? true : false;
}

// �֐����FPulseOff			���������茟�o
// ���@���Fkey				�L�[�ԍ�
// �߂�l�F					����/���s
__forceinline bool CConsoleManager::PulseOff(const ULONGLONG key)const
{
	return (m_Pad.endtrig & key) ? true : false;
}

// �֐����FAllTrig			�펞���o
// ���@���Fkey				�L�[�ԍ�
// �߂�l�F					����/���s
__forceinline bool CConsoleManager::Press(const ULONGLONG key)const
{
	return (m_Pad.normal & key) ? true : false;
}

// �֐����FNoInput			���o�Ȃ�
// ���@���F
// �߂�l�F					����/���s
bool CConsoleManager::NoInput()const
{
	return (m_Pad.normal == 0) ? true : false;
}

// �֐����FSetColor			�w�i�F�A�����F�̐ݒ�
// ���@���F
// �߂�l�F
void CConsoleManager::SetColor(COLOR&& bg, COLOR&& txt) const
{
	::SetConsoleTextAttribute(m_Handle, UPShift(bg) | txt);
}

// �֐����FExchangeCursorVisible	�J�[�\���\���E��\���̐؂�ւ�
// ���@���F
// �߂�l�F
void CConsoleManager::ExchangeCursorVisible(const BOOL&& isVisible) const
{
	if (isVisible == TRUE)
	{
		::SetConsoleCursorInfo(m_Handle, &m_CsDefault); 
	}
	else
	{
		::SetConsoleCursorInfo(m_Handle, &m_CsInfo); 
	}
}

// �֐����FUPShift			�F�ԍ���w�i�F�p�Ƀr�b�g�V�t�g����
// ���@���Fcolor			�w�i�F
// �߂�l�Fcolor << 4		�A�b�v�V�t�g�����F�ԍ�
WORD CConsoleManager::UPShift(const WORD& color) const
{
	return color << 4;
}

// �֐����FSetCCursorInfo	�J�[�\�����̐ݒ�
// ���@���Finfo				�J�[�\�����
// �@�@�@�F
// �߂�l�F
void CConsoleManager::SetCCursorInfo(const CONSOLE_CURSOR_INFO& info) const
{
	::SetConsoleCursorInfo(m_Handle, &info);
}

// �֐����FSetScreenSize	�X�N���[���T�C�Y�ƃo�b�t�@�̎w��
// ���@���Fcoord			�T�C�Y(���A����)
// �@�@�@�FrctWindowRect	��`
// �߂�l�F
void CConsoleManager::SetScreenSize(const COORD& maxSize, const SMALL_RECT& rctWindowRect) const
{
	Sleep(40);
	::SetConsoleScreenBufferSize(m_Handle, maxSize);
	Sleep(40);
	::SetConsoleWindowInfo(m_Handle, TRUE, &rctWindowRect);
}

// �֐����FClearScreen		��ʂ̃N���A
// ���@���F
// �@�@�@�F
// �߂�l�F
void CConsoleManager::ClearScreen() const
{
	system("cls");
}

// �֐����FFill				��ʂ��w��̐F�Ŗ��߂�
// ���@���F
// �@�@�@�F
// �߂�l�F
void CConsoleManager::FillAll(COLOR bg, COLOR txt, CONSOLE_SCREEN_BUFFER_INFO* csbi) const
{
	COORD init = {0, 0};
	DWORD number = 0;

	for (SHORT i = 0; i < m_MaxBufferSize.Y; i++)
	{
		// �R���\�[���X�N���[���o�b�t�@�Ɋւ�������擾
		if (::GetConsoleScreenBufferInfo(m_Handle, csbi))
		{
			init.Y = i;
			// �o�b�t�@���̎w�肵�����W����w�肵�����̕����Z���������A�O�i�F�Ɣw�i�F��ݒ�
			::FillConsoleOutputAttribute(m_Handle, UPShift(bg) | txt, m_MaxBufferSize.X, init, &number);
			// �o�b�t�@���̎w�肵�����W����A�w�肵���������w�肵����������������
			::FillConsoleOutputCharacterA(m_Handle, ' ',  m_MaxBufferSize.X, init, &number);
		}
	}
}

// �֐����FSetCursorPos			�J�[�\���ʒu�̎w��
// ���@���Fcoord				�J�[�\���ʒu
// �@�@�@�F
// �߂�l�F
void CConsoleManager::SetCursorPos(const COORD& coord)
{
	::SetConsoleCursorPosition(m_Handle, coord);
	m_Position = coord;
}

// �֐����FSetCursorPos			�J�[�\���ʒu�̈ړ�
// ���@���F
// �@�@�@�F
// �߂�l�F
void CConsoleManager::SetCursorPos(const int& x, const int& y)
{
	COORD pos = {x, y};
	::SetConsoleCursorPosition(m_Handle, pos);
	m_Position = pos;
}

// �֐����FRefleshCursorInfo	�J�[�\���ʒu���̍X�V
// ���@���F
// �@�@�@�F
// �߂�l�F
void CConsoleManager::RefleshCursorInfo() const
{
	::SetConsoleCursorPosition(m_Handle, m_Position);
}

// �֐����FViewCursorInfo		�J�[�\���ʒu���̕\��
// ���@���F
// �@�@�@�F
// �߂�l�F
void CConsoleManager::ViewCursorInfo() const
{
	COORD zero = {0, 0};
	COORD nowPos = m_Position;
	::SetConsoleCursorPosition(m_Handle, zero);
	std::cout << "Cursor (X, Y) : (" << m_Position.X << ", " << m_Position.Y << ")";
	::SetConsoleCursorPosition(m_Handle, nowPos);
}

// �֐����FWriteTextCharacter		���O�̕\��(�������������s)
// ���@���Fbg				�w�i�F
// �@�@�@�Fpos				�����F
// �@�@�@�Fstr				������
// �@�@�@�FisWait			�\��������̑ҋ@�̗L��
// �@�@�@�FdelayTime		��������Ԃ̑ҋ@����(�[���I�ȕ������葬�x)
// �߂�l�F�Ȃ�
void CConsoleManager::WriteTextCharacter(COLOR bg, COLOR txt, COORD pos, const std::string& str, bool&& isWait, const int& delayTime) const
{
	if (str.size() != 0)
	{
		DWORD number = 0;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (::GetConsoleScreenBufferInfo(m_Handle, &csbi))
		{
			COORD _pos = pos;
			bool isDelay = true;
			for (unsigned int i = 0 ; i < str.size(); i++)
			{
				// �L�[���o
				Trigger();
				if (Press(KEY::RETURNKEY) == true)
				{
					isDelay = false;
				}

				// ����̕�����̂݃X�L�b�v
				bool isThrough = false;

				// �\���ʒu�̍X�V
				_pos.X = pos.X + i;

				// �����\��
				if (IsDBCSLeadByte(str[i]) == 0)
				{
					// �o�b�t�@���̎w�肵�����W����w�肵�����̕����Z���������A�O�i�F�Ɣw�i�F��ݒ�
					::FillConsoleOutputAttribute(m_Handle, UPShift(bg) | txt, 1, _pos, &number);
					// �o�b�t�@���̎w�肵�����W����A�������w�肵����������������
					// ���p�����̕\��
					::FillConsoleOutputCharacterA(m_Handle, str[i+0], 1, _pos, &number);
					if (str[i+0] == ' ')
					{
						isThrough = true;
					}
				}
				else
				{
					// �w��̃��C�h���������ϐ��Ɋi�[
					std::string character = str.substr(i, 2);

					// �T�C�Y�̎擾
					int cSize = character.length() + 1;

					// �������̊m��
					m_Wcs = new wchar_t[cSize];

					// �}���`�o�C�g�������烏�C�h�����ւ̕ϊ�
					size_t size;
					mbstowcs_s(&size, m_Wcs, cSize, character.c_str(), _TRUNCATE);
					
					// �o�b�t�@���̎w�肵�����W����w�肵�����̕����Z���������A�O�i�F�Ɣw�i�F��ݒ�
					::FillConsoleOutputAttribute(m_Handle, UPShift(bg) | txt, 2, _pos, &number);

					// �o�b�t�@���̎w�肵�����W����A���C�h�������w�肵����������������
					::FillConsoleOutputCharacterW(m_Handle, *m_Wcs, sizeof(m_Wcs[0]), _pos, &number);

					// �������̊J��
					delete [] m_Wcs;
					m_Wcs = nullptr;

					// ���̕�����̑����q�����C�h�����̕��̗]�蕪�����ǉ��ŃC���N�������g
					i++;
				}

				// �E�F�C�g
				if (isDelay == true && delayTime != DONOTDELAY && isThrough == false)
				{
					Sleep(delayTime);
				}
			}
		}
	}
	if (isWait == true)
	{
		for(;;)
		{
			Sleep(10);
			Trigger();
			if (PulseOn(KEY::RETURNKEY) == true)
			{
				break;
			}
		}
	}
}

// �֐����FCleanLog		���O����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
void CConsoleManager::CleanLog()
{
	m_ViewInfo.clear();
}

// �֐����FViewLog		���O�̕\��(�������������s)
// ���@���Fpos			�\������W
// �߂�l�F�Ȃ�
void CConsoleManager::ViewLog(COORD pos, COLOR bg, COLOR txt, int increment, int maxSize)
{
	// �C�e���[�^
	std::vector<std::string>::iterator iter = m_ViewInfo.begin();
	// �J�E���^
	int count = 0;
	// �w��s�������O�𑗂�
	for (int i = 0; i < increment; i++)
	{
		iter++;
	}
	// �������ʒu���烍�O��5�s�܂ŕ\��
	for (/* */ ; iter != m_ViewInfo.end(); /* */ )
	{
		if (m_ViewInfo.size() > static_cast<unsigned int>(maxSize))
		{
			if (count == 0 && increment != 0)
			{
				WriteTextCharacter(bg, txt, pos, (*iter) + "��", NOWAIT, DONOTDELAY);
			}
			else if (count == maxSize - 1 && static_cast<unsigned int>(increment + maxSize) < m_ViewInfo.size())
			{
				WriteTextCharacter(bg, txt, pos, (*iter) + "��", NOWAIT, DONOTDELAY);
			}
			else
			{
				WriteTextCharacter(bg, txt, pos, (*iter), NOWAIT, DONOTDELAY);
			}
		}
		else
		{
			WriteTextCharacter(bg, txt, pos, (*iter), NOWAIT, DONOTDELAY);
		}
		++iter;
		pos.Y++;
		if (++count >= maxSize)
		{
			break;
		}
	}
}

// �֐����FCleanLogRect		���O�G���A�̃t���b�V��
// ���@���Fpos				�\������W
// �߂�l�F�Ȃ�
void CConsoleManager::CleanLogRect(COORD pos, COLOR bg, COLOR txt, int& size) const
{
	std::string cleaner(m_RctWindowRect.Right, ' ');
	for (int i = 0; i < size; i++)
	{
		WriteTextCharacter(bg, txt, pos, cleaner, NOWAIT, DONOTDELAY);
		pos.Y++;
	}
}

// �֐����FCheckClear		�f�[�^�ǂݍ��ݎ��̉�ʃN���A�m�F
// ���@���FpNumEnter		���݂̉��s���ւ̃A�h���X
// �߂�l�F�Ȃ�
void CConsoleManager::CheckClear(int* pNumEnter)
{
	if ((*pNumEnter) >= m_RctWindowRect.Bottom)
	{
		(*pNumEnter) = 0;
		FillAll(L_PURPLE, H_WHITE, &m_Csbi);
		Trigger();
		if ( ! PulseOn(KEY::RETURNKEY))
		{
			Sleep(100);
		}
	}
}