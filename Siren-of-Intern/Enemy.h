#ifndef __ENEMY_H__
#define __ENEMY_H__
#pragma once

#include <Windows.h>
#include <vector>
#include <map>
#include <iterator>
#include "GlobalInfo.h"
#include "ConsoleManager.h"
#include "Player.h"

// ---- 構造体宣言 ----
typedef struct _tagEnemyList_
{
	int			bookListIndex;	// 図鑑番号
	std::string name;			// 名称
	std::string symbol;			// 表示記号
	COLOR		txtColor;		// 文字色
	int			maxHP;			// 最大体力値
	int			expValue;		// 所持経験値
	int			strength;		// 攻撃力
	int			deffence;		// 防御力
	int			moveTime;		// 行動可能回数
	int			mode;			// 初期状態(巡回、追尾、徘徊)
	bool		isTimid;		// 逃走の可否(臆病かどうか)
	int			searchDistance;	// 索敵距離

}ENEMYLIST;

// ---- クラス宣言 ----
class CEnemy
{
private:
	std::string m_Name;			// 名称
	int			m_NowHP;		// 現在体力値
	int			m_MaxHP;		// 最大体力値
	int			m_EXPValue;		// 所持経験値
	int			m_Strength;		// 攻撃力
	int			m_Deffence;		// 防御力
	int			m_MoveTime;		// 行動可能回数
	COORD		m_Position;		// 現在位置
	bool		m_CanDrop;		// アイテム所持の有無
	int			m_DropItemIndex;// ドロップアイテム番号
	int			m_Mode;			// 現在の状態(巡回、追尾、徘徊)
	bool		m_IsTimid;		// 逃走の可否(臆病かどうか)
	int			m_SearchDistance;// 索敵距離
	int			m_BookListIndex; // 図鑑番号
	std::string m_Symbol;		// 表示記号
	COLOR		m_TXTColor;		// 文字色
	int			m_Index;		// 識別番号
	bool		m_IsDead;		// 死亡フラグ

public:
	CEnemy(){}
	~CEnemy();

	// デバッグ用
	void ViewInfo(COORD&, CConsoleManager**) const;	// 情報表示

	void Initialize();					// 初期化
	void DecideAction(CPlayer**, std::vector<CEnemy>*, int, char***, int, int, CConsoleManager**); // 行動決定(AI)
	void Move(COORD&);						// 移動
	int ReceiveAttack(int&);				// 攻撃をうける
	void Damage(int&&);						// 被ダメージ
	void DeadorAlive();						// 死亡判定
	void SetBookData(ENEMYLIST);			// 図鑑データの登録

	void SetName(std::string name)	{		 m_Name = name;			}
	int GetHP() const				{ return m_NowHP;				}
	void SetHP(int value)			{		 m_NowHP = value;		}
	bool GetDeadFlag()				{ return m_IsDead;				}
	COORD GetPosition()	const		{ return m_Position;			}
	void SetPosition(COORD& pos)	{		 m_Position = pos;		}
	void SetPosition(int& x, int& y)
	{
		COORD pos = {x, y};
		m_Position = pos;
	}
	int GetNumAction()				{ return m_MoveTime;			}
	std::string GetName()			{ return m_Name;				}
	int GetIndex()					{ return m_Index;				}
	void SetIndex(int value)		{		 m_Index = value;		}
	int GetEXPValue()				{ return m_EXPValue;			}
	std::string GetSymbol()			{ return m_Symbol;				}
	COLOR GetTXTColor()				{ return m_TXTColor;			}
	void SetItemHaving(bool flag, int value)
	{
		m_CanDrop = flag;
		m_DropItemIndex = value;
	}
	bool CanDropItem()				{ return m_CanDrop;				}
	int GetItemIndex()				{ return m_DropItemIndex;		}
};

#endif