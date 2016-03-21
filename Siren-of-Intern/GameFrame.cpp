#include "RPG.h"

// コンストラクタ
CRPG::CRPG()
{
	Initialize();
	Create();
}

// デストラクタ
CRPG::~CRPG()
{
	Release();
}

// 関数名：Initialize			初期化
// 引　数：
// 戻り値：
void CRPG::Initialize()
{
	m_GameScene				= TITLE;
	m_EndFrag				= false;
	// マップ
	m_NowMapNumber			= 0;
	m_pMapFileName			= nullptr;
	m_pppMapData			= nullptr;
	m_TerrainSymbol[NONE]	= " ";
	m_TerrainSymbol[WALL]	= "*";
	m_TerrainSymbol[GROUND]	= "-";
	m_TerrainSymbol[STAIR]	= "S";
	// 自機
	m_pPlayer				= nullptr;
	// ログのサイズ(窓縦幅35、ヘルプ表示1行、4が最大)
	m_WindowLogSize			= 3;
	// リトライフラグ
	m_Retry					= false;
	// ゲーム背景色
	m_GameBGColor			= L_BLACK;
	m_GameTXTColor			= L_YELLOW;
	m_LogTXTColor			= H_GREEN;
	// ヘルプ表示(帯)
	m_Help[0]				= "<ACTION> MOVE:Q,W,E,A,D,Z,X,C WAIT:S ->QUICKLY:+LSHIFT  PICKUPITEM:RETURN <LOGSCROLL> UP,DOWN";
	m_Help[1]				= "<MENU> OPEN/CLOSE:SPACE USE/EQUIP:RETURN [SYMBOL] P:PLAYER S:STAIRtoNEXTFLOOR I:ITEM OTHER:ENEMY";
	// アイテム
	m_ItemBGColor			= L_YELLOW;
	m_ItemTXTColor			= H_GREEN;
	m_ItemSymbol			= "I";
	m_pItemList				= nullptr;

	// 敵データ
	m_NumEnemyType			= 0;
	m_pEnemyBookList		= nullptr;


}

// 関数名：Create				生成
// 引　数：
// 戻り値：
void CRPG::Create()
{
	// 自機クラス
	m_pPlayer = new CPlayer(25, 100, 15, 3, 1, 1, 30); // 最大HP、満腹度、初期経験値最大値、攻、防、行、カウント
	// 自機の初期化
	m_pPlayer->Initialize();
	// コンソールマネージャクラス
	m_pCCManager = new CConsoleManager();
	// 画面のクリア
	m_pCCManager->FillAll(L_PURPLE, H_WHITE, m_pCCManager->GetCSBInfo());
}

// 関数名：CreateGameData		ゲームデータを構成
// 引　数：
// 戻り値：
bool CRPG::CreateGameData()
{
	// カーソルを非表示
	m_pCCManager->ExchangeCursorVisible(FALSE);

	int x = 0, y = 0;
	m_pCCManager->SetCursorPos(x, y);

	// ==== マップデータ情報の読み込み ====
	if (false == CLoadData::GetLoadingInfo("Floor\\FloorStructure.txt", &m_NumMap, &m_pMapFileName, &y,&m_pCCManager, L_PURPLE, H_WHITE))
	{
		m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "フロア構成ファイルの展開に失敗。データの構成を見直してください。ENTERキーで終了します。", WAIT, DONOTDELAY);
		return false;
	}
	m_pCCManager->SetCursorPos(x,++y);

	SMALL_RECT rect = m_pCCManager->GetScreenSize();

	// マップサイズの決定
	m_Width = rect.Right;
	m_Height = rect.Bottom - m_WindowLogSize - 2 - 1; // ウィンドウ高さ35 - ログ3行 - ヘルプ2行 - マップ名階層表示1行
	// 階層名の表示位置
	m_MapNamePosition = MAKECOORD(0, m_Height);
	// ログ表示位置
	m_LogPosition = MAKECOORD(0, m_Height + 1);

	// マップデータ格納配列の生成
	if (m_NumMap == 1)
	{
		m_pppMapData = new char**();
	}
	else
	{
		m_pppMapData = new char**[m_NumMap];
	}
	for (int i = 0; i < m_NumMap; i++)
	{
		m_pppMapData[i] = new char*[m_Height];
		for (int j = 0; j < m_Height; j++)
		{
			m_pppMapData[i][j] = new char[m_Width];
		}
	}

	// ==== 各マップデータの読み込み ====
	for (int i= 0; i < m_NumMap; i++)
	{
		if ( false == CLoadData::LoadMapData(m_pMapFileName[i], &m_pppMapData[i], m_Width, m_Height, &y, &m_pCCManager, L_PURPLE, H_WHITE))
		{
			m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "マップデータの読み込みに失敗。ファイルデータの構成を見直してください。ENTERキーで終了します。", WAIT, DONOTDELAY);
			return false;
		}
		m_pCCManager->SetCursorPos(x,y);

		// トリガー検出
		m_pCCManager->Trigger();

		if ( ! m_pCCManager->Press(KEY::RETURNKEY))
		{
			Sleep(200);
		}
	}

	std::string str = "マップデータの読み込み完了。";
	m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), str, NOWAIT, DONOTDELAY);

	// トリガー検出
	m_pCCManager->Trigger();
	if ( ! m_pCCManager->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// ==== 敵データの読み込み ====
	if (false == CLoadData::LoadEnemyData("Data\\Enemy.txt", &m_NumEnemyType, &m_pEnemyBookList, &(++y), &m_pCCManager, L_PURPLE, H_WHITE))
	{
		m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "敵データファイルの展開に失敗。データの構成を見直してください。ENTERキーで終了します。", WAIT, DONOTDELAY);
		return false;
	}
	// トリガー検出
	m_pCCManager->Trigger();
	if ( ! m_pCCManager->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// ==== アイテムデータの読み込み ====
	if (false == CLoadData::LoadItemData("Data\\Item.txt", &m_NumAllItem, &m_pItemList, &y, &m_pCCManager, L_PURPLE, H_WHITE))
	{
		m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "アイテムデータファイルの展開に失敗。データの構成を見直してください。ENTERキーで終了します。", WAIT, DONOTDELAY);
		return false;
	}

	// 自機に図鑑データのアドレスをセット
	m_pPlayer->SetItemBook(&m_pItemList);

	// カーソルを表示
	m_pCCManager->ExchangeCursorVisible(TRUE);
	m_pCCManager->SetCursorPos(x + str.size(), y);
	m_pCCManager->WriteTextCharacter(L_PURPLE, H_WHITE, m_pCCManager->GetCursorPos(), "", WAIT, DONOTDELAY);

	return true;
}

// 関数名：Release		リソースの開放
// 引　数：
// 戻り値：
void CRPG::Release()
{
	// コンソールクラス
	if (m_pCCManager != nullptr)
	{
		delete m_pCCManager;
		m_pCCManager = nullptr;
	}
	// マップデータ
	if (m_pppMapData != nullptr)
	{
		for (int i = 0; i < m_NumMap; i++)
		{
			for (int j = 0; j < m_Height; j++)
			{
				delete [] m_pppMapData[i][j];
			}
			delete [] m_pppMapData[i];
		}
		if (m_NumMap == 1)
		{
			delete m_pppMapData;
		}
		else
		{
			delete [] m_pppMapData;
			m_pppMapData = nullptr;
		}
	}
	// 自機
	if (m_pPlayer != nullptr)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	// アイテム図鑑データ
	if (m_pItemList != nullptr)
	{
		if (m_NumAllItem == 1)
		{
			delete m_pPlayer;
			m_pItemList = nullptr;
		}
		else
		{
			delete [] m_pPlayer;
			m_pItemList = nullptr;
		}
	}

	// 敵データ
	if (m_pEnemyBookList != nullptr)
	{
		if (m_NumEnemyType == 1)
		{
			delete m_pEnemyBookList;
		}
		else
		{
			delete [] m_pEnemyBookList;
		}
		m_pEnemyBookList = nullptr;
	}
}

// 関数名：GameLoop		ゲームループ処理
// 引　数：
// 戻り値：
void CRPG::GameLoop()
{
	//enum GAMESCENE : char { LOAD, SAVE, TITLE, CONTINUE, DUNGEON, CITY, DEATH, RESULT, CREDIT, };
	for (;;)
	{
		switch (m_GameScene)
		{
		case TITLE:
			Title();
			break;
		case CONTINUE:
			m_GameScene = TITLE;
			break;
		case DUNGEON:
 			Dungeon();
			break;
		case CITY:
			break;
		case DEATH:
			GameOver();
			break;
		case RESULT:
			break;
		case CREDIT:
			Sleep(100);
			CreditRoll();
			break;
		case SAVE:
			break;
		}

		if (m_EndFrag == true)
		{
			// 終了
			break;
		}
	}
}
