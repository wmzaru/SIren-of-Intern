#include "RPG.h"

// 関数名：GameOver		ゲームオーバーシーン
// 引　数：
// 戻り値：
void CRPG::GameOver()
{
	// カーソルの非表示
	m_pCCManager->ExchangeCursorVisible(FALSE);

	// 画面のクリア
	m_pCCManager->FillAll(L_BLUE, H_YELLOW, m_pCCManager->GetCSBInfo());

	// 自機の初期化
	m_pPlayer->Initialize();

	// ==== オープニングの表示 ====
	COORD pos = {0, 0};
	COORD opPos = {20, 15};
	const int gameOverHeight = 2;

	// 文字送りの速度
	int charaPopSpeed = 150;

	// 行送りを早める行数の指定
	int slowViewRowNumber = 1;
	std::string gameOverText[gameOverHeight] =
	{
		"その後シュウカツ(ダンジョン攻略)中の彼の姿を見たものはいない",
		"                                            Press Enter Key "
	};

	// ==== ゲームオーバーテキストの表示 ====
	// クレジットのロールアップ時間決定用
	float oneCycleDelay = 2.0f;
	DWORD startTime = timeGetTime();
	DWORD nowTime;
	float elapsedTime;

	// カウンタ
	int count = 0;
	int countEnd = gameOverHeight;
	pos = opPos;
	
	Sleep(200);
	for(;;)
	{
		Sleep(1);
		// キー入力検出
		m_pCCManager->Trigger();
		// タイマーカウント
		nowTime = timeGetTime();
		elapsedTime = static_cast<float>(nowTime - startTime) / 1000.0f;
		if (elapsedTime >= oneCycleDelay || m_pCCManager->Press(KEY::RETURNKEY))
		{
			// タイマーリセット
			startTime = timeGetTime();
			// 表示
			if (count < slowViewRowNumber)
			{
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, gameOverText[count], NOWAIT, charaPopSpeed);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, gameOverText[count], NOWAIT, DONOTDELAY);
				oneCycleDelay = 0.5f;
			}
			pos.Y++;
			// 終了判定
			if (++count >= countEnd)
			{
				break;
			}
		}
	}
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, "", WAIT, DONOTDELAY);

	// タイトルバック
	m_GameScene = TITLE;
}
