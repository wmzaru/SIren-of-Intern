#include "RPG.h"

#include <windows.h>
#include <vector>
#include <iterator>

// �Q�[���N���X
CRPG* pRPG;

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);

void Release();

auto WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpszCmdLine, int nCmdShow) -> int
{
	// �R���\�[�����쐬
	::AllocConsole();

	// �n���h���o�^
	::SetConsoleCtrlHandler(HandlerRoutine, TRUE);

	// ���P�[���̐ݒ�
	setlocale(LC_CTYPE, "");

	// �����_��
	srand(timeGetTime());

	// �Q�[���N���X
	pRPG = new CRPG();
	if (true == pRPG->CreateGameData())
	{
		pRPG->GameLoop();
	}

	// �J��
	Release();

	::FreeConsole();

	return 0;
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:      // Ctrl+C���󂯎����
    case CTRL_BREAK_EVENT:  // Ctrl+Break���󂯎����
    case CTRL_CLOSE_EVENT:  // �R���\�[�������
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
		MessageBox(NULL, TEXT("FINISH THE GAME. \nCaused by Interrupt or Termination Process."), TEXT("LOG"), MB_OK);
		exit(0);
		//Release();
		return FALSE;
	}

	return FALSE;
}

// �J��
void Release()
{
	if (pRPG != nullptr)
	{
		delete pRPG;
		pRPG = nullptr;
	}
}