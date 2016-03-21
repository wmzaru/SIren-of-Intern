#include "RPG.h"

// 関数名：Dungeon			ダンジョンシーン
// 引　数：
// 戻り値：
void CRPG::Dungeon()
{
	// ◇◇◇◇ ダンジョン階層ループ ◇◇◇◇
	for (;;)
	{
		// ==== 画面のクリア ====
		m_pCCManager->FillAll(m_GameBGColor, m_GameTXTColor, m_pCCManager->GetCSBInfo());

		// ==== カーソルの非表示 ====
		m_pCCManager->ExchangeCursorVisible(FALSE);

		// ==== 移動可能地形のサンプリング ====
		std::multimap<char, char> ground = ViewMapAndGetGround();
		// 保存用に複写
		std::multimap<char, char> saveAllGround = ground;
		// 走査用イテレータ
		std::multimap<char, char>::iterator gIter;

		COORD pos = {0, 0};
		std::string symbol;

		// ==== アイテムの初期化(仮) ====
		ResetItem();
		// ==== アイテムの配置 ====
		ItemPlace(ground);
		// 地形データの復元
		ground = saveAllGround;

		// ==== 自機の配置 ====
		PlayerPlace(&ground);

		// ==== 敵の初期化 ====
		ResetEnemy();

		// ●● 数・種類はマネージャを作って管理したい ●●●●●●●●●●
		// 初期敵数の決定
		m_NumStartMaxEnemy = ground.size() / 25; // 4%
		unsigned int numEnemy = rand() % m_NumStartMaxEnemy + (m_NowMapNumber + 1) * 2; // + α
		// 初期の敵数は配置できる数に収める
		if (numEnemy >= ground.size() - 1)
		{
			numEnemy = ground.size() - 1;
		}
		// 敵種類の決定
		for (unsigned int i = 0; i < numEnemy; i++)
		{
			int randomIndex;
			// 5階層ごとに5種類ずつ
			randomIndex = ((rand() % 5) + (5 * (m_NowMapNumber / 5))) % m_NumEnemyType;
			// 図鑑データの取得
			SetEnemyData(randomIndex);
			// 敵の登録
			m_Enemy.push_back(m_Sample);
		}
		numEnemy = m_Enemy.size();
		// ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

		// ==== 敵機の配置 ====
		std::vector<CEnemy>::iterator eIt;
		int enemyIndex = 0;
		EnemyPlace(&ground);

		// 地形データの復元
		ground = std::move(saveAllGround);

		// インフォ表示用の開始位置
		COORD infoPos  = {0, 0};
		COORD helpPos1 = {0, 0};
		COORD helpPos2 = {0, 0};
		helpPos1.Y = m_pCCManager->GetScreenSize().Bottom - 1;
		helpPos2.Y = helpPos1.Y + 1;
		infoPos.Y = helpPos1.Y - 1;

		// アイテムの表示
		std::vector<FALLITEMLIST>::iterator itemIter;
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_ItemTXTColor, MAKECOORD((*itemIter).X, (*itemIter).Y), m_ItemSymbol, NOWAIT, DONOTDELAY);
		}

		// ==== 自機情報の表示 ====
		m_pPlayer->ViewInfo(infoPos, &m_pCCManager);

		// ==== 階層番号・ヘルプの表示 ====
		// 階層番号
		char buf[256];
		::_itoa_s(m_NowMapNumber + 1, buf, sizeof(char) * 256, 10);
		std::string mapNumberStr = "ダンジョン：チカホ";
		mapNumberStr += buf;
		mapNumberStr += "階";
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, m_MapNamePosition, mapNumberStr, NOWAIT, DONOTDELAY);
		// ヘルプ
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos1, m_Help[0], NOWAIT, DONOTDELAY);
		m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos2, m_Help[1], NOWAIT, DONOTDELAY);

		// アイテムを踏んでいるかのフラグ
		bool isOverRap = false;

		// 自機の表示
		isOverRap = false;
		for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
		{
			COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
			// アイテムを踏んでいたらアイテムの背景色を表示
			if (EQUAL(itemPos, m_pPlayer->GetPosition()))
			{
				isOverRap = true;
				break;
			}
		}
		if (isOverRap == true)
		{
			m_pCCManager->WriteTextCharacter(m_ItemBGColor, m_pPlayer->GetSymbolTXTColor(), m_pPlayer->GetPosition(), m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}
		else
		{
			m_pCCManager->WriteTextCharacter(m_GameBGColor, m_pPlayer->GetSymbolTXTColor(), m_pPlayer->GetPosition(), m_pPlayer->GetSymbol(), NOWAIT, DONOTDELAY);
		}

		// ==== 敵機の表示 ====
		for (eIt = m_Enemy.begin(); eIt != m_Enemy.end(); eIt++)
		{
			isOverRap = false;
			for (itemIter = m_FallItemList.begin(); itemIter != m_FallItemList.end(); itemIter++)
			{
				COORD itemPos = MAKECOORD((*itemIter).X, (*itemIter).Y);
				// アイテムを踏んでいたらアイテムの背景色を表示
				if (EQUAL(itemPos, (*eIt).GetPosition()))
				{
					isOverRap = true;
					break;
				}
			}
			if (isOverRap == true)
			{
				m_pCCManager->WriteTextCharacter(m_ItemBGColor, (*eIt).GetTXTColor(), (*eIt).GetPosition(), (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(m_GameBGColor, (*eIt).GetTXTColor(), (*eIt).GetPosition(), (*eIt).GetSymbol(), NOWAIT, DONOTDELAY);
			}
		}

		// 行動ログスクロール表示用
		int scrollCount = 0;
		int numScroll = 0;

		// 行動ログの消去
		m_pCCManager->CleanLog();
		m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);

		// メニュー表示用
		bool isOpenMenu = false;
		int menuScrollCount = 0;

		// ◇◇◇◇ ダンジョン内行動ループ ◇◇◇◇
		for(;;)
		{
			// キー入力検出
			m_pCCManager->Trigger();

			// 敵管理が追加を決定したら
			// 追加で配置できる場所を走査し、その位置に敵を配置する

			// 敵インデックスの更新
			RefreshEnemyIndex();

			// ==== 入力：自機行動 ====
			COORD nowPosition = m_pPlayer->GetPosition();
			COORD target = nowPosition;
			// フラグ
			bool isInput = false;	// 自機行動用の入力の有無
			bool useItem = false;
			//enum STATE : int { _ACTION = 0x0001, _FOOT = 0x0002, _ATTACK = 0x0004, _PICKUP = 0x0008, _USEITEM = 0x0010, };
			int playerState = 0;
			// 攻撃可能な相手のインデックス番号
			int targetNumber = 0;
			// 使用するアイテムの番号
			int selectItemIndex = 0;
			ItemList& IList = m_pPlayer->GetItem();
			ItemList::iterator uiIter = IList.begin();

			// ==== メニュー表示 ====
			if (true == Menu(&isOpenMenu, &useItem, &menuScrollCount))
			{
				continue;
			}

			// アイテム使用の要求があった場合、通常操作よりも優先する
			if (useItem == true)
			{
				isInput = true;
				playerState |= _USEITEM;
				playerState |= _ACTION;
			}
			else
			{
				// ==== 入力：ログスクロール ====
				if (true == LogScroll(&scrollCount, &numScroll))
				{
					continue;
				}

				// 自機行動用の入力確認
				PlayerActionInput(nowPosition, &target, &isInput, &playerState);

				// 入力時にPLAYERが移動可能かを判定
				ConfirmPlayerMove(isInput, &playerState, &targetNumber, target);

				// アイテムが拾えるかを確認
				itemIter = ConfirmPlayerPickUp(isInput, &playerState);
			}


			// ==== 入力確定時の行動処理 (とログ表示) ====
			if (playerState & _ACTION)
			{
				// 行動が確定したので保持していたログを消去する
				m_pCCManager->CleanLog();
				scrollCount = 0;
				numScroll = 0;

				// ■■■■ 自機の行動 ■■■■
				PlayerAction(&playerState, targetNumber, &itemIter, nowPosition, target);

				// ==== 自機死亡判定(空腹) ====
				if (true == m_pPlayer->IsDead())
				{
					break;
				}

				// ==== 到達判定 ====
				nowPosition = m_pPlayer->GetPosition();
				if (STAIR == m_pppMapData[m_NowMapNumber][nowPosition.Y][nowPosition.X])
				{
					break;
				}

				// ■■■■ 敵の行動 ■■■■
				// ==== 敵を距離順にソート ====
				EnemyDistanceSort();
			
				// ==== 死亡した敵を走査して経験値計算とログを表示 ====
				EnemyDeadCheck();

				// ==== 生存している敵の行動 ====
				EnemyAction();

				// 自機死亡判定(敵からの攻撃)
				if (true == m_pPlayer->IsDead())
				{
					break;
				}

				// ■■■■ アイテムの表示 ■■■■
				ViewItem();

				// 自機情報の表示
				m_pPlayer->ViewInfo(infoPos, &m_pCCManager);
				// ヘルプの表示
				m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos1, m_Help[0], NOWAIT, DONOTDELAY);
				// ヘルプの表示
				m_pCCManager->WriteTextCharacter(m_GameBGColor, H_BLACK, helpPos2, m_Help[1], NOWAIT, DONOTDELAY);

				// 情報ログの表示
				m_pCCManager->CleanLogRect(m_LogPosition, m_GameBGColor, m_LogTXTColor, m_WindowLogSize);
				m_pCCManager->ViewLog(m_LogPosition, m_GameBGColor, m_LogTXTColor, 0, m_WindowLogSize);

			}// end if (canAction == true)
			
			// 待機時間の設定
			Sleep(10);
		}

		// ==== ダンジョンパートの終了処理 (と表示) ====
		//enum DUNGEONENDMODE : int { PLAYERDEAD, HIERARCHYCLEAR, DUNGEONCLEAR, CONTINUATION, };
		int endMode = ConfirmDungeonState();
		if (endMode == PLAYERDEAD || endMode == DUNGEONCLEAR)
		{
			break;
		}
	}
}