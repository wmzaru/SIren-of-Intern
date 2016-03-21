#include "RPG.h"

#include <windows.h>
#include <vector>
#include <iterator>

// ゲームクラス
CRPG* pRPG;

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);

void Release();

auto WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpszCmdLine, int nCmdShow) -> int
{
	// コンソールを作成
	::AllocConsole();

	// ハンドラ登録
	::SetConsoleCtrlHandler(HandlerRoutine, TRUE);

	// ロケールの設定
	setlocale(LC_CTYPE, "");

	// ランダム
	srand(timeGetTime());

	// ゲームクラス
	pRPG = new CRPG();
	if (true == pRPG->CreateGameData())
	{
		pRPG->GameLoop();
	}

	// 開放
	Release();

	::FreeConsole();

	return 0;
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:      // Ctrl+Cを受け取った
    case CTRL_BREAK_EVENT:  // Ctrl+Breakを受け取った
    case CTRL_CLOSE_EVENT:  // コンソールを閉じた
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
		MessageBox(NULL, TEXT("FINISH THE GAME. \nCaused by Interrupt or Termination Process."), TEXT("LOG"), MB_OK);
		exit(0);
		//Release();
		return FALSE;
	}

	return FALSE;
}

// 開放
void Release()
{
	if (pRPG != nullptr)
	{
		delete pRPG;
		pRPG = nullptr;
	}
}