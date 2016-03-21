#include "RPG.h"

// 関数名：Title			タイトル
// 引　数：
// 戻り値：
void CRPG::Title()
{
	// カーソルの非表示
	m_pCCManager->ExchangeCursorVisible(FALSE);

	// 画面のクリア
	m_pCCManager->FillAll(L_BLUE, H_YELLOW, m_pCCManager->GetCSBInfo());

	// 自機パラメータのリセット
	m_pPlayer->ResetParameter();

	// ==== オープニングの表示 ====
	COORD pos = {0, 0};
	COORD opPos = {20, 4};
	const int opHeight = 16;

	// 文字送りの速度
	int charaPopSpeed = 70;

	// 行送りを早める行数の指定
	int slowViewRowNumber = 7;
	std::string openingText[opHeight] =
	{
		"極寒の大地サムイドウのサッポコ共和国。その国の地下深くには　",
		"チカホとよばれる広大なダンジョンがあった。　　　　　　　　　",
		"ダンジョンの最奥には目も眩むような金銀財宝が眠っていると　　",
		"言い伝えられている・・・・・・　　　　　　　　　　　　　　　",
		"今ここに、そのダンジョンに挑もうとする若者が一人。　　　　　",
		"　　　　　　　  　　　　　　　　　　　　　　　　　　　　　　",
		"彼の名は・・・・・・　　　　　　　　　　　　　　　　　　　　",
		"　　　　　　　  　　　　　　　　　　　　　　　　　　　　　　",
		"               _人_人_人_人_人_人_人_人_                    ",
		"              ＞　インターンのシレン　 ＜                   ",
		"               ^Ｙ^Ｙ^Ｙ^Ｙ^Ｙ^Ｙ^Ｙ^Ｙ^                    ",
		"                                                            ",
		"                                            Press Enter Key "
	};

	// 再挑戦時の表示変更
	if (m_Retry == true)
	{
		charaPopSpeed = 5;
		slowViewRowNumber = opHeight;
		openingText[ 0] = "ざわ...　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　　";
		openingText[ 1] = "　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　 　　　　";
		openingText[ 2] = "ざわ...　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　　";
		openingText[ 3] = "　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　 　　　　";
		openingText[ 4] = "ざわ...　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　　";
		openingText[ 5] = "　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　 　　　　";
		openingText[ 6] = "                                                            ";
		openingText[ 7] = "                  あの手はまさか・・・！                    ";
		openingText[ 8] = "                                                            ";
		openingText[ 9] = "　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　 　　　　";
		openingText[10] = "ざわ...　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　　";
		openingText[11] = "　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　 　　　　";
		openingText[12] = "ざわ...　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　　";
		openingText[13] = "　　　　ざわ...　　　　ざわ...　　　　ざわ...　　　 　　　　";
		openingText[14] = "ざわ...　　　　ざわ...　　　　　　　　　　　　　　　　　　　";
		openingText[15] = "                      Press Enter Key  -> Confirm Result    ";
	}

	// ==== オープニングテキストの表示 ====
	// クレジットのロールアップ時間決定用
	float oneCycleDelay = 2.0f;
	if (m_Retry == true)
	{
		oneCycleDelay = 0.01f;
	}
	DWORD startTime = timeGetTime();
	DWORD nowTime;
	float elapsedTime;

	// カウンタ
	int count = 0;
	int countEnd = opHeight;
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
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, openingText[count], NOWAIT, charaPopSpeed);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, openingText[count], NOWAIT, DONOTDELAY);
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

	// ==== タイトルの表示 ====
	const int tiHeight = 16;
	std::string titleLogo[tiHeight] =
	{
		"　　┏┓┏━┓┏┓┏━━┓　 　 　  ┏━┓┏┓　　　┏━┓┏┓┏┓　 　 ┏━┓┏┓",
		"　┏┛┃┗━┛┃┃┃┏┓┃　 　 　　┗━┛┃┃　　　┗━┛┃┃┃┃　 　 ┗━┛┃┃",
		"┏┛┏┛　 　 ┃┃┃┗┛┃┏━━━┓　 　 ┃┃　　　┏━┓┃┃┃┃　┏┓　 　 ┃┃",
		"┗┫┃　 　 ┏┛┃┃┏┓┃┗━━━┛ 　 ┏┛┃　　　┗━┛┃┃┃┃┏┛┃ 　 ┏┛┃",
		"　┃┃　┏━┛┏┛┗┛┃┃　 　 　  ┏━┛┏┛　　　┏━━┛┃┃┗┛┏┛┏━┛┏┛",
		"　┗┛　┗━━┛　 　 ┗┛　 　   　┗━━┛　　の　┗━━━┛┗━━┛　┗━━┛　 "
	};

	// ロゴ表示高さ
	int height = 6;

	// 再挑戦時にロゴを変更
	if (m_Retry == true)
	{
		height = tiHeight;
		titleLogo[ 0] = "　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 ";
		titleLogo[ 1] = "　 ／ ￣/　―　 /　＿|＿_　＿__|＿　　　　　　　　　　　　　　　　　　　　　　　";
		titleLogo[ 2] = "　　 ―/　　　 / 　 ｜ ／ 　　/|　　　　　　　　　　　　　　　　　　　　　　　  ";
		titleLogo[ 3] = " 　 ＿/　　＿／　 　 |　　＿／｜　　　　　　　　　　　　　　　　　　　　　　　　";
		titleLogo[ 4] = "　　　　　　　　　　　　　　　　　　　　　　　　　　 /＼＿＿＿＿_／ヽ　　　　　　";
		titleLogo[ 5] = "　　 (.｀ヽ(｀＞ 、　　　　　　　　　　　　　　　　／''''　　　''''::＼　　　　　";
		titleLogo[ 6] = "　 　 `'＜｀ゝr'ﾌ＼　　　　　　　　　　　 　 ＋　　|（●）,　 ､（●）､:|　＋　　 ";
		titleLogo[ 7] = " ⊂ｺ二Lﾌ^´　 ノ, ／⌒)　　　　　　　　 　 　 　　 |　,,,ﾉ(､_,)ヽ､,,　:|　　　　 ";
		titleLogo[ 8] = "　⊂l二L7_　/　-ゝ-')´　　　　　　　　 　　　　+　|　　｀-=ﾆ=- '   :::|　+ 　　 ";
		titleLogo[ 9] = "　　　　　 ＼_　　､__,.ｲ＼　　　　　　　　＋　　　　＼　　｀ﾆﾆ´　 ::／　　　　+ ";
		titleLogo[10] = "　　　　　　　(T＿_ノ　　 Tヽ　　　　, -r'⌒!￣ ` :::7ヽ｀- ､　　　./|　　　　　 ";
		titleLogo[11] = "　　　　　　　　ヽ￢.　　　/ﾉ`ｰ-､ﾍ<ｰ1´|　ヽ　| :::::ﾄ、＼　(　　./　ヽ　　　　　";
		titleLogo[12] = "　　　　　　　　　＼l__,.／　i　l ヽ!　|　　　| :::::l ヽ ｀７ｰ.､‐'´|＼-､　　　";
		titleLogo[13] = " ＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿  ＿＿　　　　　　　　　　";
		titleLogo[14] = "│二│三│四│二│三│四│二│三│四│◎│◎│◎│伍│｜伍｜　　　　　　　　　　 ";
		titleLogo[15] = "│萬│萬│萬│萬│萬│萬│萬│萬│萬│◎│◎│◎│萬│｜萬｜　※同じゲームです　 ";
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

	// タイトルロゴの表示
	for (int i = 0; i < height; i++)
	{
		pos.X = startPos.X;
		pos.Y = startPos.Y + i;
		m_pCCManager->SetCursorPos(pos);
		m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, titleLogo[i], NOWAIT, DONOTDELAY);
	}
	// 再表示フラグ
	m_Retry = true;

	// 項目の表示
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
	std::string cursor = "＠";
	std::string space = "  ";
	// 初期選択項目にカーソルを表示
	m_pCCManager->SetCursorPos(sLogoPos);
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, sLogoPos, cursor, NOWAIT, DONOTDELAY);
	m_pCCManager->SetCursorPos(sLogoPos);
	for(;;)
	{
		// キー入力検出
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
			// 選択項目のカーソル表示と非選択項目のカーソル跡塗りつぶし消去
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
		//m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, "セーブデータを探します。", NOWAIT, DONOTDELAY);
		//m_GameScene = CONTINUE;
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, "現在未実装です。本編をはじめます", WAIT, DONOTDELAY);
		m_GameScene = DUNGEON;
		break;
	case _EXIT:
		nowPosition.X = 0;
		nowPosition.Y = 1;
		m_pCCManager->SetCursorPos(nowPosition);
		m_EndFrag = true;
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, "ゲームを終了します。ENTERKEYを押してください。", WAIT, DONOTDELAY);
		break;
	}
}
