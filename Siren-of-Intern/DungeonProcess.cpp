#include "RPG.h"

// 関数名：GetGroundData	マップデータから地面データを抽出
// 引　数：
// 戻り値：
std::multimap<char, char> CRPG::ViewMapAndGetGround()
{
	COORD pos = {0, 0};
	std::multimap<char, char> ground;
	std::string symbol;
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			pos = MAKECOORD(j, i);
			m_pCCManager->SetCursorPos(pos);
			char str = m_pppMapData[m_NowMapNumber][i][j];
			//enum MAP : char { NONE, WALL, GROUND, STAIR, };
			switch (str)
			{
			case NONE:
				symbol = m_TerrainSymbol[NONE];
				break;
			case WALL:
				symbol = m_TerrainSymbol[WALL];
				break;
			case GROUND:
				symbol = m_TerrainSymbol[GROUND];
				ground.insert(std::make_pair(j, i));
				break;
			case STAIR:
				symbol = m_TerrainSymbol[STAIR];
				break;
			}
			if (symbol == m_TerrainSymbol[STAIR])
			{
				m_pCCManager->WriteTextCharacter(H_WHITE, L_BLACK, pos, symbol, NOWAIT, DONOTDELAY);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, pos, symbol, NOWAIT, DONOTDELAY);
			}
		}
	}
	return ground;
}

// 関数名：SetEnemyData		インデックス番号で呼び出した敵データを参照
// 引　数：index			インデックス番号
// 戻り値：
void CRPG::SetEnemyData(int index)
{
	// 図鑑から敵サンプルへデータを登録
	m_Sample.SetBookData(m_pEnemyBookList[index]);

	// サンプルの初期化 (体力の現在値など)
	m_Sample.Initialize();

	// 乱数で所持アイテムを決定(仮)
	int seed = rand() % 20;
	bool flag = false;
	if (seed == 0)
	{
		flag = true;
	}

	int dropItemIndex;// ドロップアイテム番号
	dropItemIndex = rand() % m_NumAllItem;

	m_Sample.SetItemHaving(flag, dropItemIndex);
}

// 関数名：ResetEnemy			登録した敵の消去(コンテナの全要素削除->初期化)
// 引　数：
// 戻り値：
void CRPG::ResetEnemy()
{
	m_Enemy.clear();
}

// 関数名：SetItemData		インデックス番号で呼び出したアイテムデータを参照
// 引　数：index			インデックス番号
// 戻り値：
void CRPG::SetItemData(int index)
{

}

// 関数名：ResetItem			登録したアイテムの消去(コンテナの全要素削除->初期化)
// 引　数：
// 戻り値：
void CRPG::ResetItem()
{
	m_FallItemList.clear();
}

// 関数名：LogScroll		ログスクロール
// 引　数：pScrollCount		スクロール量へのアドレス
// 　　　：pNumScroll		最大スクロール可能量へのアドレス
// 戻り値：
bool CRPG::LogScroll(int* pScrollCount, int* pNumScroll)
{
	int logSize = m_pCCManager->GetLogSize();
	if (m_pCCManager->GetLogSize() <= m_WindowLogSize)
	{
		return false;
	}

	// 最大スクロール量の更新
	(*pNumScroll) = logSize - m_WindowLogSize;

	// フラグ
	bool isRefresh = false;

	// ログを上にスクロール
	if (m_pCCManager->PulseOn(KEY::UPKEY))
	{
		if ((*pScrollCount) > 0)
		{
			isRefresh = true;
			(*pScrollCount)--;
		}
	}
	// ログを下にスクロール
	if (m_pCCManager->PulseOn(KEY::DOWNKEY))
	{
		if ((*pScrollCount) < (*pNumScroll))
		{
			isRefresh = true;
			(*pScrollCount)++;
		}
	}
	if (isRefresh == true)
	{
		// スクロールした情報ログの再表示
		m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);
		m_pCCManager->ViewLog(m_LogPosition, m_GameBGColor, m_LogTXTColor,(*pScrollCount), m_WindowLogSize);
		Sleep(10);
	}
	return isRefresh;
}

// 関数名：Menu				メニュー表示
// 引　数：pIsOpen			メニューを開いているかどうか
// 　　　：pScrollCount		スクロール量へのアドレス
// 戻り値：
bool CRPG::Menu(bool* pIsOpen, bool* pItemUse, int* pScrollCount)
{
	const int viewHeight = 3; // 行動ログの高さ
	char buffer[256];
	std::string selectCursor = "← [USE:ENTER / BACK:SPACE]";
	// アイテムデータ
	ItemList& iList = m_pPlayer->GetItem();
	ItemList::iterator itemIter;

	// 非表示の時にSPACEボタンで開く
	if (m_pCCManager->PulseOn(KEY::SPACEKEY))
	{
		// フラグが立っていない時はメニューモードに入る
		if ((*pIsOpen) == false)
		{
			(*pIsOpen) = true;

			// 表示領域のクリア
			m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

			COORD pos = m_LogPosition;
			// スクロールしたメニューの再表示
			int count = 0;
			for (itemIter = iList.begin(); itemIter != iList.end(); itemIter++)
			{
				std::string name = m_pItemList[(*itemIter).first].name + ":";
				::_itoa_s((*itemIter).second, buffer, sizeof(char) * 256, 10);
				name += buffer;
				if (count == 0)
				{
					name += selectCursor;
				}
				// アイテム一覧の表示
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_LogTXTColor, pos, name, NOWAIT, DONOTDELAY);
				pos.Y++;
				if (++count == viewHeight)
				{
					break;
				}
			}
			// アイテムが何もないとき
			if (iList.size() == 0)
			{
				std::string str = "何ももっていない。 [BACK:SPACE]";
				// アイテム一覧の表示
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_LogTXTColor, pos, str, NOWAIT, DONOTDELAY);
			}
		}
		// フラグが立っているときはメニューモードから抜ける
		else
		{
			(*pIsOpen) = false;

			// スクロール量のリセット
			(*pScrollCount) = 0;

			// 表示領域のクリア
			m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

			return false;
		}
	}


	// フラグ未成立はそのまま返却
	if ((*pIsOpen) == false)
	{
		return false;
	}

	// 選択しているアイテムを使用するためのフラグ切り替え
	if (m_pCCManager->PulseOn(KEY::RETURNKEY))
	{
		// 所持アイテムが0のときは無効
		if (iList.size() != 0)
		{
			// フラグ切り替え
			(*pItemUse) = true;
			(*pIsOpen) = false;
		}
	}

	if ((*pItemUse) == false)
	{
		// メニュー表示の更新フラグ
		bool isRefresh = false;
		// アイテム一覧の行数
		int listSize = m_pPlayer->GetItem().size();

		// ログを上にスクロール
		if (m_pCCManager->PulseOn(KEY::UPKEY))
		{
			if ((*pScrollCount) > 0)
			{
				isRefresh = true;
				(*pScrollCount)--;
			}
		}
		// ログを下にスクロール
		if (m_pCCManager->PulseOn(KEY::DOWNKEY))
		{
			if ((*pScrollCount) < listSize - 1)
			{
				isRefresh = true;
				(*pScrollCount)++;
			}
		}

		if (isRefresh == true)
		{
			// 表示領域のクリア
			m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

			COORD pos = m_LogPosition;
			int viewCount = 0;
			int passCount = 0;
			// スクロールしたメニューの再表示
			itemIter = iList.begin();

			// アイテムの名前を登録する
			std::vector<std::string> nameList;
			for (itemIter = iList.begin(); itemIter != iList.end(); itemIter++)
			{
				// アイテムの名称を取得
				std::string name = m_pItemList[(*itemIter).first].name + ":";
				::_itoa_s((*itemIter).second, buffer, sizeof(char) * 256, 10);
				name += buffer;

				// 現在指定しているアイテムの名称部分にカーソルをつける
				if (viewCount++ == (*pScrollCount))
				{
					name += selectCursor;
				}
				nameList.push_back(name);
			}

			// 登録した名前を表示する
			viewCount = 0;
			std::vector<std::string>::iterator nameIter;
			for (nameIter = nameList.begin(); nameIter != nameList.end(); nameIter++)
			{
				if (passCount++ < (*pScrollCount) - 2)
				{
					continue;
				}

				// アイテム一覧の表示
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_LogTXTColor, pos, (*nameIter), NOWAIT, DONOTDELAY);
				pos.Y++;
				if (++viewCount == viewHeight)
				{
					break;
				}
			}
			Sleep(10);
		}
	}
	else
	{
		// 表示領域のクリア
		m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

		// ==== 現在のスクロール量で指定されているアイテムのインデックス番号を取得する ====
		COORD pos = m_LogPosition;
		int passCount = 0;
		for (itemIter = iList.begin(); itemIter != iList.end(); itemIter++)
		{
			if (passCount++ != (*pScrollCount))
			{
				continue;
			}

			// 使用するアイテムのインデックス番号をプレイヤに指定する
			m_pPlayer->SetUseItemIndex((*itemIter).first);
			break;
		}

		// アイテム使用が決まってメニューを閉じるのでスクロール量をリセットする
		(*pScrollCount) = 0;

		return false;
	}

	return true;
}

// 関数名：PlayerActionInput	自機行動用入力確認
// 引　数：pNowPosition			現在の座標
// 　　　：pTarget				移動先の座標へのアドレス
// 　　　：pIsInput				入力があったかのフラグへのアドレス
// 　　　：pPlayerState			自機の状態へのアドレス
// 戻り値：
void CRPG::PlayerActionInput(COORD nowPosition, COORD* pTarget, bool* pIsInput, int* pPlayerState)
{
	// アイテムの取得
	if (m_pCCManager->PulseOn(KEY::RETURNKEY))
	{
		(*pIsInput)		= true;
		(*pPlayerState)	|= _PICKUP;
		return;
	}

	// 足踏み
	if ((m_pCCManager->PulseOn(KEY::SKEY))
	 || (m_pCCManager->Press(KEY::SKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		(*pIsInput)		= true;
		(*pPlayerState)	|= _FOOT;
		(*pPlayerState)	|= _ACTION;
		return;
	}

	// 上への移動
	if ((m_pCCManager->PulseOn(KEY::WKEY))
	 || (m_pCCManager->Press(KEY::WKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.Y > 0)
		{
			(*pTarget).Y--;
			(*pIsInput) = true;
			return;
		}
	}
	// 下への移動
	if ((m_pCCManager->PulseOn(KEY::XKEY))
	 || (m_pCCManager->Press(KEY::XKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.Y < m_Height - 1)
		{
			(*pTarget).Y++;
			(*pIsInput) = true;
			return;
		}
	}
	// 左への移動
	if ((m_pCCManager->PulseOn(KEY::AKEY))
	 || (m_pCCManager->Press(KEY::AKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X > 0)
		{
			(*pTarget).X--;
			(*pIsInput) = true;
			return;
		}
	}
	// 右への移動
	if ((m_pCCManager->PulseOn(KEY::DKEY))
	 || (m_pCCManager->Press(KEY::DKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X < m_Width - 1)
		{
			(*pTarget).X++;
			(*pIsInput) = true;
			return;
		}
	}
	// 左上への移動
	if ((m_pCCManager->PulseOn(KEY::QKEY))
	 || (m_pCCManager->Press(KEY::QKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X > 0 && nowPosition.Y > 0)
		{
			(*pTarget).X--;
			(*pTarget).Y--;
			(*pIsInput) = true;
			return;
		}
	}
	// 右上への移動
	if ((m_pCCManager->PulseOn(KEY::EKEY))
	 || (m_pCCManager->Press(KEY::EKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X < m_Width - 1 && nowPosition.Y > 0)
		{
			(*pTarget).X++;
			(*pTarget).Y--;
			(*pIsInput) = true;
			return;
		}
	}
	// 左下への移動
	if ((m_pCCManager->PulseOn(KEY::ZKEY))
	 || (m_pCCManager->Press(KEY::ZKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X > 0 && nowPosition.Y < m_Height - 1)
		{
			(*pTarget).X--;
			(*pTarget).Y++;
			(*pIsInput) = true;
			return;
		}
	}
	// 右下への移動
	if ((m_pCCManager->PulseOn(KEY::CKEY))
	 || (m_pCCManager->Press(KEY::CKEY) && m_pCCManager->Press(KEY::LSHIFTKEY)))
	{
		if (nowPosition.X < m_Width - 1 && nowPosition.Y < m_Height - 1)
		{
			(*pTarget).X++;
			(*pTarget).Y++;
			(*pIsInput) = true;
			return;
		}
	}
}

// 関数名：ItemPlace			アイテムの配置
// 引　数：ground				地形データ
// 戻り値：
void CRPG::ItemPlace(std::multimap<char, char> ground)
{
	// 変数
	COORD startPos;
	int size = ground.size();
	int number = rand() % size;

	// マップ開始時に落ちているアイテムの数
	int numItem = rand() % (m_NowMapNumber + 3) + 2;

	// 走査用イテレータ
	std::multimap<char, char>::iterator gIter = ground.begin();

	for (int i = 0; i < numItem;  /**/ )
	{
		// 配置場所をランダム指定
		size = ground.size();
		number = rand() % size;
		gIter = ground.begin();
		for (int j = 0; j < number; j++)
		{
			gIter++;
		}
		startPos.X = gIter->first;
		startPos.Y = gIter->second;

		// アイテム番号のランダム指定
		int index = rand() % m_NumAllItem;

		// ●●●●●●●●●●●●●●●●●●
		// 武具の出現は4階以降に強制指定
		if (m_NowMapNumber < 3)
		{
			int type = m_pItemList[index].type;
			if (type == ARMOR || type == WEAPON || type == SHIELD)
			{
				continue;
			}
		}
		// ●●●●●●●●●●●●●●●●●●


		// アイテムサンプルにデータを設定
		m_ItemBookSample.X = startPos.X;
		m_ItemBookSample.Y = startPos.Y;
		m_ItemBookSample.index = index;

		// アイテムを設定
		m_FallItemList.push_back(m_ItemBookSample);

		// アイテム同士も重複してしまうので要素を削除
		ground.erase(gIter);

		i++;
	}

}

// 関数名：PlayerPlace			アイテムの配置
// 引　数：ground				地形データ
// 戻り値：
void CRPG::PlayerPlace(std::multimap<char, char>* pGround)
{
	// 変数
	COORD startPos;
	int size = (*pGround).size();
	int number = rand() % size;

	// 地形リストのイテレータ
	std::multimap<char, char>::iterator gIter = (*pGround).begin();

	for (int j = 0; j < number; j++)
	{
		gIter++;
	}
	startPos.X = gIter->first;
	startPos.Y = gIter->second;

	// 要素を削除
	(*pGround).erase(gIter);

	// 座標の設定
	m_pPlayer->SetPosition(startPos);
}

// 関数名：EnemyPlace			敵の配置
// 引　数：pGround				地形データコンテナへのアドレス
// 戻り値：
void CRPG::EnemyPlace(std::multimap<char, char>* pGround)
{
	// 変数
	int cIndex = 0;
	COORD startPos;
	int size;
	int number;

	// 敵リストのイテレータ
	std::vector<CEnemy>::iterator eIt;

	// 地形リストのイテレータ
	std::multimap<char, char>::iterator gIter;

	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		// 地形をランダム指定
		size = (*pGround).size();
		number = rand() % size;
		gIter = (*pGround).begin();
		for (int j = 0; j < number; j++)
		{
			gIter++;
		}
		startPos.X = gIter->first;
		startPos.Y = gIter->second;

		// 要素の削除
		(*pGround).erase(gIter);

		// 敵に座標を設定する
		(*eIt).SetPosition(startPos);
		(*eIt).SetIndex(cIndex++);
	}
}

// 関数名：RefreshEnemyIndex	敵インデックスの更新
// 引　数：
// 戻り値：
void CRPG::RefreshEnemyIndex()
{
	int enemyIndex = 0;
	std::vector<CEnemy>::iterator eIt;
	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		(*eIt).SetIndex(enemyIndex++);
	}
}

// 関数名：ConfirmPlayerMove	移動の確認
// 引　数：isInput				入力の有無
// 　　　：pPlayerState			自機の状態へのアドレス
// 　　　：target				移動先の座標
// 戻り値：
void CRPG::ConfirmPlayerMove(bool isInput, int* pPlayerState, int* pTargetNumber, COORD target)
{
	if (isInput == false)
	{
		return;
	}

	if ( ! (   ( ! ((*pPlayerState) & _FOOT))
			&& ( ! ((*pPlayerState) & _PICKUP))
			&& ( ! ((*pPlayerState) & _USEITEM))))
	{
		return;
	}

	// 敵がいるかどうか
	bool enemyExist = false;
	std::vector<CEnemy>::iterator eIt;
	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		if ((*eIt).GetDeadFlag())
		{
			continue;
		}
		COORD enemy = (*eIt).GetPosition();
		if (EQUAL(target, enemy))
		{
			enemyExist = true;
			(*pTargetNumber) = (*eIt).GetIndex();
			break;
		}
	}
	if (enemyExist == true)
	{
		(*pPlayerState) |= _ACTION;
		(*pPlayerState) |= _ATTACK;
	}
	else
	{
		char after = m_pppMapData[m_NowMapNumber][target.Y][target.X];
		// enum MAP : char { NONE, WALL, GROUND, STAIR, };
		if (after == GROUND || after == STAIR)
		{
			(*pPlayerState) |= _ACTION;
		}
	}
}

// 関数名：ConfirmPlayerPickUp	アイテム拾得の確認
// 引　数：isInput				入力の有無
// 　　　：pPlayerState			自機の状態へのアドレス
// 戻り値：
std::vector<FALLITEMLIST>::iterator CRPG::ConfirmPlayerPickUp(bool isInput, int* pPlayerState)
{
	if (isInput == false)
	{
		return m_FallItemList.end();
	}

	if ( ! (   ( ! ((*pPlayerState) & _FOOT))
			&& (   ((*pPlayerState) & _PICKUP))
			&& ( ! ((*pPlayerState) & _USEITEM))))
	{
		return m_FallItemList.end();
	}

	// その場所にアイテムがあれば取得
	std::vector<FALLITEMLIST>::iterator itemIter;
	for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
	{
		COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
		// 自機と重なっているか
		if (EQUAL(itemPos, m_pPlayer->GetPosition()))
		{
			// アイテム取得が確定するので
			// 行動フラグを立てる
			(*pPlayerState) |= _ACTION;
			return itemIter;
		}
	}
	return m_FallItemList.end();
}

// 関数名：EnemyDistanceSort	敵の距離順ソート
// 引　数：
// 戻り値：
void CRPG::EnemyDistanceSort()
{
	//
	// 敵の呼び出し順序をコンテナへの登録順から自機からの距離が近い順へと変更する
	//

	// マルチマップ一時変数(挿入時ソートされる)
	std::multimap<int, std::vector<CEnemy>::iterator> dII;

	// イテレータ
	std::vector<CEnemy>::iterator eIt;

	// 距離とイテレータをセットにしてマルチマップに格納
	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
	{
		int distance = DISTANCE(m_pPlayer->GetPosition(), (*eIt).GetPosition());
		dII.insert(std::pair<int, std::vector<CEnemy>::iterator>(distance, eIt));
	}

	// 
	// ソートした順番で再構成してm_Eに戻す
	//

	// ベクタ一時変数
	std::vector<CEnemy> tmpEnemy;

	// イテレータ
	std::multimap<int, std::vector<CEnemy>::iterator>::iterator dIIiter;
	// 挿入時にソートが済んでいるので、イテレータで順次呼び出して格納
	for (dIIiter = dII.begin(); dIIiter != dII.end(); dIIiter++)
	{
		tmpEnemy.push_back((*(dIIiter->second)));
	}

	// クラス変数に代入して戻す
	m_Enemy = tmpEnemy;
}

// 関数名：EnemyDeadCheck		敵の死亡確認 (とログの表示)
// 引　数：
// 戻り値：
void CRPG::EnemyDeadCheck()
{
	// 地形記号 (仮)
	std::string symbol = "-";

	// イテレータ
	std::vector<CEnemy>::iterator eIt;

	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); /* */ )
	{
		if ((*eIt).GetDeadFlag())
		{
			COORD enemyPos = (*eIt).GetPosition();

			// 死亡ログ
			std::string str;
			str = (*eIt).GetName();
			str += "を倒した。";
			// 経験値計算とログ表示
			m_pPlayer->GetEXP((*eIt).GetEXPValue(), str, &m_pCCManager);
			// 現在地の塗りつぶし
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, enemyPos, symbol, NOWAIT, DONOTDELAY);

			// アイテム所持フラグがたっていればアイテムをドロップ
			if ((*eIt).CanDropItem() == true)
			{
				m_ItemBookSample.index = (*eIt).GetItemIndex();
				m_ItemBookSample.X = enemyPos.X;
				m_ItemBookSample.Y = enemyPos.Y;
				m_FallItemList.push_back(m_ItemBookSample);
				str = (*eIt).GetName() + "は";
				str += m_pItemList[(*eIt).GetItemIndex()].name + "を落とした。";
				m_pCCManager->AddLog(str);
			}

			// 要素を消去
			eIt = m_Enemy.erase(eIt);
			continue;
		}
		// イテレータを進める
		++eIt;
	}
}

// 関数名：PlayerAction			自機の行動
// 引　数：pPlayerState			自機の状態へのアドレス
// 　　　：targetNumber			攻撃対象となる敵のインデックス番号
// 　　　：pItemIter			拾得対象となるアイテムを指すイテレータへのアドレス
// 　　　：nowPosition			現在地
// 　　　：target				移動先の場所
// 戻り値：
void CRPG::PlayerAction(int* pPlayerState, int targetNumber,
	std::vector<FALLITEMLIST>::iterator* pItemIter, COORD nowPosition, COORD target)
{
	// 敵コンテナのイテレータ
	std::vector<CEnemy>::iterator eIt;

	// 拾得可能アイテムコンテナのイテレータ
	std::vector<FALLITEMLIST>::iterator itemIter = (*pItemIter);

	// 自機のアイテムリスト
	ItemList& IList = m_pPlayer->GetItem();
	ItemList::iterator uiIter;

	if ((*pPlayerState) & _ATTACK)
	{
		// ==== 攻撃 ====
		int strength = m_pPlayer->GetATK();
		int damage;
		eIt = m_Enemy.begin();
		for (int i = 0; i < targetNumber; i++)
		{
			eIt++;
		}
		damage = (*eIt).ReceiveAttack(strength);

		// 自機の行動カウンタを勧める
		m_pPlayer->Foot(false);

		// ログを表示
		std::string cleaner(m_pCCManager->GetScreenSize().Right, ' ');
		std::string str;
		char buffer[256];
		str = m_pPlayer->GetName() + "は[" + (*eIt).GetSymbol() + "]" + (*eIt).GetName() + "を攻撃！";
		if (damage > 0)
		{
			::_itoa_s(damage, buffer,256 * sizeof(char),10);
			str += buffer;
			str += "のダメージを与えた！";
		}
		else
		{
			str += "ダメージが通らなかった！";
		}
		m_pCCManager->AddLog(str);
	}
	else if (((*pPlayerState) & _FOOT) || ((*pPlayerState) & _PICKUP))
	{
		// ==== 待機/アイテムの拾得 ====
		if ((*pPlayerState) & _PICKUP && itemIter != m_FallItemList.end())
		{
			// 入力確認時に走査してヒットしたアイテムをそのまま登録する
			// 所持アイテムに登録
			m_pPlayer->AddItem((*itemIter).index);
			// 取得ログの作成と設定
			std::string str = m_pPlayer->GetName() + "は" + m_pItemList[(*itemIter).index].name + "を手に入れた。";
			m_pCCManager->AddLog(str);
			// 要素の削除
			m_FallItemList.erase(itemIter);
			// アイテムがなくなるので元背景色で塗りつぶして再表示
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_pPlayer->GetSymbolTXTColor(), target, m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}
		m_pPlayer->Foot(true);
	}
	else if ((*pPlayerState) & _USEITEM)
	{
		// ==== アイテムの使用 ====
		ITEM nowUseItem = m_pItemList[m_pPlayer->GetUseItemIndex()];
		int oldMaxValue;
		int newMaxValue;
		int healValue;
		char buffer[256];
		std::string str;
		

		switch (nowUseItem.type)
		{
		case MEDICINE:
			// 使用ログの作成と設定
			str = m_pPlayer->GetName() + "は" + nowUseItem.name + "を食べた。";
			m_pCCManager->AddLog(str);
			oldMaxValue = m_pPlayer->GetMaxHP();
			healValue = m_pPlayer->Heal(nowUseItem.value);
			newMaxValue = m_pPlayer->GetMaxHP();
			// 結果の表示
			if (nowUseItem.value == healValue)
			{
				::_itoa_s(nowUseItem.value, buffer, 256 * sizeof(char), 10);
				str  = "体力が";
				str += buffer;
				str += "回復した。";
			}
			else
			{
				str = "体力が全快して元気が出た。";
			}
			m_pCCManager->AddLog(str);
			// 最大値増加があれば追加表示
			if (oldMaxValue < newMaxValue)
			{
				str = "体力の最大値が";
				::_itoa_s(newMaxValue, buffer, 256 * sizeof(char), 10);
				str += buffer;
				str += "になった。";
				m_pCCManager->AddLog(str);
			}
			break;
		case FOOD:
			// 使用ログの作成と設定
			str = m_pPlayer->GetName() + "は" + nowUseItem.name + "を食べた。";
			m_pCCManager->AddLog(str);
			oldMaxValue = m_pPlayer->GetHungryLimit();
			healValue = m_pPlayer->EatFood(nowUseItem.value);
			newMaxValue = m_pPlayer->GetHungryLimit();
			// 結果の表示
			if (nowUseItem.value == healValue)
			{
				::_itoa_s(nowUseItem.value, buffer, 256 * sizeof(char), 10);
				str = "満腹度が";
				str += buffer;
				str += "回復した。";
			}
			else
			{
				str = "おなかが一杯になってとても満たされた。";
			}
			m_pCCManager->AddLog(str);
			// 最大値増加があれば追加表示
			if (oldMaxValue < newMaxValue)
			{
				str = "満腹度の最大値が";
				::_itoa_s(newMaxValue, buffer, 256 * sizeof(char), 10);
				str += buffer;
				str += "になった。";
				m_pCCManager->AddLog(str);
			}
			break;
		case WEAPON:
			// 装着ログの作成と設定
			str = m_pPlayer->GetName() + "は" + nowUseItem.name + "を装備した。";
			m_pCCManager->AddLog(str);
			// アイテムの番号を設定する
			m_pPlayer->EquippedWeapon(nowUseItem.itemIndex);
			break;
		case SHIELD:
			// 装着ログの作成と設定
			str = m_pPlayer->GetName() + "は" + nowUseItem.name + "を装備した。";
			m_pCCManager->AddLog(str);
			// アイテムの番号を設定する
			m_pPlayer->EquippedShield(nowUseItem.itemIndex);
			break;
		case ARMOR:
			// 装着ログの作成と設定
			str = m_pPlayer->GetName() + "は" + nowUseItem.name + "を装備した。";
			m_pCCManager->AddLog(str);
			// アイテムの番号を設定する
			m_pPlayer->EquippedArmor(nowUseItem.itemIndex);
			break;
		default:
			assert(false);
			break;
		}

		if (nowUseItem.type == MEDICINE || nowUseItem.type == FOOD)
		{
			// アイテム数の削減とリストからの削除
			uiIter = IList.find(nowUseItem.itemIndex);
			uiIter->second--;
			if (uiIter->second == 0)
			{
				IList.erase(uiIter);
			}
			m_pPlayer->SetItemList(std::move(IList));
		}
	}
	else
	{
		// ==== 移動 ====
		// 移動前の位置を地形で上塗り
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, nowPosition, m_TerrainSymbol[GROUND], NOWAIT, DONOTDELAY);
		nowPosition = target;

		// アイテムを踏むかを確認
		bool isOverRap = false;
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
			if (EQUAL(itemPos, nowPosition))
			{
				isOverRap = true;
				break;
			}
		}

		// 移動先の位置に自機を表示
		if (isOverRap == true)
		{
			// アイテムを踏んでいたらアイテムの背景色を表示
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_pPlayer->GetSymbolTXTColor(), target, m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}
		else
		{
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_pPlayer->GetSymbolTXTColor(), target, m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}

		m_pPlayer->Move(nowPosition);
	}
}

// 関数名：EnemyAction			生存している敵の行動
// 引　数：
// 戻り値：
void CRPG::EnemyAction()
{
	COORD oldPos;
	COORD newPos;

	// 敵コンテナのイテレータ
	std::vector<CEnemy>::iterator eIt;

	for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); /**/ )
	{
		for (int j = 0; j < (*eIt).GetNumAction(); j++)
		{
			oldPos = (*eIt).GetPosition();
			(*eIt).DecideAction(&m_pPlayer, &m_Enemy, m_Enemy.size(), &m_pppMapData[m_NowMapNumber], m_Width, m_Height, &m_pCCManager);
			newPos = (*eIt).GetPosition();

			// 移動があった場合元の場所に元の地形を表示する
			if ( false == EQUAL(oldPos, newPos))
			{
				// 現在地の塗りつぶし
				m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, oldPos, m_TerrainSymbol[GROUND], NOWAIT, DONOTDELAY);
			}
		}

		// 座標の更新
		newPos = (*eIt).GetPosition();

		// アイテムを踏んでいるかのフラグ
		bool isOverRap = false;

		// 拾得可能アイテムコンテナのイテレータ
		std::vector<FALLITEMLIST>::iterator itemIter;

		// 生存している敵の場所に敵マークを表示
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
			// アイテムを踏んでいたらアイテムの背景色を表示
			if (EQUAL(itemPos, newPos))
			{
				isOverRap = true;
				break;
			}
		}
		if (isOverRap == true)
		{
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, (*eIt).GetTXTColor(), newPos, (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
		}
		else
		{
			m_pCCManager->WriteTextCharacter(m_GameBGColor, (*eIt).GetTXTColor(), newPos, (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
		}

		// イテレータを進める
		++eIt;
	}
}

// 関数名：ViewItem				アイテムの表示
// 引　数：
// 戻り値：
void CRPG::ViewItem()
{
	// イテレータ
	std::vector<FALLITEMLIST>::iterator itemIter;

	// アイテムコンテナの要素全て
	for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
	{
		// 座標
		COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);

		// ==== 自機と重なっているか ====
		if (EQUAL(itemPos, m_pPlayer->GetPosition()))
		{
			continue;
		}

		// ==== 敵と重なっているか ====
		bool canView = true;
		std::vector<CEnemy>::iterator eIt;
		for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++ )
		{
			if (false == (*eIt).GetDeadFlag() && EQUAL(itemPos, (*eIt).GetPosition()))
				{
					canView = false;
					break;
				}
		}
		if (canView == false)
		{
			continue;
		}

		// 表示
		m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_ItemTXTColor, itemPos, m_ItemSymbol, NOWAIT, DONOTDELAY);
	}
}

// 関数名：ConfirmDungeonState	階層クリア、死亡、ゴール時のログ表示と遷移
// 引　数：pos					表示位置
// 戻り値：
int CRPG::ConfirmDungeonState()
{
	//enum DUNGEONENDMODE : int { PLAYERDEAD, HIERARCHYCLEAR, DUNGEONCLEAR, CONTINUATION, };

	// 画面のクリア
	m_pCCManager->FillAll(m_GameBGColor, m_GameTXTColor, m_pCCManager->GetCSBInfo());

	// 表示位置 (中心)
	COORD center = {m_Width / 2, m_Height / 2};

	// 自機の死亡
	if (true == m_pPlayer->IsDead())
	{
		m_NowMapNumber = 0;
		m_GameScene = DEATH;
		std::string view  = "## YOU DIED. OH MY GODDESS X( ##";
		std::string clean = "                                                ";
		center.X -= view.size() / 2;
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_RED, center, view, WAIT, DONOTDELAY);
		m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, center, clean, NOWAIT, DONOTDELAY);
		return PLAYERDEAD;
	}
	else
	{
		// 階層のクリア
		if (++m_NowMapNumber != m_NumMap)
		{
			std::string view  = "## GOAL!! GO TO THE NEXT FLOOR. PRESS ENTER KEY.##";
			std::string clean = "                                                  ";
			center.X -= view.size() / 2;
			m_pCCManager->WriteTextCharacter(m_GameBGColor, H_RED, center, view, WAIT, DONOTDELAY);
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, center, clean, NOWAIT, DONOTDELAY);
			return HIERARCHYCLEAR;
		}
		// ダンジョン全体のクリア
		else
		{
			std::string view  = "## DUNGEON CLEAR!! PRESS ENTER KEY TO FINISH.##";
			std::string clean = "                                               ";
			center.X -= view.size() / 2;
			m_pCCManager->WriteTextCharacter(m_GameBGColor, H_RED, center, view, WAIT, DONOTDELAY);
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_GameTXTColor, center, clean, NOWAIT, DONOTDELAY);
			
			// ◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
			// 街に帰るか、クリアになるか
			m_GameScene = CREDIT;
			// ◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇

			m_NowMapNumber = 0;
			return DUNGEONCLEAR;
		}
	}
	return CONTINUATION;
}