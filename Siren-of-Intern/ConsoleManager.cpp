#include "ConsoleManager.h"

// コンストラクタ
CConsoleManager::CConsoleManager() : m_Wcs(nullptr)
{
	Initialize();
}

// デストラクタ
CConsoleManager::~CConsoleManager()
{
	CloseHandle(m_Handle);
	if (m_Wcs != nullptr)
	{
		delete [] m_Wcs;
	}
}

// 関数名：Initialize		初期化
// 引　数：
// 戻り値：
void CConsoleManager::Initialize()
{
	// パッドデータの初期化
	m_Pad.normal			= 0;
	m_Pad.alltrig			= 0;
	m_Pad.endtrig			= 0;
	m_Pad.old				= 0;
	m_Pad.trig				= 0;
	m_CsDefault.dwSize		= 100; // 1 ～ 100
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

	// 色の設定
	SetColor(L_PURPLE, H_WHITE);

	// フォントサイズの設定
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	::GetCurrentConsoleFontEx(m_Handle, 0, &info);
	// ※ default = (8, 18), max = (12, 27), min = (1, 1)
	info.dwFontSize.X = 12;
	info.dwFontSize.Y = 27;
	::SetCurrentConsoleFontEx(m_Handle, 0, &info);

	// バッファサイズの設定
	SetScreenSize(m_MaxBufferSize, m_RctWindowRect);

	Sleep(0);

	// タイトルの設定
	::SetConsoleTitle("20160107 - 0122 Intern RPG ROGUELIKE GAME made by A.I.");

	// カーソル情報の設定
	::SetConsoleCursorInfo(m_Handle, &m_CsDefault);

}

// 関数名：Trigger			トリガー検出
// 引　数：
// 戻り値：
__forceinline void CConsoleManager::Trigger() const
{
	Sleep(0);

	m_Pad.old = m_Pad.normal;	// 保存

	// 終了
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		exit(0);
	}

	// ----- キー入力処理 -----
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

	// ----- トリガー処理 -----
	m_Pad.normal = KeyData;
	m_Pad.alltrig = m_Pad.old ^ m_Pad.normal;
	m_Pad.trig = m_Pad.alltrig & m_Pad.normal;
	m_Pad.endtrig = m_Pad.alltrig & m_Pad.old;

}

// 関数名：PulseOn			立ち上がり検出
// 引　数：key				キー番号
// 戻り値：					成功/失敗
__forceinline bool CConsoleManager::PulseOn(const ULONGLONG key)const
{
	return (m_Pad.trig & key) ? true : false;
}

// 関数名：PulseOff			立ち下がり検出
// 引　数：key				キー番号
// 戻り値：					成功/失敗
__forceinline bool CConsoleManager::PulseOff(const ULONGLONG key)const
{
	return (m_Pad.endtrig & key) ? true : false;
}

// 関数名：AllTrig			常時検出
// 引　数：key				キー番号
// 戻り値：					成功/失敗
__forceinline bool CConsoleManager::Press(const ULONGLONG key)const
{
	return (m_Pad.normal & key) ? true : false;
}

// 関数名：NoInput			検出なし
// 引　数：
// 戻り値：					成功/失敗
bool CConsoleManager::NoInput()const
{
	return (m_Pad.normal == 0) ? true : false;
}

// 関数名：SetColor			背景色、文字色の設定
// 引　数：
// 戻り値：
void CConsoleManager::SetColor(COLOR&& bg, COLOR&& txt) const
{
	::SetConsoleTextAttribute(m_Handle, UPShift(bg) | txt);
}

// 関数名：ExchangeCursorVisible	カーソル表示・非表示の切り替え
// 引　数：
// 戻り値：
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

// 関数名：UPShift			色番号を背景色用にビットシフトする
// 引　数：color			背景色
// 戻り値：color << 4		アップシフトした色番号
WORD CConsoleManager::UPShift(const WORD& color) const
{
	return color << 4;
}

// 関数名：SetCCursorInfo	カーソル情報の設定
// 引　数：info				カーソル情報
// 　　　：
// 戻り値：
void CConsoleManager::SetCCursorInfo(const CONSOLE_CURSOR_INFO& info) const
{
	::SetConsoleCursorInfo(m_Handle, &info);
}

// 関数名：SetScreenSize	スクリーンサイズとバッファの指定
// 引　数：coord			サイズ(幅、高さ)
// 　　　：rctWindowRect	矩形
// 戻り値：
void CConsoleManager::SetScreenSize(const COORD& maxSize, const SMALL_RECT& rctWindowRect) const
{
	Sleep(40);
	::SetConsoleScreenBufferSize(m_Handle, maxSize);
	Sleep(40);
	::SetConsoleWindowInfo(m_Handle, TRUE, &rctWindowRect);
}

// 関数名：ClearScreen		画面のクリア
// 引　数：
// 　　　：
// 戻り値：
void CConsoleManager::ClearScreen() const
{
	system("cls");
}

// 関数名：Fill				画面を指定の色で埋める
// 引　数：
// 　　　：
// 戻り値：
void CConsoleManager::FillAll(COLOR bg, COLOR txt, CONSOLE_SCREEN_BUFFER_INFO* csbi) const
{
	COORD init = {0, 0};
	DWORD number = 0;

	for (SHORT i = 0; i < m_MaxBufferSize.Y; i++)
	{
		// コンソールスクリーンバッファに関する情報を取得
		if (::GetConsoleScreenBufferInfo(m_Handle, csbi))
		{
			init.Y = i;
			// バッファ内の指定した座標から指定した数の文字セル分だけ、前景色と背景色を設定
			::FillConsoleOutputAttribute(m_Handle, UPShift(bg) | txt, m_MaxBufferSize.X, init, &number);
			// バッファ内の指定した座標から、指定した文字を指定した数だけ書き込む
			::FillConsoleOutputCharacterA(m_Handle, ' ',  m_MaxBufferSize.X, init, &number);
		}
	}
}

// 関数名：SetCursorPos			カーソル位置の指定
// 引　数：coord				カーソル位置
// 　　　：
// 戻り値：
void CConsoleManager::SetCursorPos(const COORD& coord)
{
	::SetConsoleCursorPosition(m_Handle, coord);
	m_Position = coord;
}

// 関数名：SetCursorPos			カーソル位置の移動
// 引　数：
// 　　　：
// 戻り値：
void CConsoleManager::SetCursorPos(const int& x, const int& y)
{
	COORD pos = {x, y};
	::SetConsoleCursorPosition(m_Handle, pos);
	m_Position = pos;
}

// 関数名：RefleshCursorInfo	カーソル位置情報の更新
// 引　数：
// 　　　：
// 戻り値：
void CConsoleManager::RefleshCursorInfo() const
{
	::SetConsoleCursorPosition(m_Handle, m_Position);
}

// 関数名：ViewCursorInfo		カーソル位置情報の表示
// 引　数：
// 　　　：
// 戻り値：
void CConsoleManager::ViewCursorInfo() const
{
	COORD zero = {0, 0};
	COORD nowPos = m_Position;
	::SetConsoleCursorPosition(m_Handle, zero);
	std::cout << "Cursor (X, Y) : (" << m_Position.X << ", " << m_Position.Y << ")";
	::SetConsoleCursorPosition(m_Handle, nowPos);
}

// 関数名：WriteTextCharacter		ログの表示(消去も同時実行)
// 引　数：bg				背景色
// 　　　：pos				文字色
// 　　　：str				文字列
// 　　　：isWait			表示完了後の待機の有無
// 　　　：delayTime		文字送り間の待機時間(擬似的な文字送り速度)
// 戻り値：なし
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
				// キー検出
				Trigger();
				if (Press(KEY::RETURNKEY) == true)
				{
					isDelay = false;
				}

				// 特定の文字列のみスキップ
				bool isThrough = false;

				// 表示位置の更新
				_pos.X = pos.X + i;

				// 文字表示
				if (IsDBCSLeadByte(str[i]) == 0)
				{
					// バッファ内の指定した座標から指定した数の文字セル分だけ、前景色と背景色を設定
					::FillConsoleOutputAttribute(m_Handle, UPShift(bg) | txt, 1, _pos, &number);
					// バッファ内の指定した座標から、文字を指定した数だけ書き込む
					// 半角文字の表示
					::FillConsoleOutputCharacterA(m_Handle, str[i+0], 1, _pos, &number);
					if (str[i+0] == ' ')
					{
						isThrough = true;
					}
				}
				else
				{
					// 指定のワイド文字だけ変数に格納
					std::string character = str.substr(i, 2);

					// サイズの取得
					int cSize = character.length() + 1;

					// メモリの確保
					m_Wcs = new wchar_t[cSize];

					// マルチバイト文字からワイド文字への変換
					size_t size;
					mbstowcs_s(&size, m_Wcs, cSize, character.c_str(), _TRUNCATE);
					
					// バッファ内の指定した座標から指定した数の文字セル分だけ、前景色と背景色を設定
					::FillConsoleOutputAttribute(m_Handle, UPShift(bg) | txt, 2, _pos, &number);

					// バッファ内の指定した座標から、ワイド文字を指定した数だけ書き込む
					::FillConsoleOutputCharacterW(m_Handle, *m_Wcs, sizeof(m_Wcs[0]), _pos, &number);

					// メモリの開放
					delete [] m_Wcs;
					m_Wcs = nullptr;

					// 元の文字列の走査子をワイド文字の幅の余剰分だけ追加でインクリメント
					i++;
				}

				// ウェイト
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

// 関数名：CleanLog		ログ消去
// 引　数：なし
// 戻り値：なし
void CConsoleManager::CleanLog()
{
	m_ViewInfo.clear();
}

// 関数名：ViewLog		ログの表示(消去も同時実行)
// 引　数：pos			表示基準座標
// 戻り値：なし
void CConsoleManager::ViewLog(COORD pos, COLOR bg, COLOR txt, int increment, int maxSize)
{
	// イテレータ
	std::vector<std::string>::iterator iter = m_ViewInfo.begin();
	// カウンタ
	int count = 0;
	// 指定行だけログを送る
	for (int i = 0; i < increment; i++)
	{
		iter++;
	}
	// 送った位置からログを5行まで表示
	for (/* */ ; iter != m_ViewInfo.end(); /* */ )
	{
		if (m_ViewInfo.size() > static_cast<unsigned int>(maxSize))
		{
			if (count == 0 && increment != 0)
			{
				WriteTextCharacter(bg, txt, pos, (*iter) + "↑", NOWAIT, DONOTDELAY);
			}
			else if (count == maxSize - 1 && static_cast<unsigned int>(increment + maxSize) < m_ViewInfo.size())
			{
				WriteTextCharacter(bg, txt, pos, (*iter) + "↓", NOWAIT, DONOTDELAY);
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

// 関数名：CleanLogRect		ログエリアのフラッシュ
// 引　数：pos				表示基準座標
// 戻り値：なし
void CConsoleManager::CleanLogRect(COORD pos, COLOR bg, COLOR txt, int& size) const
{
	std::string cleaner(m_RctWindowRect.Right, ' ');
	for (int i = 0; i < size; i++)
	{
		WriteTextCharacter(bg, txt, pos, cleaner, NOWAIT, DONOTDELAY);
		pos.Y++;
	}
}

// 関数名：CheckClear		データ読み込み時の画面クリア確認
// 引　数：pNumEnter		現在の改行数へのアドレス
// 戻り値：なし
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