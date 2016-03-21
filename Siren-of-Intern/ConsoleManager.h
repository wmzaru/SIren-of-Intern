#ifndef __CWM_H__
#define __CWM_H__
#pragma once

// ---- Include File ----
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

// ---- �񋓑̐錾 ----
enum COLOR : WORD
{
	L_BLACK, L_BLUE, L_GREEN, L_CYAN, L_RED, L_PURPLE, L_YELLOW, L_WHITE,
	H_BLACK, H_BLUE, H_GREEN, H_CYAN, H_RED, H_PURPLE, H_YELLOW, H_WHITE,
};
enum DIRECTION : int { UP, DOWN, LEFT, RIGHT,  };
enum COLORNUMBER : int { COLOR0, COLOR1, COLOR2, };
enum ISWAIT : char { NOWAIT = false, WAIT = true };
enum ISDELAY : int { DONOTDELAY = -1 };

// ---- �\���̐錾 ----
typedef struct tagPadData
{
	ULONGLONG old;
	ULONGLONG normal;
	ULONGLONG alltrig;
	ULONGLONG trig;
	ULONGLONG endtrig;
}PAD;
namespace KEY
{
	const ULONGLONG		UPKEY		= 0x00000001;
	const ULONGLONG		DOWNKEY		= 0x00000002;
	const ULONGLONG		LEFTKEY		= 0x00000004;
	const ULONGLONG		RIGHTKEY	= 0x00000008;
	const ULONGLONG		SPACEKEY	= 0x00000010;
	const ULONGLONG		RETURNKEY	= 0x00000020;
	const ULONGLONG		QKEY		= 0x00000040;
	const ULONGLONG		WKEY		= 0x00000080;
	const ULONGLONG		EKEY		= 0x00000100;
	const ULONGLONG		AKEY		= 0x00000200;
	const ULONGLONG		SKEY		= 0x00000400;
	const ULONGLONG		DKEY		= 0x00000800;
	const ULONGLONG		ZKEY		= 0x00001000;
	const ULONGLONG		XKEY		= 0x00002000;
	const ULONGLONG		CKEY		= 0x00004000;
	const ULONGLONG		RSHIFTKEY	= 0x00008000;
	const ULONGLONG		MKEY		= 0x00010000;
	const ULONGLONG		LSHIFTKEY	= 0x00020000;
}


// ---- �N���X�錾 ----
class CConsoleManager
{
private:
	HANDLE						m_Handle;			// �E�B���h�E�n���h��
	CONSOLE_CURSOR_INFO			m_CsDefault;		// ���̃J�[�\����� (dwSize: 1�`100)
	CONSOLE_CURSOR_INFO			m_CsInfo;			// �ύX��̃J�[�\�����
	COORD						m_MaxBufferSize;	// �o�b�t�@�T�C�Y
	mutable COORD				m_Position;			// �J�[�\�����W
	SMALL_RECT					m_RctWindowRect;	// ��ʃT�C�Y (Bottom, Right���J�[�\���ő�ʒu)
	CONSOLE_SCREEN_BUFFER_INFO	m_Csbi;				// �o�b�t�@���
	mutable PAD					m_Pad;				// �g���K�[
	mutable wchar_t*			m_Wcs;				// ���C�h�����ϊ��p
	std::vector<std::string>	m_ViewInfo;			// �s����񃍃O

public:
	CConsoleManager();
	~CConsoleManager();

	void Initialize();
	void Trigger() const;
	bool PulseOn(const ULONGLONG)const;
	bool PulseOff(const ULONGLONG)const;
	bool Press(const ULONGLONG)const;
	bool NoInput()const;
	void SetColor(COLOR&&, COLOR&&) const;
	void ExchangeCursorVisible(const BOOL&&) const;
	WORD UPShift(const WORD&) const;
	void SetCCursorInfo(const CONSOLE_CURSOR_INFO&) const;
	void SetScreenSize(const COORD&, const SMALL_RECT&) const;
	void ClearScreen() const;
	void FillAll(COLOR, COLOR, CONSOLE_SCREEN_BUFFER_INFO*) const;
	void SetCursorPos(const COORD&);
	void SetCursorPos(const int&, const int&);
	void RefleshCursorInfo() const;
	void ViewCursorInfo() const;
	// �w��̈ʒu�ɕ������\��
	void WriteTextCharacter(COLOR, COLOR, COORD, const std::string&, bool&&, const int&) const;
	// ���O�\���֘A�@�\
	void CleanLog();
	void ViewLog(COORD, COLOR, COLOR, int, int);
	void CleanLogRect(COORD, COLOR, COLOR, int&) const;
	// �f�[�^�ǂݍ��ݎ��̉�ʃN���A�ƕ`��ʒu�̃��Z�b�g
	void CheckClear(int*);

	COORD GetCursorPos()		{ return m_Position;				}
	COORD* GetPCursorPos()		{ return &m_Position;				}
	CONSOLE_SCREEN_BUFFER_INFO* GetCSBInfo()
								{ return &m_Csbi;					}
	SMALL_RECT GetScreenSize()	{ return m_RctWindowRect;			}
	int GetLogSize()			{ return m_ViewInfo.size();			}
	void AddLog(std::string str){		 m_ViewInfo.push_back(str);	}
};

#endif