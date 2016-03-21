#include "RPG.h"

// 関数名：CreditRoll			クレジットシーン
// 引　数：
// 戻り値：
void CRPG::CreditRoll()
{
	// バックグラウンド色指定
	COLOR bBack = L_BLACK;
	COLOR bText = H_WHITE;
	// クレジット背景、文字色指定
	COLOR cBack = L_BLACK;
	COLOR cText = H_GREEN;
	// スキップ表示背景、文字色指定
	COLOR sBack = L_BLACK;
	COLOR sText = H_YELLOW;

	// カーソルの非表示
	m_pCCManager->ExchangeCursorVisible(FALSE);

	// 画面のクリア
	m_pCCManager->FillAll(bBack, bText, m_pCCManager->GetCSBInfo());

	// クレジットのロールアップ時間決定用
	float oneCycleDelay = 0.5f;
	DWORD startTime = timeGetTime();
	DWORD nowTime;
	float elapsedTime;

	// 行表示間の遅延
	int onRowDelay = 100;

	// 文字送りの速度
	int charaPopSpeed = 70;

	// カウンタ
	int count = 0;
	int rollHeight = m_Height;
	int countEnd = rollHeight;
	const int numRows = 12;

	// クレジット
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

	// 初期選択項目にカーソルを表示
	m_pCCManager->SetCursorPos(startPos);
	bool isComplete = false;
	for(;;)
	{
		Sleep(1);
		// キー入力検出
		m_pCCManager->Trigger();
		// ロールスキップ
		if (m_pCCManager->Press(KEY::RETURNKEY))
		{
			if (isComplete == false)
			{
				count = countEnd - 1;
			}
		}

		if (isComplete == false)
		{
			// タイマーカウント
			nowTime = timeGetTime();
			elapsedTime = static_cast<float>(nowTime - startTime) / 1000.0f;
			if (elapsedTime >= oneCycleDelay)
			{
				// タイマーリセット
				startTime = timeGetTime();

				// 最後の一回だけ画面をフラッシュ
				if (count >= countEnd - 1)
				{
					m_pCCManager->FillAll(bBack, bText, m_pCCManager->GetCSBInfo());
				}

				// 指定範囲にクレジットの表示
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
					// クレジット(1列)
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
				// クレジットのごみをフラッシュ
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
				// カウントアップ
				count++;
				// 終了判定
				if (count >= countEnd)
				{
					m_GameScene = TITLE;
					isComplete = true;;
				}
			}
			// スキップ表示
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
		// 終了 (タイトルバック)
		if (m_pCCManager->PulseOn(KEY::RETURNKEY))
		{
			m_GameScene = TITLE;
			break;
		}
	}
}
