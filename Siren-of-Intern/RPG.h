#ifndef __TREASUREHUNT_H__
#define __TREASUREHUNT_H__
#pragma once

#include <Windows.h>
#include <tuple>
#include <map>
#include <iterator>
#include "GlobalInfo.h"
#include "LoadData.h"
#include "ConsoleManager.h"
#include "Player.h"
#include "Enemy.h"

// ---- クラス宣言 ----

class CRPG
{
private:
	CConsoleManager*		m_pCCManager;		// コンソール出力制御クラス
	char					m_GameScene;		// ゲームシーン

	// ゲーム用フラグ
	bool					m_Retry;			// 再挑戦フラグ
	bool					m_EndFrag;			// 終了フラグ

	// マップデータ
	int						m_NumMap;			// マップ数
	int						m_NowMapNumber;		// 現在のマップ番号
	std::string*			m_pMapFileName;		// マップデータのファイル名格納用ポインタ
	char***					m_pppMapData;		// マップデータ格納用ポインタ
	int						m_Width;			// マップ横幅の要素数
	int						m_Height;			// マップ縦幅の要素数
	std::string				m_TerrainSymbol[NUMTERRAIN]; // 地形の種類
	

	// 表示ログ
	COORD					m_MapNamePosition;	// マップ名の表示位置
	int						m_WindowLogSize;	// ゲームログの縦幅
	COORD					m_LogPosition;		// ログの表示位置
	COLOR					m_GameBGColor;		// 背景色
	COLOR					m_GameTXTColor;		// 文字色
	COLOR					m_LogTXTColor;		// ログ文字色
	std::string				m_Help[2];			// ヘルプ表示

	// 自機
	CPlayer*				m_pPlayer;			// プレイヤクラス

	// 敵
	int						m_NumEnemyType;		// 敵の種類数
	ENEMYLIST*				m_pEnemyBookList;	// 敵図鑑データ
	int						m_NumStartMaxEnemy; // 開始時の敵の最大数
	CEnemy					m_Sample;			// 図鑑参照用敵クラス
	std::vector<CEnemy>		m_Enemy;			// 現在出現している敵

	// アイテム
	COLOR					m_ItemBGColor;		// アイテムの背景色
	COLOR					m_ItemTXTColor;		// アイテムの文字色
	std::string				m_ItemSymbol;		// アイテムの表示記号
	int						m_numItem;			// フィールド上のアイテムの数
	int						m_NumAllItem;		// アイテム総種類数
	ITEM*					m_pItemList;		// アイテム図鑑
	FALLITEMLIST			m_ItemBookSample;	// アイテム標本
	std::vector<FALLITEMLIST> m_FallItemList;	// 落ちているアイテム

	// クラス化する
/*	typedef struct _tagEnemyManager_
	{
		// 出現率や出現種類、最大数などを決めたらマップデータから取得する
		int						m_NumEnemy;			// 敵の数
		int*					m_NumEnemyType;		// 出現する敵の種類数
		std::vector<CEnemy>		m_Enemy;			// 敵クラス
	}EMANAGER;
	*/

public:
	CRPG();
	~CRPG();

	bool CreateGameData();		// データの作成
	void Initialize();			// 初期化
	void Create();				// 生成
	void Release();				// リソースの開放
	void GameLoop();			// ゲームループ
	void Title();				// タイトルシーン
	void Dungeon();				// ダンジョンシーン
	void GameOver();			// ゲームオーバーシーン
	void CreditRoll();			// クレジットシーン

	// ダンジョンシーン内部処理
	std::multimap<char, char> ViewMapAndGetGround(); // マップデータから地面を抽出する
	void SetEnemyData(int);		// インデックス番号で呼び出した敵データを参照
	void ResetEnemy();			// 登録した敵の消去
	void SetItemData(int);		// インデックス番号で呼び出したアイテムデータを参照
	void ResetItem();			// 登録した敵の消去
	bool Menu(bool*, bool*, int*);	// メニュー表示
	bool LogScroll(int*, int*);	// ログスクロール
	void PlayerActionInput(COORD, COORD*, bool*, int*);	// 自機行動用入力確認
	void ItemPlace(std::multimap<char, char>);		// アイテムの再配置
	void PlayerPlace(std::multimap<char, char>*);	// 自機の再配置
	void EnemyPlace(std::multimap<char, char>*);	// 敵の再配置
	void RefreshEnemyIndex();	// 敵インデックスの更新
	void ConfirmPlayerMove(bool, int*, int*, COORD);					// 移動可能かを確認
	std::vector<FALLITEMLIST>::iterator ConfirmPlayerPickUp(bool, int*);// アイテム取得可能かを確認
	void EnemyDistanceSort();	// 敵の距離順ソート
	void EnemyDeadCheck();		// 敵の死亡確認 (とログの表示)
	// 自機の行動
	void PlayerAction(int*, int, std::vector<FALLITEMLIST>::iterator*, COORD, COORD);
	void EnemyAction();			// 生存している敵の行動
	void ViewItem();			// アイテムの表示
	int ConfirmDungeonState();	// 階層クリア、死亡、ゴール時のログ表示と遷移
};
#endif